#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>
#include <QLineEdit>

#include <random>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit *answerEdit;
    QTextToSpeech speaker;
    std::default_random_engine randomDevice;
    int num;
    int range;

    void answer();
    void speak();
    void setRate(int rate);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setRange(int range);
};
#endif // MAINWINDOW_H
