#include "mainwindow.h"

#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QMessageBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QDebug>
#include <QIntValidator>
#include <QSettings>
#include <QDateTime>
#include <QMetaEnum>

#include <time.h>
#include <limits>
#include <random>

class NumberProvider : public AbstractNumberProvider
{
    std::default_random_engine randomDevice;
    int num;
    int range;
public:
    explicit NumberProvider(int range) : range(range)
    {
        randomDevice.seed(time(0));
        num = randomDevice() % (range + 1);
    }
    QString getNumber() override
    {
        return QString::number(num);
    }
    QString checkNumber(const QString &input, bool &ok) override
    {
        ok = input.toInt() == num;
        return QString::number(num);
    }
    virtual QString formatHint() override
    {
        return QString{"12345678910987654321"}.left(QString::number(range).length());
    }
};
class PhoneNumberProvider : public AbstractNumberProvider
{
    std::default_random_engine randomDevice;
    QString num;
public:
    explicit PhoneNumberProvider()
    {
        randomDevice.seed(time(0));
        num = QString{"%1-%2-%3-%4"}.arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
                .arg(rangedRandom(0, 999), 3, 10, QLatin1Char('0'))
                .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0'))
                .arg(rangedRandom(0, 99), 2, 10, QLatin1Char('0'));
    }
    QString getNumber() override
    {
        qDebug() << __LINE__ << num;
        return num;
    }
    QString checkNumber(const QString &input, bool &ok) override
    {
        ok = input == num;
        return num;
    }
    int rangedRandom(int minimum, int maximum)
    {
        int rNum = randomDevice() % (maximum - minimum) + minimum;
        return rNum;
    }
    virtual QString formatHint() override
    {
        return "123-456-78-90";
    }
};
class DateProvider : public AbstractNumberProvider
{
    std::default_random_engine randomDevice;
    QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
    QDate date;
    QString format = "MMMM d yyyy";
public:
    DateProvider()
    {
        randomDevice.seed(time(0));
        date = QDate(rangedRandom(1900, 2050),
                     rangedRandom(1, 12),
                     rangedRandom(1, 31));
    }
    QString getNumber() override
    {
        return toStr(date);
    }
    QString checkNumber(const QString &input, bool &ok) override
    {
        QDate inputDate = fromStr(input);
        ok = inputDate == date;
        qDebug() << input << inputDate << toStr(inputDate) << date  << toStr(date);
        return toStr(date);
    }
    int rangedRandom(int minimum, int maximum)
    {
        int rNum = randomDevice() % (maximum - minimum) + minimum;
        return rNum;
    }
    QString toStr(const QDate &date)
    {
        QString dateString = locale.toString(date, format);
        qDebug() << __LINE__ << dateString;
        return dateString;
    }
    QDate fromStr(const QString &input)
    {
        return locale.toDate(input, format);
    }
    virtual QString formatHint() override
    {
        return "January 56 1976";
    }
};
class TimeProvider : public AbstractNumberProvider
{
    std::default_random_engine randomDevice;
    QLocale locale = QLocale{QLocale::English, QLocale::UnitedStates};
    QTime t;
    QString format = "H:mm a";
public:
    TimeProvider()
    {
        randomDevice.seed(time(0));
        t = QTime(rangedRandom(0, 23),
                     rangedRandom(0, 59));
    }
    QString getNumber() override
    {
        return toStr(t);
    }
    QString checkNumber(const QString &input, bool &ok) override
    {
        QTime inputTime = fromStr(input);
        ok = inputTime == t;
        qDebug() << input << inputTime << toStr(inputTime) << t  << toStr(t);
        return toStr(t);
    }
    int rangedRandom(int minimum, int maximum)
    {
        int rNum = randomDevice() % (maximum - minimum) + minimum;
        return rNum;
    }
    QString toStr(const QTime &time)
    {
        QString timeString = locale.toString(time, format);
        qDebug() << __LINE__ << timeString;
        return timeString;
    }
    QTime fromStr(const QString &input)
    {
        return locale.toTime(input, format);
    }
    virtual QString formatHint() override
    {
        return "01:45 pm";
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      num(0)
{
    QSettings settings;
    int rate = settings.value(rateKey, 0).toInt();
    range = settings.value(rangeKey, 999).toInt();

    setCentralWidget(new QWidget);
    {
        QVBoxLayout *mainLayout = new QVBoxLayout{centralWidget()};
        {
            QGridLayout *layout = new QGridLayout;
            mainLayout->addLayout(layout);
            for(int i = NUMBER; i < PROVIDERS_COUNT; ++i)
            {
                {
                    QMetaEnum metaEnum =
                            QMetaEnum::fromType<ProviderType>();
                    const char *value = metaEnum.valueToKey(i);
                    QCheckBox *cb = new QCheckBox(value);
                    cb->setChecked(true);
                    providerCheckBoxes[i] = cb;
                    connect(cb, &QCheckBox::toggled, this, &MainWindow::checked);
                }
                layout->addWidget(providerCheckBoxes[i], 0, i);
                {
                    labels[i] = new QLabel;
                }
                layout->addWidget(labels[i], 1, i);
            }
        }
        mainLayout->addWidget(hintLabel = new QLabel);
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel(tr("Answer")));
            {
                answerEdit = new QLineEdit;
                layout->addWidget(answerEdit);
//                answerEdit->setValidator(new QIntValidator);
                connect(answerEdit, &QLineEdit::returnPressed, this, &MainWindow::answer);
            }
            {
                QPushButton *answerButton = new QPushButton("OK");
                layout->addWidget(answerButton);
                connect(answerButton, &QPushButton::clicked, this, &MainWindow::answer);

            }
            {
                QPushButton *repeatButton = new QPushButton(tr("Repeat"));
                layout->addWidget(repeatButton);
                connect(repeatButton, &QPushButton::clicked, this, &MainWindow::repeat);
            }
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel{tr("Speech rate")});
            {
                QSlider * speechRateSlider = new QSlider{Qt::Horizontal};
                speechRateSlider->setMinimum(-100);
                speechRateSlider->setMaximum(100);
                speechRateSlider->setValue(rate);
                layout->addWidget(speechRateSlider);
                connect(speechRateSlider, &QSlider::valueChanged, this, &MainWindow::setRate);
            }
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel{tr("Numbers range")});
            {
                QSpinBox * spinBox = new QSpinBox;
                spinBox->setMinimum(10);
                spinBox->setMaximum(__INT_MAX__);
                spinBox->setValue(range);
                layout->addWidget(spinBox);
                connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(setRange(int)));
            }
        }
        {
            statusLabel = new QLabel;
            mainLayout->addWidget(statusLabel);
        }
    }
    {
        setRate(rate);
    }
    speak();

}

