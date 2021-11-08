#include <QObject>
#include <QtCharts/QXYSeries>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>

#include "score2dx/Core/Core.hpp"

namespace gui
{

class Core : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList playerList READ getPlayerList NOTIFY playerListChanged)
    Q_PROPERTY(QStringList playStyleList READ getPlayStyleList CONSTANT)
    Q_PROPERTY(QStringList versionNameList READ getVersionNameList CONSTANT)
    Q_PROPERTY(bool isDownloadingIst MEMBER mIsDownloadingIst NOTIFY isDownloadingIstChanged)

public:
        explicit Core(QObject* parent=nullptr);

        Q_INVOKABLE
        QString
        getScore2dxVersion()
        const;

        Q_INVOKABLE
        bool
        addPlayer(const QString &iidxId);

        Q_INVOKABLE
        void
        loadDirectory(const QString &fileUrl);

        Q_INVOKABLE
        void
        downloadIst(const QString &iidxId,
                    const QString &versions,
                    const QString &styles,
                    bool runInPowerShell);

        const QStringList & getPlayerList() const { return mPlayerList; }
        const QStringList & getPlayStyleList() const { return mPlayStyleList; }
        const QStringList & getVersionNameList() const { return mVersionNameList; }

        const score2dx::Core &
        GetScore2dxCore()
        const;

        QStringListModel &
        GetDifficultyListModel()
        { return mDifficultyListModel; };

signals:
        void playerListChanged();
        void isDownloadingIstChanged();

private:
    score2dx::Core mCore;
    QStringList mPlayerList;
    QStringList mPlayStyleList;
    QStringListModel mDifficultyListModel;

    QStringList mVersionNameList;

    bool mIsDownloadingIst{false};

        void
        UpdateChart(const std::map<std::string, const score2dx::Csv*> &csvs);

        void
        UpdatePlayerList();
};

}
