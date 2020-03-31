#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include <QSharedPointer>

#include <array>

#include "AbstractNumberProvider.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const QString rateKey = "rate";
    const QString rangeKey = "range";
    const QString onKey = "on";
    std::array<QString, PROVIDERS_COUNT> providerTitles =
    {
        tr("Number"),
        tr("Date"),
        tr("Time"),
        tr("Phone number"),
    };

    std::array<QCheckBox *, PROVIDERS_COUNT> providerCheckBoxes;
    std::array<QLabel *, PROVIDERS_COUNT> labels;
    std::array<int, PROVIDERS_COUNT> rightAnswersCounter = {0};
    std::array<int, PROVIDERS_COUNT> answerCounter = {0};
    QLineEdit *answerEdit;
    QLabel *hintLabel;
    QLabel *statusLabel;
    QTextToSpeech speaker;

    QSharedPointer<AbstractNumberProvider> numberProvider;

    int num;
    int range;
    int negative = 0;
    int positive = 0;
    bool on = true;

    void answer();
    void speak();
    void repeat();
    void setRate(int rate);
    void pronounce();
    void checked();

    ProviderType providerType = NUMBER;

public slots:
    void setRange(int range);
};
#endif // MAINWINDOW_H
