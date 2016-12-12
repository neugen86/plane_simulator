#ifndef QSimulatorController_H
#define QSimulatorController_H

#include <QMap>
#include <QSize>
#include <QMutex>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>

#include "interchange/subscription.h"
#include "scene/interface/container.h"

class QPainter;

typedef QSharedPointer<scene::interface::Container> QContainerPtr;

class QSimulatorController
        : public QObject
{
    Q_OBJECT

    typedef physics::Object PhysObject;
    typedef QSharedPointer<PhysObject> QPhysObjectPtr;

    class Rectangular
    {
        types::value_t m_ratio;
        types::value_t m_width;
        types::value_t m_height;

    public:
        explicit Rectangular(types::value_t width = 0.,
                             types::value_t height = 0.)
            : m_ratio(width / height)
            , m_width(width), m_height(height) {}
        types::value_t ratio() const { return m_ratio; }
        types::value_t width() const { return m_width; }
        types::value_t height() const { return m_height; }
    };

    QPoint m_pos;

    QMutex m_lock;

    QPointF m_offset;

    types::value_t m_lambda;

    Rectangular m_rectProjection;
    const Rectangular m_rectSource;

    const QContainerPtr m_pContainer;

    QMap<types::obj_id, QPhysObjectPtr> m_objects;

    PhysObject* m_pSelectedObj;

public:
    QSimulatorController(const QContainerPtr& pContainer,
                         QObject *parent = 0);

    void insertAt(const QPoint& point);
    void removeSelected();

    void setMousePos(const QPoint& pos) { m_pos = pos; }

    void setCanvasSize(const QSize& size);

    void setData(const SubscriptionData& data);

    void paint(QPainter& painter);

    void clear();

};

#endif // QSimulatorController_H
