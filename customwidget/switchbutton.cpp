#include "switchbutton.h"

#include <QColor>
#include <QRect>
#include <QRectF>
#include <QPainter>

SwitchButton::SwitchButton(QWidget *parent) : QAbstractButton(parent),
button_height(15), opacity(15), switch_status(false), margin(3), thumb("#d5d5d5")
{
   animation = new QPropertyAnimation(this, "offset", this);
   setOffset(button_height /2);
   y = button_height / 2;
   setBrush(QColor("#49CC52"));
}

SwitchButton::SwitchButton(const QBrush &brush, QWidget *parent):
  button_height(15), opacity(0), switch_status(false), margin(3), thumb("#d5d5d5")
{
    animation = new QPropertyAnimation(this);
    y = button_height / 2;
    setBrush(brush);
}

void SwitchButton::setSwitchStatus(bool status)
{
    this->switch_status = status;
    if (switch_status ==true)
        setOffset(60);
    else
        setOffset(10);
}

void SwitchButton::setBrush(const QBrush &brush)
{
    this->brush = brush;
}

void SwitchButton::setOffset(int offset)
{
    this->x = offset;
    update();
}

void SwitchButton::setButtonHeight(int height)
{
    this->button_height = height;
}

void SwitchButton::setThumbColor(QColor &color)
{
    this->thumb.setColor(color);
}

void SwitchButton::setBrushColor(QColor &color)
{
    this->brush.setColor(color);
}

bool SwitchButton::isSwitchOn() const
{
    return switch_status;
}

QBrush SwitchButton::getBrush() const
{
    return brush;
}

int SwitchButton::getOffset() const
{
    return x;
}

int SwitchButton::getButtonHeight() const
{
    return button_height;
}

QSize SwitchButton::sizeHint() const
{
    return QSize(2 * (button_height + margin), button_height + 2 * margin);
}

//Events

void SwitchButton::paintEvent(QPaintEvent *event)
{
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        if (isEnabled())
        {
            painter.setBrush(switch_status ? getBrush() : Qt::black);
            painter.setOpacity(switch_status ? 0.5 : 0.38);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawRoundedRect(QRect(margin, margin, width() - 2 * margin, height() - 2 * margin), 8.0, 8.0);
            painter.setBrush(thumb);
            painter.setOpacity(1.0);
            painter.drawEllipse(QRectF(getOffset() - (button_height / 2), y - (button_height / 2), height(), height()));
        }
        else
        {
            painter.setBrush(Qt::black);
            painter.setOpacity(0.12);
            painter.drawRoundedRect(QRect(margin, margin, width() - 2 * margin, height() - 2 * margin), 8.0, 8.0);
            painter.setOpacity(1.0);
            painter.setBrush(QColor("#BDBDBD"));
            painter.drawEllipse(QRectF(getOffset() - (button_height / 2), y - (button_height / 2), height(), height()));
        }
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
            switch_status = switch_status ? false : true;
            thumb = switch_status ? brush : QBrush("#d5d5d5");
            if (switch_status)
            {
                animation->setStartValue(button_height/ 2);
                animation->setEndValue(width() - button_height);
                animation->setDuration(120);
                animation->start();
                switch_status = true;
            }
            else
            {
                animation->setStartValue(getOffset());
                animation->setEndValue(button_height / 2);
                animation->setDuration(120);
                animation->start();
                switch_status = false;
            }
        }
        QAbstractButton::mouseReleaseEvent(event);
        emit buttonClicked();
}

void SwitchButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    QAbstractButton::enterEvent(event);
}
