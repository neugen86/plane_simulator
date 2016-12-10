#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <scene/scene.h>
#include <widget/qsimulatorwidget.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSharedPointer<scene::Scene> pScene = QSharedPointer<scene::Scene>(new scene::Scene);

    widget::QSimulatorWidget* pWdg = new widget::QSimulatorWidget(pScene, pScene, this);
    setCentralWidget(pWdg);

    pScene->start();

    pScene->insertObject(physics::Object(algebra::Point(256., 192.), 5.));
    pScene->insertObject(physics::Object(algebra::Point(512., 567.), 7.5));
    pScene->insertObject(physics::Object(algebra::Point(768., 384.), 10.));
}

MainWindow::~MainWindow()
{
    delete ui;
}
