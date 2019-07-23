#include "animateddialog.h"

#include <QPropertyAnimation>

AnimatedDialog::AnimatedDialog(QWidget *parent) : QDialog(parent)
{
    setAnimateMiliseconds();
    setAnimateIncSize();
    setAnimateCurve();
    removeControlButtons();
}


// Methods @set
  // for the default i have values for them
  // because most of my dialogs should looks like that
void AnimatedDialog::setAnimateMiliseconds(int miliseconds)
{
    this->miliseconds = miliseconds;
}

void AnimatedDialog::setAnimateCurve(const QEasingCurve &curve)
{
    this->curve = curve;
}

void AnimatedDialog::setAnimateIncSize(int heightsize)
{
    this->heightsize = heightsize;
}

 //Methods @get
int AnimatedDialog::getAnimateMiliseconds()
{
    return miliseconds;
}

QEasingCurve& AnimatedDialog::getAnimateCurve()
{
    return curve;
}

int AnimatedDialog::getAnimateIncSize()
{
    return heightsize;
}

//this function helps us when we want to set dialog to the center positions
// it get`s dialog width and divide it
unsigned short AnimatedDialog::divideDialogWidth()
{
    return this->width()/2;
}

void AnimatedDialog::removeControlButtons()
{
    // let`s remove buttons (close, minimize and maximize)
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

void AnimatedDialog::createDialogAnimation()
{
    QPropertyAnimation* animation = new QPropertyAnimation(this, "size");
    animation->setDuration(getAnimateMiliseconds());
    animation->setStartValue(QSize(this->width(), this->height()));
    animation->setEndValue(QSize(this->width(), this->height()+getAnimateIncSize()));
    animation->setEasingCurve(getAnimateCurve());
    animation->start();
}

// Events

void AnimatedDialog::showEvent(QShowEvent *event)
{
    createDialogAnimation();
}
