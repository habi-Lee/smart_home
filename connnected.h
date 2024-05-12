#ifndef CONNNECTED_H
#define CONNNECTED_H

#include <QWidget>
#include <widget.h>
#include <events.h>

namespace Ui {
class connnected;
}

class connnected : public QWidget
{
    Q_OBJECT

public:
    explicit connnected(QWidget *parent = nullptr);
    ~connnected();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_connnected_customContextMenuRequested(const QPoint &pos);

private:
    Ui::connnected *ui;
    Widget *w;
    events *e;
};

#endif // CONNNECTED_H
