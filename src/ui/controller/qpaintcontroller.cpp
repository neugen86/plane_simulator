#include "qpaintcontroller.h"

#include <QPainter>

namespace controller
{
QPaintController::QPaintController(double sourceWidth, double sourceHeight, QObject* parent)
    : QObject(parent)
    , m_rectSource(sourceWidth, sourceHeight)
{
}

void QPaintController::setCanvasSize(const QSize& size)
{
}

void QPaintController::setData(const interchange::ObjectList& list)
{
}

void QPaintController::paint(QPainter& painter)
{
}
} // namespace controller
