#pragma once

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
    Q_PROPERTY(QStringList difficultyList READ getDifficultyList CONSTANT)
    Q_PROPERTY(QStringList versionNameList READ getVersionNameList CONSTANT)
    Q_PROPERTY(bool isDownloadingIst MEMBER mIsDownloadingIst NOTIFY isDownloadingIstChanged)
    Q_PROPERTY(bool isDownloadingMe MEMBER mIsDownloadingMe NOTIFY isDownloadingMeChanged)
    Q_PROPERTY(bool isChromeDriverReady MEMBER mIsChromeDriverReady NOTIFY isChromeDriverReadyChanged)
    Q_PROPERTY(bool hasError MEMBER mHasError NOTIFY hasErrorChanged)

public:
        explicit Core(QObject* parent=nullptr);

        Q_INVOKABLE
        QString
        getScore2dxVersion()
        const;

        Q_INVOKABLE
        QString
        getDbFilename()
        const;

        Q_INVOKABLE
        QString
        getChromeStatus()
        const;

        Q_INVOKABLE
        bool
        addPlayer(const QString &iidxId);

        Q_INVOKABLE
        void
        loadDirectory(const QString &fileUrl);

        Q_INVOKABLE
        QString
        findMeUserIidxId(const QString &inputMeUserName);

        Q_INVOKABLE
        void
        downloadMe(const QString &meUserName);

        Q_INVOKABLE
        void
        downloadIst(const QString &iidxId,
                    const QString &versions,
                    const QString &styles,
                    bool runInPowerShell);

    //! @brief Set active version and re-analyze ScoreAnalysis for player with iidxId.
    //! @return version begin date.
        Q_INVOKABLE
        QString
        setActiveVersion(const QString &iidxId,
                         const QString &activeVersionIndex);

        Q_INVOKABLE
        QString
        getErrorMessage()
        const;

        Q_INVOKABLE
        void
        clearError();

        const QStringList & getPlayerList() const { return mPlayerList; }
        const QStringList & getPlayStyleList() const { return mPlayStyleList; }
        const QStringList & getDifficultyList() const { return mDifficultyList; }
        const QStringList & getVersionNameList() const { return mVersionNameList; }

        const score2dx::Core &
        GetScore2dxCore()
        const;

        void
        AnalyzeActivity(const std::string &iidxId,
                        const std::string &beginDateTime,
                        const std::string &endDateTime);

signals:
        void playerListChanged();
        void isDownloadingIstChanged();
        void isDownloadingMeChanged();
        void isChromeDriverReadyChanged();
        void hasErrorChanged();

private:
    score2dx::Core mCore;
    QStringList mPlayerList;
    QStringList mPlayStyleList;
    QStringList mDifficultyList;
    QStringList mVersionNameList;

    bool mIsDownloadingIst{false};
    bool mIsDownloadingMe{false};
    bool mIsChromeDriverReady{false};
    bool mHasError{false};

    QString mChromeStatus;
    QString mErrorMessage;

        void
        UpdateChart(const std::map<std::string, const score2dx::Csv*> &csvs);

        void
        UpdatePlayerList();
};

}
