#include "monitorlistwindow.h"
#include <QToolBar>
#include <QListWidget>
#include <QListWidgetItem>
#include "desktopduplication.h"
#include "listwidget.h"

MonitorListWindow::MonitorListWindow(QWidget *parent) : QMainWindow(parent)
{
    初始化界面();
    重设屏幕截图();
}

void MonitorListWindow::初始化界面()
{
    setIconSize(QSize(32, 32));

    QToolBar *监视器列表工具栏 = addToolBar(tr("Monitor list toolbar"));
    监视器列表工具栏->setObjectName("Monitor list toolbar");
    const QIcon 刷新图标 = QIcon(":/img/ico/Action-reload.ico");
    auto 刷新 = new QAction(刷新图标, tr("Rescan"), this);
    刷新->setStatusTip(tr("Show/Hide monitor list."));
    监视器列表工具栏->addAction(刷新);

    m_屏幕截图视图 = new ListWidget;
    m_屏幕截图视图->setViewMode(QListView::IconMode);
    m_屏幕截图视图->setMovement(QListView::Static);
    m_屏幕截图视图->setSpacing(12);
    m_屏幕截图视图->setResizeMode(QListView::Adjust);
    m_屏幕截图视图->setFlow(QListView::TopToBottom);
    m_屏幕截图视图->setWrapping(false);

    setCentralWidget(m_屏幕截图视图);
}

void MonitorListWindow::重设屏幕截图()
{
    m_屏幕截图视图->clear();
    DesktopDuplication duplication(this);
    int 屏幕数量 = duplication.getOutputCount();
    Q_ASSERT(屏幕数量);

    for(int i=0; i<屏幕数量; ++i)
    {
        QImage 截图 = duplication.takeSnapshot(i);
        Q_ASSERT(!截图.isNull());
        QListWidgetItem *截图项 = new QListWidgetItem(m_屏幕截图视图);
        截图项->setIcon(QIcon(QPixmap::fromImage(截图)));
        auto 描述 = duplication.getOutputDesc(i);
        截图项->setText(QString(tr("Display #%1 \n")).arg(i) +
                     QString(tr("%1, %2 %3x%4")).arg(描述.DesktopCoordinates.left)
                     .arg(描述.DesktopCoordinates.top)
                     .arg(描述.DesktopCoordinates.right - 描述.DesktopCoordinates.left)
                     .arg(描述.DesktopCoordinates.bottom - 描述.DesktopCoordinates.top));
        截图项->setTextAlignment(Qt::AlignHCenter);
        截图项->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    m_屏幕截图视图->setCurrentRow(0);
}
