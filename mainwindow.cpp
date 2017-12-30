#include "mainwindow.h"
#include <QSettings>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QDockWidget>
#include <QtDebug>
#include "MonitorList/monitorlistwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_pCanva = new Canva(this);
    setCentralWidget(m_pCanva);
    setIconSize(QSize(64, 64));

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    setupInterface();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
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

    auto outputsDock = new QDockWidget(tr("Monitor list"),this);
    outputsDock->setObjectName("Monitor list dock");
    outputsDock->setVisible(monitorList->isChecked());
    addDockWidget(Qt::LeftDockWidgetArea, outputsDock);

    auto 监视器列表框架 = new MonitorListWindow;
    outputsDock->setWidget(监视器列表框架);

    connect(monitorList, &QAction::toggled, outputsDock, &QDockWidget::setVisible);
    connect(监视器列表框架, &MonitorListWindow::currentMonitorChanged, this, &MainWindow::setCurrentMonitor);
    setCurrentMonitor(监视器列表框架->currentMonitor());
}
