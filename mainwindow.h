#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>

class QAction;
class QDockWidget;
class Canva;
class MainFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void setCurrentMonitor(int id) {m_monitorId = id;}

protected:
    void closeEvent(QCloseEvent *event);

private:
    void setupInterface();
    void setupMachine();
    void saveStage();
    void restoreStage();

private:
    MainFrame *m_machine;
    Canva *m_pCanva;
    int m_monitorId;
};

#endif // MAINWINDOW_H
