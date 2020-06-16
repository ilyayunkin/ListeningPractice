#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>

#include <array>

#include "ProviderType.h"
#include "AbstractView.h"
#include "AbstractControllerManipulator.h"
#include "AbstractSettings.h"

class MainWindow : public QMainWindow, public AbstractView
{
    Q_OBJECT
    AbstractControllerManipulator *controller = nullptr;
    AbstractSettings *settings;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setController(AbstractControllerManipulator *controller);
private:
    std::array<QCheckBox *, PROVIDERS_COUNT> providerCheckBoxes;
    std::array<QLabel *, PROVIDERS_COUNT> labels;
    QSpinBox * spinBox;
    QLineEdit *answerEdit;
    QLabel *hintLabel;
    QLabel *statusLabel;
    QCheckBox *repeatCheckBox;
    QSlider * repeatProbabilitySlider;

    void answer();
    void pronounce(QString word);
    void checked();

public slots:
    void setRate(int rate);
    void setRange(int range);

    // AbstractView interface
public:
    void showAnswerFormat(const QString &text) override;
    void showError(const QString &title, const QString &text) override;
    void showGameCounters(const GameCounters &counters) override;
    void showFlags(const ProvidersFlags &flags) override;
    void showRange(int val) override;
    void showRate(int val) override;
    void showRepeat(const bool on, const int probability) override;

signals:
    void showSayDialog();
    void showSpeechConfigDialog();
};
#endif // MAINWINDOW_H
