#ifndef ANIMATEDDIALOG_H
#define ANIMATEDDIALOG_H

#include <QDialog>
#include <QEasingCurve>

class AnimatedDialog : public QDialog
{
    Q_OBJECT
public:

    explicit AnimatedDialog(QWidget *parent = nullptr);

  // Methods @set
    // for the default i have values for them
    // because most of my dialogs should looks like that
    void setAnimateMiliseconds(int miliseconds = 500);
    void setAnimateCurve(const QEasingCurve &curve = QEasingCurve::InOutBack);
    void setAnimateIncSize(int heightsize = 10);

 //Methods @get
    int getAnimateMiliseconds();
    QEasingCurve& getAnimateCurve();
    int getAnimateIncSize();

    //this function helps us
    //when we want to set dialog to the center positions
    // it get`s dialog width and divide it
    unsigned short divideDialogWidth();

    void removeControlButtons();

protected:
     void createDialogAnimation();

    // Events
     void showEvent(QShowEvent *event) override;

private:
     int miliseconds;
     int heightsize;
     QEasingCurve curve;
};

#endif
