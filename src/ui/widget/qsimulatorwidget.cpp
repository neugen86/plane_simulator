#include "qsimulatorwidget.h"

#include <QPainter>
#include <QResizeEvent>

namespace widget
{
QSimulatorWidget::QSimulatorWidget(const QContainerPtr& pContainer,
                                   const QBroadcasterPtr& pBroadcaster,
                                   QWidget* parent)
    : QWidget(parent)
    , m_subscriber(pBroadcaster)
    , m_controller(pContainer->width(), pContainer->height(), this)
{
    connect(&m_subscriber, SIGNAL(updated()), this, SLOT(update()));
    m_subscriber.activate();
}

void QSimulatorWidget::resizeEvent(QResizeEvent* event)
{
    m_controller.setCanvasSize(event->size());
}

void QSimulatorWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    m_controller.paint(painter);
}

void QSimulatorWidget::onUpdate()
{
    m_controller.setData(m_subscriber.data());
}
} // namespace widget
