#ifndef QSimulatorController_H
#define QSimulatorController_H

#include <QMap>
#include <QSize>
#include <QMutex>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>

#include "interchange/subscription.h"
#include "scene/interface/controllable_container.h"

class QPainter;

typedef QSharedPointer<scene::interface::ControllableContainer> QControllableContainerPtr;

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

    const QControllableContainerPtr m_pContainer;

    QMap<types::obj_id, QPhysObjectPtr> m_objects;

    PhysObject* m_pSelectedObj;

public:
    QSimulatorController(const QControllableContainerPtr& pContainer,
                         QObject *parent = 0);

    void paint(QPainter& painter);

    void setCanvasSize(const QSize& size);
    void setData(const SubscriptionData& data);

    void setMousePos(const QPoint& point, bool fixed);

    void insertAt(const QPoint& point);
    void releaseSelected();
    void removeSelected();

    void clear();

};

#endif // QSimulatorController_H
