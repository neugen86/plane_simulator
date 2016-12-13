#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <widget/qsimulator_widget.h>

namespace values
{
static const double MsPerSecond = 1000;
static const types::duration_t FpsStep = 75;
static const types::duration_t MaxFpsStep = FpsStep * 6;
} // namespace values

static const QString AboutText =
        MainWindow::tr("Motion balls simulation.\n" \
                       "Evgeny Neustroev <neugen86@gmail.com>");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_maxFrameRate(0)
    , m_desiredFrameRate(0)
{
    ui->setupUi(this);

    initMenu();

    initScene();

    onReset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenu()
{
    QMenu* menu(nullptr);
    QActionGroup* group(nullptr);

    menu = ui->menuBar->addMenu(tr("Scene"));
    m_pResetAction = menu->addAction(tr("Reset"), this, SLOT(onReset()));

    menu = ui->menuBar->addMenu(tr("Playback"));
    m_pStartAction = menu->addAction(tr("Start"), this, SLOT(onStart()));
    m_pPauseAction = menu->addAction(tr("Pause"), this, SLOT(onPause()));
    m_pStopAction = menu->addAction(tr("Stop"), this, SLOT(onStop()));

    menu = ui->menuBar->addMenu(tr("FPS"));
    m_pFasterAction = menu->addAction(tr("Faster"), this, SLOT(onFaster()));
    m_pSlowerAction = menu->addAction(tr("Slower"), this, SLOT(onSlower()));

    group = new QActionGroup(this);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(onGravityChanged(QAction*)));
    m_pNewtonianGravityAction = group->addAction(tr("Newtonian"));
    m_pNewtonianGravityAction->setCheckable(true);
    m_pSimpleGravityAction = group->addAction(tr("Simple"));
    m_pSimpleGravityAction->setCheckable(true);
    m_pNoneGravityAction = group->addAction(tr("None"));
    m_pNoneGravityAction->setCheckable(true);
    menu = ui->menuBar->addMenu(tr("Gravity"));
    menu->addActions(group->actions());

    group = new QActionGroup(this);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(onRadiusChanged(QAction*)));
    m_pSmallRadiusAction = group->addAction(tr("Small"));
    m_pSmallRadiusAction->setCheckable(true);
    m_pBigRadiusAction = group->addAction(tr("Big"));
    m_pBigRadiusAction->setCheckable(true);
    menu = ui->menuBar->addMenu(tr("Radius"));
    menu->addActions(group->actions());

    group = new QActionGroup(this);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(onMassChanged(QAction*)));
    m_pLightMassAction = group->addAction(tr("Light"));
    m_pLightMassAction->setCheckable(true);
    m_pHeavyMassAction = group->addAction(tr("Heavy"));
    m_pHeavyMassAction->setCheckable(true);
    menu = ui->menuBar->addMenu(tr("Mass"));
    menu->addActions(group->actions());

    menu = ui->menuBar->addMenu(tr("Help"));
    menu->addAction(tr("About..."), this, SLOT(onAbout()));
}

void MainWindow::initScene()
{
    QSharedPointer<scene::Scene> pScene(new scene::Scene);
    m_pWithGravity = pScene;
    m_pPlayable = pScene;

    m_pSimulatorWidget.reset(new QSimulatorWidget(pScene, pScene));
    setCentralWidget(m_pSimulatorWidget.data());
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

void MainWindow::updateFps(types::duration_t duration)
{
    m_pSimulatorWidget->setDuration(duration);

    m_pFasterAction->setEnabled(duration > 0);
    m_pSlowerAction->setEnabled(duration < values::MaxFpsStep);

    const types::duration_t max = m_pPlayable->realDuration();

    m_desiredFrameRate = values::MsPerSecond / std::max(duration, max);
    m_maxFrameRate = values::MsPerSecond / max;

    updateStatus();
}

void MainWindow::onReset()
{
    onStop();

    m_pSimulatorWidget->clear();

    m_pSimpleGravityAction->setChecked(true);
    onGravityChanged(nullptr);

    m_pSmallRadiusAction->setChecked(true);
    onRadiusChanged(nullptr);

    m_pLightMassAction->setChecked(true);
    onMassChanged(nullptr);

    updateFps(0);

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
    updateFps(m_pSimulatorWidget->duration() - values::FpsStep);
}

void MainWindow::onSlower()
{
    updateFps(m_pSimulatorWidget->duration() + values::FpsStep);
}

void MainWindow::onGravityChanged(QAction*)
{
    typedef physics::Gravity::Type GravityType;

    if (m_pNewtonianGravityAction->isChecked())
    {
        m_pWithGravity->setGravityType(GravityType::Newtonian);
    }
    else if (m_pSimpleGravityAction->isChecked())
    {
        m_pWithGravity->setGravityType(GravityType::Simple);
    }
    else if (m_pNoneGravityAction->isChecked())
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

void MainWindow::onRadiusChanged(QAction*)
{
    if (m_pSmallRadiusAction->isChecked())
    {
        m_pSimulatorWidget->setBodyRadius(BodyRadius::Small);
    }
    else if (m_pBigRadiusAction->isChecked())
    {
        m_pSimulatorWidget->setBodyRadius(BodyRadius::Big);
    }
}

void MainWindow::onMassChanged(QAction*)
{
    if (m_pLightMassAction->isChecked())
    {
        m_pSimulatorWidget->setBodyMass(BodyMass::Light);
    }
    else if (m_pHeavyMassAction->isChecked())
    {
        m_pSimulatorWidget->setBodyMass(BodyMass::Heavy);
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("About"), AboutText);
}
