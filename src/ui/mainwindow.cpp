#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <widget/qsimulatorwidget.h>

static const double MsPerSecond = 1000;
static const unsigned int MaxFpsSteps = 6;
static const types::duration_t FpsStep = 50;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pScene(QSharedPointer<scene::Scene>(new scene::Scene))
{
    ui->setupUi(this);

    m_pSimulatorWidget.reset(new QSimulatorWidget(m_pScene, m_pScene));
    setCentralWidget(m_pSimulatorWidget.data());

    initMenu();

    onFaster();

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

    menu = ui->menuBar->addMenu(tr("FPS"));
    m_pFasterAction = menu->addAction(tr("Faster"), this, SLOT(onFaster()));
    m_pSlowerAction = menu->addAction(tr("Slower"), this, SLOT(onSlower()));

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
    static const QString ErrorValue(tr("ERROR"));
    static const QString StatusMessage(tr("Scene was %1, gravity is %2, " \
                                          "fps = %3, maximum fps = %4"));

    QString sceneState;
    switch (m_pScene->state())
    {
    case PlaybackState::STARTED:
        sceneState = tr("started");
        break;
    case PlaybackState::PAUSED:
        sceneState = tr("paused");
        break;
    case PlaybackState::STOPPED:
        sceneState = tr("stopped");
        break;
    default:
        sceneState = ErrorValue;
    }

    QString gravityValue;
    switch (m_pScene->gravityType())
    {
    case physics::Gravity::Type::Newtonian:
        gravityValue = tr("newtonian");
        break;
    case physics::Gravity::Type::Simple:
        gravityValue = tr("simple");
        break;
    case physics::Gravity::Type::None:
        gravityValue = tr("none");
        break;
    default:
        gravityValue = ErrorValue;
    }

    const types::duration_t subscriptionDuration = m_pSimulatorWidget->duration();

    const double desiredFrameRate = MsPerSecond /
            (subscriptionDuration > 0 ? subscriptionDuration : m_pScene->duration());

    const double realFrameRate = MsPerSecond / m_pScene->realDuration();

    ui->statusBar->showMessage(StatusMessage .arg(sceneState).arg(gravityValue)
                               .arg(QString::number(desiredFrameRate))
                               .arg(QString::number(realFrameRate)));
}

void MainWindow::updateFpsActions(types::duration_t duration)
{
    m_pFasterAction->setEnabled(duration > 0);
    m_pSlowerAction->setEnabled(duration < FpsStep * MaxFpsSteps);
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

void MainWindow::onFaster()
{
    const types::duration_t duration = m_pSimulatorWidget->duration() - FpsStep;
    m_pSimulatorWidget->setDuration(duration);

    updateFpsActions(duration);

    updateStatus();
}

void MainWindow::onSlower()
{
    const types::duration_t duration = m_pSimulatorWidget->duration() + FpsStep;
    m_pSimulatorWidget->setDuration(duration);

    updateFpsActions(duration);

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
