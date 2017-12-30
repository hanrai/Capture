#ifndef MONITORLISTWINDOW_H
#define MONITORLISTWINDOW_H

#include <QMainWindow>
#include <QImage>

class ListWidget;

class MonitorListWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(int currentMonitor READ currentMonitor WRITE setCurrentMonitor NOTIFY currentMonitorChanged)
public:
    explicit MonitorListWindow(QWidget *parent = nullptr);

    int currentMonitor() const
    {
        return m_currentMonitor;
    }

signals:
    void currentMonitorChanged(int id);

public slots:

    void setCurrentMonitor(int currentMonitor)
    {
        if (m_currentMonitor == currentMonitor)
            return;

        m_currentMonitor = currentMonitor;
        emit currentMonitorChanged(m_currentMonitor);
    }

private:
    void 初始化界面();
private slots:
    void resetScreenList(void);

private:
    ListWidget *m_屏幕截图视图;
    int m_currentMonitor;
};

#endif // MONITORLISTWINDOW_H
