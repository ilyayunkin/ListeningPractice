#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>
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
    std::array<int, PROVIDERS_COUNT> rightAnswersCounter = {0};
    std::array<int, PROVIDERS_COUNT> answerCounter = {0};
    QLineEdit *answerEdit;
    QLabel *hintLabel;
    QLabel *statusLabel;
    QCheckBox *repeatCheckBox;
    QSlider * repeatProbabilitySlider;
    QTextToSpeech speaker;

    int num;
    int range;
    int negative = 0;
    int positive = 0;
    bool on = true;

    void answer();
    void speak();
    void repeat();
    void setRate(int rate);
    void pronounce(QString word);
    void checked();

    ProviderType providerType = NUMBER;

public slots:
    void loadImage();
    void setRange(int range);
};
#endif // MAINWINDOW_H
