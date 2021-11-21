#pragma once

#include <QObject>

#include "gui/Activity/ActivityListModel.hpp"
#include "gui/Core/Core.hpp"

namespace gui
{

class ActivityManager : public QObject
{
    Q_OBJECT

public:
        explicit ActivityManager(Core &core, QObject* parent=nullptr);

        Q_INVOKABLE
        void
        updateActivity(const QString &iidxId,
                       const QString &playStyleQStr,
                       const QString &date);

        ActivityListModel &
        GetActivityListModel()
        { return mActivityListModel; }

private:
    Core &mGuiCore;

    ActivityListModel mActivityListModel;
};

}
