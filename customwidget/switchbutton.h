#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QAbstractButton>
#include <QBrush>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QPropertyAnimation>

class SwitchButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(int offset READ getOffset WRITE setOffset)
    Q_PROPERTY(QBrush brush READ getBrush WRITE setBrush)
public:
    SwitchButton(QWidget *parent = nullptr);
    SwitchButton(const QBrush& brush, QWidget* parent = nullptr);

    QSize sizeHint() const override;

// Methods @set
    void setSwitchStatus(bool status);
    void setBrush(const QBrush &brush);
    void setOffset(int offset);
    void setButtonHeight(int height);
    void setThumbColor(QColor &color);
    void setBrushColor(QColor &color);

// Methods @get
    bool isSwitchOn() const;
    QBrush getBrush() const;
    int getOffset() const;
    int getButtonHeight() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;

signals:
    void  buttonClicked();

private:
    bool switch_status;
    qreal opacity;
    int x, y, button_height, margin;
    QPropertyAnimation *animation;
    QBrush thumb, brush, track;
};

#endif
