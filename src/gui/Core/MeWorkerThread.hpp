#pragma once

#include <vector>

#include <QThread>

#include "score2dx/Core/Core.hpp"

namespace gui
{

class MeWorkerThread : public QThread
{
    Q_OBJECT

public:
        MeWorkerThread(score2dx::Core &core,
                       const QString &user,
                       QObject* parent=nullptr);

        void
        run()
        override;

signals:
    void ResultReady(const QString &errorMessage);

private:
    score2dx::Core &mCore;
    QString mUser;
};

}
