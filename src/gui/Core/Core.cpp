#include "gui/Core/Core.hpp"

#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QTimer>
#include <QUrl>

#include "ies/Common/IntegralRangeUsing.hpp"
#include "ies/Common/SmartEnum.hxx"
#include "ies/StdUtil/Find.hxx"
#include "ies/String/SplitString.hpp"
#include "ies/Time/TimeUtilFormat.hxx"

#include "score2dx/Iidx/Definition.hpp"
#include "score2dx/Iidx/Version.hpp"

#include "gui/version.hpp"
#include "gui/Core/MeWorkerThread.hpp"

namespace s2Time = ies::Time;

namespace gui
{

IES_SMART_ENUM(CsvTableColumn,
    DateTime,
    Filename,
    Version,
    TotalPlayCount
);

Core::
Core(QObject *parent)
:   QObject(parent)
{
    for (auto playStyle : score2dx::PlayStyleSmartEnum::ToRange())
    {
        mPlayStyleList << ToString(playStyle).c_str();
    }

    for (auto versionIndex : IndexRange{0, score2dx::VersionNames.size()})
    {
        mVersionNameList << score2dx::VersionNames.at(versionIndex).c_str();
    }

    for (auto difficulty : score2dx::DifficultySmartEnum::ToRange())
    {
        mDifficultyList << ToString(difficulty).c_str();
    }
}

QString
Core::
getScore2dxVersion()
const
{
    static const std::string annotate = "";
    auto version =
        QString::number(SCORE_2DX_GUI_VERSION_MAJOR)
        +"."+QString::number(SCORE_2DX_GUI_VERSION_MINOR)
        +"."+QString::number(SCORE_2DX_GUI_VERSION_PATCH);
    if (!annotate.empty())
    {
        version += ("-"+annotate).c_str();
    }
    return version;
}

QString
Core::
getDbFilename()
const
{
    QString path{mCore.GetMusicDatabase().GetFilename().c_str()};
    //'' remove 'table/'.
    auto filename = path.right(path.size()-6);
    return filename;
}

bool
Core::
addPlayer(const QString &iidxId)
{
    //'' valid IIDX ID is in form of '5483-7391'.
    //'' if user enter '54837391', still accept and insert '-' at pos 4.
    auto id = iidxId;
    if (!iidxId.contains('-')&&iidxId.size()>=4)
    {
        id.insert(4, '-');
    }
    auto isIidxId = score2dx::IsIidxId(id.toStdString());
    if (isIidxId)
    {
        mCore.AddPlayer(id.toStdString());
        UpdatePlayerList();
    }
    return isIidxId;
}

void
Core::
loadDirectory(const QString &fileUrl)
{
    QUrl url{fileUrl};
    auto directory = url.toLocalFile().toStdString();
    mCore.LoadDirectory(directory);
    std::cout << std::flush;

    UpdatePlayerList();
}

QString
Core::
findMeUserIidxId(const QString &inputMeUserName)
{
    auto iidxId = mCore.AddIidxMeUser(inputMeUserName.toStdString());
    return iidxId.c_str();
}

void
Core::
downloadMe(const QString &meUserName)
{
    mIsDownloadingMe = true;
    emit isDownloadingMeChanged();

    auto *workerThread = new MeWorkerThread(mCore, meUserName, this);
    connect(workerThread, &MeWorkerThread::ResultReady,
            [this](const QString &errorMessage)
            {
                if (!errorMessage.isEmpty())
                {
                    qDebug() << "Download ME error: " << errorMessage;
                }

                UpdatePlayerList();

                mIsDownloadingMe = false;
                emit isDownloadingMeChanged();
            }
    );
    connect(workerThread, &MeWorkerThread::finished, workerThread, &QObject::deleteLater);
    workerThread->start();
}

void
Core::
downloadIst(const QString &iidxId,
            const QString &versions,
            const QString &styles,
            bool runInPowerShell)
{
    mIsDownloadingIst = true;
    emit isDownloadingIstChanged();

    std::set<std::string> scrapVersions;
    std::set<score2dx::PlayStyleAcronym> scrapStyles;

    auto versionsStr = versions.toStdString();
    auto splitVerions = ies::SplitString(", ", versionsStr);
    for (auto &v : splitVerions)
    {
        if (v.size()==2&&std::all_of(v.begin(), v.end(), ::isdigit))
        {
            scrapVersions.emplace(v);
        }
    }

    if (scrapVersions.empty())
    {
        scrapVersions = {"28"};
    }

    auto stylesStr = styles.toStdString();
    auto splitStyles = ies::SplitString(", ", stylesStr);
    for (auto &style : splitStyles)
    {
        if (score2dx::PlayStyleAcronymSmartEnum::Has(style))
        {
            scrapStyles.emplace(score2dx::ToPlayStyleAcronym(style));
        }
    }

    if (scrapStyles.empty())
    {
        scrapStyles = {score2dx::PlayStyleAcronym::SP};
    }

    std::ofstream configFile{"config.txt"};

    configFile << "id = " << iidxId.toStdString() << "\n";

    configFile << "version = [";
    for (auto &v : scrapVersions)
    {
        if (v!=*scrapVersions.begin())
        {
            configFile << ", ";
        }
        configFile << v;
    }
    configFile << "]\n";

    configFile << "style = [";
    for (auto &style : scrapStyles)
    {
        if (style!=*scrapStyles.begin())
        {
            configFile << ", ";
        }
        configFile << ToString(style);
    }
    configFile << "]\n";

    configFile.close();

    auto process = new QProcess(this);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, iidxId](int exitCode, QProcess::ExitStatus exitStatus)
            {
                Q_UNUSED(exitCode);
                Q_UNUSED(exitStatus);

                auto succeeded = mCore.LoadDirectory("./IST/"+iidxId.toStdString());
                qDebug() << "LoadDirectory succeeded " << succeeded;
                UpdatePlayerList();

                mIsDownloadingIst = false;
                emit isDownloadingIstChanged();
            }
    );
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            process, &QProcess::deleteLater);

    QString command{"ist_scraper.exe"};
    QStringList args;

    if (runInPowerShell)
    {
        command = "powershell";
        args = QStringList{"-Command", "Start-Process -Wait ./ist_scraper.exe"};
    }

    process->start(command, args);
}

QString
Core::
setActiveVersion(const QString &iidxId,
                 const QString &activeVersionIndex)
{
    if (iidxId.isEmpty()||activeVersionIndex.isEmpty()) { return {}; }

    mCore.SetActiveVersionIndex(activeVersionIndex.toULongLong());
    mCore.Analyze(iidxId.toStdString());

    auto &dateTimeRange = score2dx::GetVersionDateTimeRange(activeVersionIndex.toULongLong());
    auto &begin = dateTimeRange.Get(ies::RangeSide::Begin);
    auto tokens = ies::SplitString(" ", begin);
    if (tokens.empty()) { return {}; }

    std::cout << std::flush;

    return tokens[0].c_str();
}

const score2dx::Core &
Core::
GetScore2dxCore()
const
{
    return mCore;
}

void
Core::
AnalyzeActivity(const std::string &iidxId,
                const std::string &beginDateTime,
                const std::string &endDateTime)
{
    mCore.AnalyzeActivity(iidxId, beginDateTime, endDateTime);
}

void
Core::
UpdatePlayerList()
{
    mPlayerList.clear();
    for (auto &[iidxId, playerScore] : mCore.GetPlayerScores())
    {
        mPlayerList.append(iidxId.c_str());
    }

    emit playerListChanged();
}

}
