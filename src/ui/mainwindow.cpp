#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <widget/qsimulator_widget.h>

static const double MsPerSecond = 1000;
static const unsigned int MaxFpsSteps = 6;
static const types::duration_t FpsStep = 75;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_maxFrameRate(0)
    , m_desiredFrameRate(0)
{
    ui->setupUi(this);

    initMenu();

    initScene();

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

void MainWindow::initScene()
{
    QSharedPointer<scene::Scene> pScene(new scene::Scene);

    m_pSimulatorWidget.reset(new QSimulatorWidget(pScene, pScene));
    setCentralWidget(m_pSimulatorWidget.data());

    m_pWithGravity = pScene;
    m_pPlayable = pScene;
}

void MainWindow::updateStatus()
{
    static const QString StatusMessage(tr("Scene was %1, gravity is %2, " \
                                          "fps = %3, maximum fps = %4"));

    ui->statusBar->showMessage(StatusMessage
                               .arg(m_strSceneState).arg(m_strGravityValue)
                               .arg(QString::number(m_desiredFrameRate))
                               .arg(QString::number(m_maxFrameRate)));
}

void MainWindow::updateFpsActions(types::duration_t duration)
{
    m_pFasterAction->setEnabled(duration > 0);
    m_pSlowerAction->setEnabled(duration < FpsStep * MaxFpsSteps);

    const types::duration_t max = m_pPlayable->duration();

    m_desiredFrameRate = MsPerSecond / std::max(duration, max);
    m_maxFrameRate = MsPerSecond / max;

    updateStatus();
}

void MainWindow::onReset()
{
    onStop();

    m_pSimulatorWidget->clear();

    switch (m_pWithGravity->gravityType())
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

    onStart();
}

void MainWindow::onStart()
{
    m_pStartAction->setEnabled(false);
    m_pPauseAction->setEnabled(true);
    m_pStopAction->setEnabled(true);

    if (m_pPlayable->start())
        m_strSceneState = tr("started");

    updateStatus();
}

void MainWindow::onPause()
{
    m_pStartAction->setEnabled(true);
    m_pPauseAction->setEnabled(false);
    m_pStopAction->setEnabled(true);

    if (m_pPlayable->pause())
        m_strSceneState = tr("paused");

    updateStatus();
}

void MainWindow::onStop()
{
    m_pStartAction->setEnabled(true);
    m_pPauseAction->setEnabled(false);
    m_pStopAction->setEnabled(false);

    if (m_pPlayable->stop())
        m_strSceneState = tr("stopped");

    updateStatus();
}

void MainWindow::onFaster()
{
    const types::duration_t duration = m_pSimulatorWidget->duration() - FpsStep;
    m_pSimulatorWidget->setDuration(duration);

    updateFpsActions(duration);
}

void MainWindow::onSlower()
{
    const types::duration_t duration = m_pSimulatorWidget->duration() + FpsStep;
    m_pSimulatorWidget->setDuration(duration);

    updateFpsActions(duration);
}

void MainWindow::onGravityChanged(QAction* checkedAction)
{
    typedef physics::Gravity::Type GravityType;

    if (m_pNewtonianGravityAction == checkedAction)
    {
        m_pWithGravity->setGravityType(GravityType::Newtonian);
    }
    else if (m_pSimpleGravityAction == checkedAction)
    {
        m_pWithGravity->setGravityType(GravityType::Simple);
    }
    else if (m_pNoneGravityAction == checkedAction)
    {
        m_pWithGravity->setGravityType(GravityType::None);
    }

    switch (m_pWithGravity->gravityType())
    {
    case physics::Gravity::Type::Newtonian:
        m_strGravityValue = tr("newtonian");
        break;
    case physics::Gravity::Type::Simple:
        m_strGravityValue = tr("simple");
        break;
    case physics::Gravity::Type::None:
        m_strGravityValue = tr("none");
        break;
    default:
        m_strGravityValue = tr("unknown");
    }

    updateStatus();
}

void MainWindow::onAbout()
{

}
