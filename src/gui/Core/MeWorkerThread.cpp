#include "gui/Core/MeWorkerThread.hpp"

namespace gui
{

MeWorkerThread::
MeWorkerThread(score2dx::Core &core,
               const QString &user,
               QObject* parent)
:   QThread(parent),
    mCore(core),
    mUser(user)
{
}

void
MeWorkerThread::
run()
{
    QString errorMessage;

    try
    {
        auto iidxId = mCore.AddIidxMeUser(mUser.toStdString());
        mCore.ExportIidxMeData(mUser.toStdString());
        mCore.LoadDirectory("ME/"+iidxId);
    }
    catch (const std::exception &e)
    {
        errorMessage = e.what();
    }

    emit ResultReady(errorMessage);
}

}
