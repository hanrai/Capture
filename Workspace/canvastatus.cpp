#include "canvastatus.h"
#include <QLabel>
#include <QHBoxLayout>
#include "Workspace/canva.h"

CanvaStatus::CanvaStatus(Canva *canva, QWidget *parent) : QWidget(parent),
    m_canva(canva),
    m_colorIndicator(new QLabel),
    m_colorName(new QLabel),
    m_position(new QLabel)
{
    m_colorIndicator->setText(" ");
    m_colorIndicator->setFrameStyle(QFrame::Box);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(2);
    layout->setSpacing(4);
    layout->addWidget(m_colorIndicator);
    layout->addWidget(m_colorName);
    layout->addWidget(m_position);
    setLayout(layout);
    connect(m_canva, &Canva::positionChanged, this, &CanvaStatus::updateInfo);
}

void CanvaStatus::updateInfo()
{
    setVisible(m_canva->position().x()>=0 ? true:false);
    QString colorName = m_canva->color().name();
    m_colorName->setText(colorName);
    m_position->setText(QString("(%1, %2)").arg(m_canva->position().x()).arg(m_canva->position().y()));
    QString style = QString("background: %1").arg(colorName);
    m_colorIndicator->setStyleSheet(style);
}
