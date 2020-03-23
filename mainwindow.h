#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>
#include <QLineEdit>
#include <QLabel>

#include <random>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    const QString rateKey = "rate";
    const QString rangeKey = "range";

    QLineEdit *answerEdit;
    QLabel *statusLabel;
    QTextToSpeech speaker;
    std::default_random_engine randomDevice;
    int num;
    int range;
    int negative = 0;
    int positive = 0;

    void answer();
    void speak();
    void repeat();
    void setRate(int rate);
    void pronounce(int num);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setRange(int range);
};
#endif // MAINWINDOW_H
