#include "qsimulatorwidget.h"

#include <QPainter>
#include <QResizeEvent>

QSimulatorWidget::QSimulatorWidget(const QContainerPtr& pContainer,
                                   const QBroadcasterPtr& pBroadcaster,
                                   QWidget* parent)
    : QWidget(parent)
    , m_subscriber(pBroadcaster)
    , m_controller(pContainer->width(), pContainer->height(), this)
{
    connect(&m_subscriber, SIGNAL(updated()), this, SLOT(onUpdate()));
    m_subscriber.activate();
}

void QSimulatorWidget::setDuration(types::duration_t duration)
{
    m_subscriber.setDuration(duration);
}

types::duration_t QSimulatorWidget::duration() const
{
    return m_subscriber.duration();
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
    update();
}
