#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <scene/scene.h>

namespace Ui {
class MainWindow;
}

class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow* ui;

    QAction* m_pResetAction;

    QAction* m_pStartAction;
    QAction* m_pPauseAction;
    QAction* m_pStopAction;

    QActionGroup* m_pGravityActions;
    QAction* m_pNewtonianGravityAction;
    QAction* m_pSimpleGravityAction;
    QAction* m_pNoneGravityAction;

    QSharedPointer<scene::Scene> m_pScene;

    QScopedPointer<QWidget> m_pSimulatorWidget;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initMenu();
    void updateStatus();

private slots:
    void onReset();
    void onStart();
    void onPause();
    void onStop();
    void onGravityChanged(QAction* checkedAction);
    void onAbout();

};

#endif // MAINWINDOW_H
