#include "qsubscriber.h"

#include <QDebug>
#include <QMutexLocker>

#include "physics/physics.h"

namespace interchange
{
QThreadWorker::QThreadWorker(const interchange::ConsumerPtr& consumer, DataCallback& callback)
    : QThread()
    , m_stopped(false)
    , m_lock()
    , m_pConsumer(consumer)
    , m_callback(callback)
{
}

void QThreadWorker::run()
{
    for (;;)
    {
        {
            concurrent::guard guard(m_lock);
            if (m_stopped)
                break;
        }

        m_callback.setData(m_pConsumer->get());

        if (!m_pConsumer->active())
            break;
    }
}

void QThreadWorker::stop()
{
    concurrent::guard guard(m_lock);
    m_stopped = true;
}

QSubscriber::QSubscriber(const QBroadcasterPtr& pBroadcaster)
    : QObject()
    , DataCallback()
    , m_active(false)
    , m_lock()
    , m_pBroadcaster(pBroadcaster)
    , m_pWorker(nullptr)
{
}

QSubscriber::~QSubscriber()
{
    deactivate();
}

SubscriptionData QSubscriber::data() const
{
    concurrent::guard guard(m_lock);
    return m_data;
}

void QSubscriber::setData(const SubscriptionData& data)
{
    {
        concurrent::guard guard(m_lock);
        m_data = data;
    }

    emit updated();
}

void QSubscriber::setDuration(types::duration_t duration)
{
    m_pConsumer->setDuration(duration);
}

types::duration_t QSubscriber::duration() const
{
    return m_pConsumer->duration();
}

bool QSubscriber::activate()
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

bool QSubscriber::deactivate()
{
    if (!m_active)
        return false;

    m_pBroadcaster->unsubscribe(m_pConsumer);

    stopThread();

    m_pConsumer.reset();

    m_active = false;

    return true;
}

void QSubscriber::startThread()
{
    static DataCallback& callback = static_cast<DataCallback&>(*this);

    m_pWorker = QSharedPointer<QThreadWorker>(new QThreadWorker(m_pConsumer, callback));
    m_pWorker->start();
}

void QSubscriber::stopThread()
{
    m_pWorker->stop();
    m_pWorker->wait();
    m_pWorker.reset();
}
} // namespace interchange
