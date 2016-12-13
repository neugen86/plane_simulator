#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <scene/scene.h>

namespace Ui {
class MainWindow;
}

class QActionGroup;
class QSimulatorWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow* ui;

    QAction* m_pResetAction;

    QAction* m_pStartAction;
    QAction* m_pPauseAction;
    QAction* m_pStopAction;

    QAction* m_pFasterAction;
    QAction* m_pSlowerAction;

    QAction* m_pLightMassAction;
    QAction* m_pHeavyMassAction;

    QAction* m_pSmallRadiusAction;
    QAction* m_pBigRadiusAction;

    QAction* m_pNewtonianGravityAction;
    QAction* m_pSimpleGravityAction;
    QAction* m_pNoneGravityAction;

    QScopedPointer<QSimulatorWidget> m_pSimulatorWidget;

    QSharedPointer<scene::interface::Playable> m_pPlayable;
    QSharedPointer<scene::interface::WithGravity> m_pWithGravity;

    double m_maxFrameRate;
    double m_desiredFrameRate;

    QString m_strSceneState;
    QString m_strGravityValue;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initMenu();
    void initScene();
    void updateStatus();
    void updateFpsActions(types::duration_t duration);

private slots:
    void onReset();
    void onStart();
    void onPause();
    void onStop();
    void onFaster();
    void onSlower();
    void onGravityChanged(QAction* checkedAction);
    void onRadiusChanged(QAction* checkedAction);
    void onMassChanged(QAction* checkedAction);
    void onAbout();

};

#endif // MAINWINDOW_H
