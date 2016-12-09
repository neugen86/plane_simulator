#include "subscriber.h"

#include <QDebug>
#include <QMutexLocker>

#include "physics/physics.h"

namespace interchange
{
ThreadWorker::ThreadWorker(const interchange::ConsumerPtr& consumer, DataCallback& callback)
    : QThread()
    , m_stopped(false)
    , m_lock()
    , m_pConsumer(consumer)
    , m_callback(callback)
{
}

void ThreadWorker::run()
{
    for (;;)
    {
        {
            concurrent::guard guard(m_lock);
            if (m_stopped) break;
        }

        m_callback.setData(m_pConsumer->get());
    }
}

void ThreadWorker::stop()
{
    concurrent::guard guard(m_lock);
    m_stopped = true;
}

Subscriber::Subscriber(const QBroadcasterPtr& pBroadcaster)
    : QObject()
    , DataCallback()
    , m_active(false)
    , m_lock()
    , m_pBroadcaster(pBroadcaster)
    , m_pWorker(nullptr)
{
}

Subscriber::~Subscriber()
{
    deactivate();
}

interchange::ObjectList Subscriber::data() const
{
    concurrent::guard guard(m_lock);
    return m_data;
}

void Subscriber::setData(const interchange::ObjectList& data)
{
    for (const physics::ObjectPtr& pObj : data)
    {
        if (pObj->id() == 0)
        {
            qDebug() << "id =" << pObj->id() << "\tpos = ("
                     << pObj->position().x() << ","
                     << pObj->position().y() << ")";
        }
    }

    {
        concurrent::guard guard(m_lock);
        m_data = data;
    }

    emit updated();
}

bool Subscriber::activate()
{
    if (m_active)
        return false;

    m_pConsumer = m_pBroadcaster->subscribe();

    if (!m_pConsumer)
        return false;

    startThread();

    m_active = true;

    return true;
}

bool Subscriber::deactivate()
{
    if (!m_active)
        return false;

    stopThread();

    m_pBroadcaster->unsubscribe(m_pConsumer);
    m_pConsumer.reset();

    m_active = false;

    return true;
}

void Subscriber::startThread()
{
    static DataCallback& callback = static_cast<DataCallback&>(*this);

    m_pWorker = QSharedPointer<ThreadWorker>(new ThreadWorker(m_pConsumer, callback));
    m_pWorker->start();
}

void Subscriber::stopThread()
{
    m_pWorker->stop();
    m_pWorker->wait();
    m_pWorker.reset();
}
} // namespace interchange
