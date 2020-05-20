#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>

#include <array>

#include "ProviderFactory.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ProviderFactory &providerFactory;
public:
    explicit MainWindow(ProviderFactory &providerFactory, QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::array<QCheckBox *, PROVIDERS_COUNT> providerCheckBoxes;
    std::array<QLabel *, PROVIDERS_COUNT> labels;
    std::array<int, PROVIDERS_COUNT> rightAnswersCounter = {{0}};
    std::array<int, PROVIDERS_COUNT> answerCounter = {{0}};
    QLineEdit *answerEdit;
    QLabel *hintLabel;
    QLabel *statusLabel;
    QCheckBox *repeatCheckBox;
    QSlider * repeatProbabilitySlider;

    int num;
    int range;
    int negative = 0;
    int positive = 0;
    bool on;

    void answer();
    void updateQuestion();
    void repeat();
    void setRate(int rate);
    void pronounce(QString word);
    void checked();
    void updateStatus();

    ProviderType providerType = NUMBER;

public slots:
    void loadImage();
    void setRange(int range);

signals:
    void say(QString);
    void showSayDialog();
    void showSpeechConfigDialog();
};
#endif // MAINWINDOW_H
