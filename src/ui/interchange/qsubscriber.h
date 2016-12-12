#ifndef QSUBSCRIBER_H
#define QSUBSCRIBER_H

#include <QObject>
#include <QThread>
#include <QSharedPointer>

#include "concurrent/lock.h"
#include "interchange/broadcaster.h"

typedef QSharedPointer<interchange::Broadcaster> QBroadcasterPtr;

namespace interchange
{
class DataCallback
{
public:
    virtual ~DataCallback() {}
    virtual void setData(const interchange::ObjectList& list) = 0;
};

class QThreadWorker
        : public QThread
{
    Q_OBJECT

    bool m_stopped;
    concurrent::spinlock m_lock;

    const interchange::ConsumerPtr m_pConsumer;
    DataCallback& m_callback;

public:
    explicit QThreadWorker(const interchange::ConsumerPtr& consumer,
                           DataCallback& callback);

    void run();
    void stop();

};

class QSubscriber
        : public QObject
        , private DataCallback
{
    Q_OBJECT

    bool m_active;

    interchange::ObjectList m_data;

    mutable concurrent::spinlock m_lock;

    const QBroadcasterPtr m_pBroadcaster;
    interchange::ConsumerPtr m_pConsumer;

    QSharedPointer<QThreadWorker> m_pWorker;

public:
    explicit QSubscriber(const QBroadcasterPtr& pBroadcaster);
    ~QSubscriber();

    interchange::ObjectList data() const;
    void setData(const interchange::ObjectList& data);

    void setDuration(types::duration_t duration);
    types::duration_t duration() const;

    bool activate();
    bool deactivate();

private:
    void startThread();
    void stopThread();

signals:
    void updated();

};
} // namespace interchange

#endif // QSUBSCRIBER_H
