#ifndef SAYDIALOG_H
#define SAYDIALOG_H

#include <QWidget>

namespace Ui {
class SayDialog;
}

class SayDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SayDialog(QWidget *parent = nullptr);
    ~SayDialog();

signals:
    void say(QString text);

private:
    Ui::SayDialog *ui;
};

#endif // SAYDIALOG_H
