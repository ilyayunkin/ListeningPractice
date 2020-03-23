#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextToSpeech>
#include <QLineEdit>
#include <QLabel>

#include <QSharedPointer>

class AbstractNumberProvider
{
public:
    virtual QString getNumber() = 0;
    virtual QString checkNumber(const QString &input, bool &ok) = 0;
    virtual ~AbstractNumberProvider(){};
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum ProviderType
    {
        NUMBER,
        DATE,

        PROVIDERS_COUNT
    };
    ProviderType providerType = NUMBER;
    const QString rateKey = "rate";
    const QString rangeKey = "range";

    QLineEdit *answerEdit;
    QLabel *statusLabel;
    QTextToSpeech speaker;

    QSharedPointer<AbstractNumberProvider> numberProvider;

    int num;
    int range;
    int negative = 0;
    int positive = 0;

    void answer();
    void speak();
    void repeat();
    void setRate(int rate);
    void pronounce();
//    int rangedRandom(int minimum, int maximum);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setRange(int range);
};
#endif // MAINWINDOW_H
