#include "listwidget.h"
#include <QResizeEvent>

ListWidget::ListWidget(QWidget *parent) :
    QListWidget(parent)
{

}

void ListWidget::resizeEvent(QResizeEvent *event)
{
    setIconSize(QSize(event->size().width() - spacing() * 2.5,
                      event->size().height()));
    QListWidget::resizeEvent(event);
}
