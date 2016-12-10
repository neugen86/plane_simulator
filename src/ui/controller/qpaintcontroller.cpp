#include "qpaintcontroller.h"

#include <QPainter>

#include "physics/physics.h"

namespace controller
{

QPaintController::QPaintController(types::value_t sourceWidth, types::value_t sourceHeight, QObject* parent)
    : QObject(parent)
    , m_lambda(physics::constants::PositiveUnit)
    , m_rectSource(sourceWidth, sourceHeight)
{
}

void QPaintController::setCanvasSize(const QSize& size)
{
    const types::value_t sourceRatio = m_rectSource.ratio();
    const Rectangular rectCanvas(size.width(), size.height());

    const bool sourceRatioLessThanUnit =
            less(sourceRatio, physics::constants::PositiveUnit);
    const bool canvasRatioLessSourceRatio =
            less(rectCanvas.ratio(), sourceRatio);

    const bool byHeight = (sourceRatioLessThanUnit && canvasRatioLessSourceRatio) ||
            (!sourceRatioLessThanUnit && !canvasRatioLessSourceRatio);

    m_lambda = byHeight
            ? rectCanvas.height() / m_rectSource.height()
            : rectCanvas.width() / m_rectSource.width();

    m_rectProjection = Rectangular(m_rectSource.width() * m_lambda,
                                   m_rectSource.height() * m_lambda);

    m_offset.setX((size.width() - m_rectProjection.width()) / 2);
    m_offset.setY((size.height() - m_rectProjection.height()) / 2);
}

void QPaintController::setData(const interchange::ObjectList& list)
{
    QMutexLocker guard(&m_lock);

    foreach (const physics::ObjectPtr& pObj, list)
    {
        const types::obj_id id = pObj->id();
        auto existing = m_objects.find(id);

        if (existing != m_objects.end())
        {
            (*existing)->setPosition(pObj->position());
        }
        else
        {
            m_objects.insert(id, QPhysObjectPtr(new PhysObject(*pObj)));
        }
    }
}

void QPaintController::paint(QPainter& painter)
{
    painter.save();

    painter.scale(.99, .99);

    QPainterPath framePath;
    framePath.addRect(m_offset.x(), m_offset.y(),
                      m_rectProjection.width(), m_rectProjection.height());

    painter.setPen(QPen(QColor(0, 0, 0), 1, Qt::SolidLine,
                        Qt::FlatCap, Qt::MiterJoin));
    painter.setBrush(QColor(250, 250, 250));

    painter.drawPath(framePath);

    {
        QMutexLocker guard(&m_lock);

        foreach (QPhysObjectPtr pObj, m_objects.values())
        {
            const types::value_t radius = pObj->radius() * m_lambda;

            const QPointF pos(pObj->position().x() * m_lambda + m_offset.x(),
                              pObj->position().y() * m_lambda  + m_offset.y());

            painter.drawEllipse(pos, radius, radius);
        }
    }

    painter.restore();
}
} // namespace controller