MainWindow::~MainWindow()
{
}

void MainWindow::speak()
{
    if(on)
    {
        switch(providerType)
        {
        default:
        case NUMBER:numberProvider = QSharedPointer<AbstractNumberProvider>(new NumberProvider{range});
            break;
        case DATE:numberProvider = QSharedPointer<AbstractNumberProvider>(new DateProvider);
            break;
        case TIME:numberProvider = QSharedPointer<AbstractNumberProvider>(new TimeProvider);
            break;
        case PHONE_NUMBER:numberProvider = QSharedPointer<AbstractNumberProvider>(new PhoneNumberProvider);
            break;
        }
        hintLabel->setText(QString{"Format: %1"}.arg(numberProvider->formatHint()));
        pronounce();
    }
}

void MainWindow::repeat()
{
    pronounce();
    answerEdit->setFocus();
}

void MainWindow::pronounce()
{
    speaker.say(numberProvider->getNumber());
}

void MainWindow::answer()
{
    bool ok;
    {
        QString rightAnswer = numberProvider->checkNumber(answerEdit->text(), ok);
        ++(answerCounter[providerType]);
        if(ok)
        {
            speaker.say(tr("Right!"));
            positive++;
            ++(rightAnswersCounter[providerType]);
        }else
        {
            QString text = tr("Auch! It was \n %1").arg(rightAnswer);
            speaker.say(text);
            QMessageBox::information(this, tr("Mistake"), text);
            negative++;
        }
        {
            QString text = tr("Positive: %1 Negative: %2").arg(positive).arg(negative);
            statusLabel->setText(text);
        }
        labels[providerType]->setText(tr("%1/%2").
                                      arg(rightAnswersCounter[providerType]).
                                      arg(answerCounter[providerType]));
    }
    {
        do
        {
            providerType = static_cast<ProviderType>((providerType + 1) % PROVIDERS_COUNT);
        }while(!providerCheckBoxes[providerType]->isChecked());
    }


    speak();
    answerEdit->clear();
    answerEdit->setFocus();
}

void MainWindow::setRate(int rate)
{
    QSettings settings;
    settings.setValue(rateKey, rate);
    speaker.setRate(rate * 0.01);
}

void MainWindow::setRange(int range)
{
    QSettings settings;
    settings.setValue(rangeKey, range);
    this->range = range;
}

void MainWindow::checked()
{
    bool oldOn = on;
    on = false;
    for(auto cb : providerCheckBoxes)
    {
        on |= cb->isChecked();
    }

    if(!oldOn && on)
    {
        speak();
    }
}

