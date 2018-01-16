#include "mainwindow.h"
#include <QSettings>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QDockWidget>
#include <QtDebug>
#include <QStatusBar>
#include "MonitorList/monitorlistwindow.h"
#include "Workspace/canva.h"
#include "Workspace/canvastatus.h"
#include "MainFrame.h"
#include "desktopduplication.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_machine(new MainFrame(this)),
    m_canva(new Canva(this)),
    m_canvaStatus(new CanvaStatus(m_canva))
{
    setCentralWidget(m_canva);
    setIconSize(QSize(64, 64));

    restoreStage();

    setupInterface();
    setupMachine();

    m_machine->start();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveStage();
    QMainWindow::closeEvent(event);
}

void MainWindow::setupInterface()
{
    QToolBar *captureToolBar = addToolBar(tr("Capture"));
    captureToolBar->setObjectName("Capture Toolbar");

    const QIcon newIcon = QIcon(":/img/ico/App-desktop-share.ico");
    auto monitorList = new QAction(newIcon, tr("Show monitors"), this);
    monitorList->setStatusTip(tr("Show/Hide monitor list."));
    monitorList->setCheckable(true);
    captureToolBar->addAction(monitorList);

    const QIcon 截屏图标 = QIcon(":/img/ico/App-snapshot.ico");
    auto 截屏 = new QAction(截屏图标, tr("Capture screen"), this);
    截屏->setStatusTip(tr("Capture snapshot for current monitor."));
    connect(截屏, &QAction::triggered, [=](bool){
        m_machine->submitEvent("CaptureButton.Clicked");
    });
    captureToolBar->addAction(截屏);

    auto outputsDock = new QDockWidget(tr("Monitor list"),this);
    outputsDock->setObjectName("Monitor list dock");
    outputsDock->setVisible(monitorList->isChecked());
    outputsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    outputsDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::LeftDockWidgetArea, outputsDock);

    auto 监视器列表框架 = new MonitorListWindow;
    outputsDock->setWidget(监视器列表框架);

    connect(monitorList, &QAction::toggled, outputsDock, &QDockWidget::setVisible);
    connect(监视器列表框架, &MonitorListWindow::currentMonitorChanged, this, &MainWindow::setCurrentMonitor);
    setCurrentMonitor(监视器列表框架->currentMonitor());

    auto stat = statusBar();
    stat->addWidget(m_canvaStatus);
}

void MainWindow::setupMachine()
{
    m_machine->connectToState("Capture", [this](const bool actived){setVisible(!actived);});

    m_machine->connectToEvent("TakeSnapshot", this, [this](const QScxmlEvent &){
        DesktopDuplication duplication;
        m_canva->setImage(duplication.takeSnapshot(m_monitorId));
    });
}

void MainWindow::saveStage()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void MainWindow::restoreStage()
{
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}
