#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include <QSharedPointer>

#include <array>

class AbstractNumberProvider
{
public:
    virtual QString getNumber() = 0;
    virtual QString checkNumber(const QString &input, bool &ok) = 0;
    virtual QString formatHint() = 0;
    virtual ~AbstractNumberProvider(){};

    AbstractNumberProvider() = default;

    AbstractNumberProvider & operator=(const AbstractNumberProvider &) = delete;
    AbstractNumberProvider & operator=(const AbstractNumberProvider &&) = delete;
    AbstractNumberProvider(AbstractNumberProvider &) = delete;
    AbstractNumberProvider(AbstractNumberProvider &&) = delete;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ProviderType
    {
        NUMBER,
        DATE,
        TIME,
        PHONE_NUMBER,

        PROVIDERS_COUNT
    };
    Q_ENUM(ProviderType)

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const QString rateKey = "rate";
    const QString rangeKey = "range";

    std::array<QCheckBox *, PROVIDERS_COUNT> checkBoxes;
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
