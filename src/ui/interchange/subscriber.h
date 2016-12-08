#ifndef FUF_H
#define FUF_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QSharedPointer>

#include "interchange/broadcaster.h"

namespace interchange
{
typedef QSharedPointer<interchange::Broadcaster> QBroadcasterPtr;

class DataCallback
{
public:
    virtual ~DataCallback() {}
    virtual void setData(const interchange::ObjectList& list) = 0;
};

class ThreadWorker
        : public QThread
{
    Q_OBJECT

    QMutex m_lock;
    bool m_stopped;

    const interchange::ConsumerPtr m_pConsumer;
    DataCallback& m_callback;

public:
    explicit ThreadWorker(const interchange::ConsumerPtr& consumer,
                          DataCallback& callback);

    void run();
    void stop();

};

class Subscriber
        : public QObject
        , private DataCallback
{
    Q_OBJECT

    bool m_active;

    mutable QMutex m_lock;

    interchange::ObjectList m_data;

    const QBroadcasterPtr m_pBroadcaster;
    interchange::ConsumerPtr m_pConsumer;

    QSharedPointer<ThreadWorker> m_pWorker;

public:
    explicit Subscriber(const QBroadcasterPtr& pBroadcaster);
    ~Subscriber();

    interchange::ObjectList data() const;
    void setData(const interchange::ObjectList& data);

    bool activate();
    bool deactivate();

private:
    void startThread();
    void stopThread();

signals:
    void updated();

};
} // namespace interchange

#endif // FUF_H
