#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canva.h"

class QAction;
class QDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void closeEvent(QCloseEvent *event);

private:
    void setupInterface();

private:
    Canva *m_pCanva;
};

#endif // MAINWINDOW_H
