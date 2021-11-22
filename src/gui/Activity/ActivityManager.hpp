#pragma once

#include <QObject>

#include "gui/Activity/ActivityListModel.hpp"
#include "gui/Core/Core.hpp"

namespace gui
{

class ActivityManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString activityPlayStyle READ getActivityPlayStyle NOTIFY activityPlayStyleChanged)
    Q_PROPERTY(QString activityDate READ getActivityDate NOTIFY activityDateChanged)

public:
        explicit ActivityManager(Core &core, QObject* parent=nullptr);

        Q_INVOKABLE
        void
        updateActivity(const QString &iidxId,
                       const QString &playStyleQStr,
                       const QString &date);

        const QString & getActivityPlayStyle() const { return mActivityPlayStyle; }
        const QString & getActivityDate() const { return mActivityDate; }

        ActivityListModel &
        GetActivityListModel()
        { return mActivityListModel; }

signals:
        void activityPlayStyleChanged();
        void activityDateChanged();

private:
    Core &mGuiCore;

    ActivityListModel mActivityListModel;
    //! @brief Activity play style, may not be same as current selected play style.
    QString mActivityPlayStyle{ToString(score2dx::PlayStyle::SinglePlay).c_str()};
    QString mActivityDate;
};

}
