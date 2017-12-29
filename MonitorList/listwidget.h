#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // LISTWIDGET_H
