#ifndef QSIMULATORWIDGET_H
#define QSIMULATORWIDGET_H

#include <QWidget>

#include "interchange/qsubscriber.h"
#include "interchange/broadcaster.h"
#include "scene/interface/container.h"
#include "ui/controller/qpaintcontroller.h"

typedef QSharedPointer<scene::interface::Container> QContainerPtr;

namespace widget
{
class QSimulatorWidget
        : public QWidget
{
    Q_OBJECT

    interchange::QSubscriber m_subscriber;
    controller::QPaintController m_controller;

public:
    QSimulatorWidget(const QContainerPtr& pContainer,
                     const QBroadcasterPtr& pBroadcaster,
                     QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);

private slots:
    void onUpdate();

};
} // namespace widget

#endif // QSIMULATORWIDGET_H
