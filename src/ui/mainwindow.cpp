#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdexcept>
#include <widget/qsimulatorwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pScene(QSharedPointer<scene::Scene>(new scene::Scene))
{
    ui->setupUi(this);

    m_pSimulatorWidget.reset(new widget::QSimulatorWidget(m_pScene, m_pScene));
    setCentralWidget(m_pSimulatorWidget.data());

    initMenu();

    onReset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenu()
{
    QMenu* menu(nullptr);

    menu = ui->menuBar->addMenu(tr("Scene"));
    m_pResetAction = menu->addAction(tr("Reset"), this, SLOT(onReset()));

    menu = ui->menuBar->addMenu(tr("Playback"));
    m_pStartAction = menu->addAction(tr("Start"), this, SLOT(onStart()));
    m_pPauseAction = menu->addAction(tr("Pause"), this, SLOT(onPause()));
    m_pStopAction = menu->addAction(tr("Stop"), this, SLOT(onStop()));

    m_pGravityActions = new QActionGroup(this);
    connect(m_pGravityActions, SIGNAL(triggered(QAction*)),
            this, SLOT(onGravityChanged(QAction*)));

    m_pNewtonianGravityAction = m_pGravityActions->addAction(tr("Newtonian"));
    m_pNewtonianGravityAction->setCheckable(true);
    m_pSimpleGravityAction = m_pGravityActions->addAction(tr("Simple"));
    m_pSimpleGravityAction->setCheckable(true);
    m_pNoneGravityAction = m_pGravityActions->addAction(tr("None"));
    m_pNoneGravityAction->setCheckable(true);

    menu = ui->menuBar->addMenu(tr("Gravity"));
    menu->addActions(m_pGravityActions->actions());

    menu = ui->menuBar->addMenu(tr("Help"));
    menu->addAction(tr("About..."), this, SLOT(onAbout()));
}

void MainWindow::updateStatus()
{
    static const QString UnknownValue(tr("UNKNOWN"));
    static const QString StatusMessage(tr("Scene state: %1, gravity: %2."));

    QString sceneState;
    switch (m_pScene->state())
    {
    case PlaybackState::STARTED:
        sceneState = tr("STARTED");
        break;
    case PlaybackState::PAUSED:
        sceneState = tr("PAUSED");
        break;
    case PlaybackState::STOPPED:
        sceneState = tr("STOPPED");
        break;
    default:
        sceneState = UnknownValue;
    }

    QString gravityValue;
    switch (m_pScene->gravityType())
    {
    case physics::Gravity::Type::Newtonian:
        gravityValue = tr("NEWTONIAN");
        break;
    case physics::Gravity::Type::Simple:
        gravityValue = tr("SIMPLE");
        break;
    case physics::Gravity::Type::None:
        gravityValue = tr("NONE");
        break;
    default:
        gravityValue = UnknownValue;
    }

    ui->statusBar->showMessage(StatusMessage
                               .arg(sceneState)
                               .arg(gravityValue));
}

void MainWindow::onReset()
{
    onStop();

    m_pScene->removeAll();

    m_pScene->insertObject(physics::Object(algebra::Point(256., 192.), 5.));
    m_pScene->insertObject(physics::Object(algebra::Point(512., 567.), 7.5));
    m_pScene->insertObject(physics::Object(algebra::Point(768., 384.), 10.));

    switch (m_pScene->gravityType())
    {
    case physics::Gravity::Type::Newtonian:
        m_pNewtonianGravityAction->setChecked(true);
        break;
    case physics::Gravity::Type::Simple:
        m_pSimpleGravityAction->setChecked(true);
        break;
    default:
        m_pNoneGravityAction->setChecked(true);
    }
}

void MainWindow::onStart()
{
    m_pStartAction->setEnabled(false);
    m_pPauseAction->setEnabled(true);
    m_pStopAction->setEnabled(true);

    m_pScene->start();

    updateStatus();
}

void MainWindow::onPause()
{
    m_pStartAction->setEnabled(true);
    m_pPauseAction->setEnabled(false);
    m_pStopAction->setEnabled(true);

    m_pScene->pause();

    updateStatus();
}

void MainWindow::onStop()
{
    m_pStartAction->setEnabled(true);
    m_pPauseAction->setEnabled(false);
    m_pStopAction->setEnabled(false);

    m_pScene->stop();

    updateStatus();
}

void MainWindow::onGravityChanged(QAction* checkedAction)
{
    typedef physics::Gravity::Type GravityType;

    if (m_pNewtonianGravityAction == checkedAction)
    {
        m_pScene->setGravityType(GravityType::Newtonian);
    }
    else if (m_pSimpleGravityAction == checkedAction)
    {
        m_pScene->setGravityType(GravityType::Simple);
    }
    else if (m_pNoneGravityAction == checkedAction)
    {
        m_pScene->setGravityType(GravityType::None);
    }

    updateStatus();
}

void MainWindow::onAbout()
{

}
