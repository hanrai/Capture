#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canva.h"
#include <QtDebug>

class QAction;
class QDockWidget;

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

private:
    Canva *m_pCanva;
    int m_monitorId;
};

#endif // MAINWINDOW_H
