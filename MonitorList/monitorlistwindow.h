#ifndef MONITORLISTWINDOW_H
#define MONITORLISTWINDOW_H

#include <QMainWindow>
#include <QImage>

class ListWidget;

class MonitorListWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MonitorListWindow(QWidget *parent = nullptr);

signals:

public slots:

private:
    void 初始化界面();
private slots:
    void resetSnapshots(bool);

private:
    ListWidget *m_屏幕截图视图;
};

#endif // MONITORLISTWINDOW_H
