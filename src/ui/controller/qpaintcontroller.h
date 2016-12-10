#ifndef QPAINTCONTROLLER_H
#define QPAINTCONTROLLER_H

#include <QMap>
#include <QSize>
#include <QMutex>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>

#include "interchange/subscription.h"

class QPainter;

namespace controller
{
class QPaintController
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

    QMutex m_lock;

    QPointF m_offset;

    types::value_t m_lambda;

    Rectangular m_rectProjection;
    const Rectangular m_rectSource;

    QMap<types::obj_id, QPhysObjectPtr> m_objects;

public:
    QPaintController(types::value_t sourceWidth,
                     types::value_t sourceHeight,
                     QObject *parent = 0);

    void setCanvasSize(const QSize& size);
    void setData(const interchange::ObjectList& list);
    void paint(QPainter& painter);

private:

};
} // namespace controller

#endif // QPAINTCONTROLLER_H
