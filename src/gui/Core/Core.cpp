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

#include "icl_s2/Common/IntegralRangeUsing.hpp"
#include "icl_s2/Common/SmartEnum.hxx"
#include "icl_s2/StdUtil/Find.hxx"
#include "icl_s2/String/SplitString.hpp"

#include "score2dx/Iidx/Definition.hpp"
#include "score2dx/Iidx/Version.hpp"

#include "gui/version.hpp"

namespace gui
{

ICL_S2_SMART_ENUM(CsvTableColumn,
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
    auto version =
        QString::number(SCORE_2DX_GUI_VERSION_MAJOR)
        +"."+QString::number(SCORE_2DX_GUI_VERSION_MINOR)
        +"."+QString::number(SCORE_2DX_GUI_VERSION_PATCH);
    return version;
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
    auto splitVerions = icl_s2::SplitString(", ", versionsStr);
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
    auto splitStyles = icl_s2::SplitString(", ", stylesStr);
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

void
Core::
setActiveVersion(const QString &iidxId,
                 const QString &activeVersionIndex)
{
    if (iidxId.isEmpty()||activeVersionIndex.isEmpty()) { return; }

    mCore.SetActiveVersionIndex(activeVersionIndex.toULongLong());
    mCore.Analyze(iidxId.toStdString());
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
