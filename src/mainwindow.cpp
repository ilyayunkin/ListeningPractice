#include "mainwindow.h"

#include <QPushButton>
#include <QSpinBox>
#include <QMessageBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QDebug>
#include <QRegularExpressionValidator>
#include <QSettings>

namespace
{
const QString rateKey = "rate";
const QString rangeKey = "range";
const QString onKey = "on";
const QString repeatKey = "repeat";
const QString probabilityKey = "probability";

const std::array<QString, PROVIDERS_COUNT> providerTitles =
{
    QObject::tr("Number"),
    QObject::tr("Date"),
    QObject::tr("Time"),
    QObject::tr("Phone number"),
    QObject::tr("Word"),
    QObject::tr("Phrase"),
};
}

MainWindow::MainWindow(ProviderFactory &provider, QWidget *parent)
    : QMainWindow(parent),
      providerFactory(provider),
      num(0),
      on(false)
{
    QSettings settings;
    int rate = settings.value(rateKey, 0).toInt();
    range = settings.value(rangeKey, 999).toInt();
    QString turnedOnProviders = settings.value(onKey, QString()).toString();

    setCentralWidget(new QWidget);
    {
        for(int i = NUMBER; i < PROVIDERS_COUNT; ++i)
        {
            {
                QCheckBox *cb = new QCheckBox(providerTitles[i]);
                if(turnedOnProviders.isEmpty() ||
                        turnedOnProviders.contains(providerTitles[i]))
                {
                    cb->setChecked(true);
                    if(!on)
                    {
                        on = true;
                        providerType = static_cast<ProviderType>(i);
                    }
                }
                providerCheckBoxes[i] = cb;
                connect(cb, &QCheckBox::toggled, this, &MainWindow::checked);
            }
            labels[i] = new QLabel;
        }
    }
    {
        answerEdit = new QLineEdit;
        QRegularExpression rx("([a-z]|[A-Z]|[0-9]|\\:|\\-|\\'|\\s)+");
        QValidator *validator = new QRegularExpressionValidator(rx, this);
        answerEdit->setValidator(validator);
        connect(answerEdit, &QLineEdit::returnPressed, this, &MainWindow::answer);
    }
    QPushButton *answerButton;
    {
        answerButton = new QPushButton("OK");
        connect(answerButton, &QPushButton::clicked, this, &MainWindow::answer);
    }
    QPushButton *repeatButton;
    {
        repeatButton = new QPushButton(tr("Repeat"));
        connect(repeatButton, &QPushButton::clicked, this, &MainWindow::repeat);
    }
    QSlider * speechRateSlider;
    {
        speechRateSlider = new QSlider{Qt::Horizontal};
        speechRateSlider->setMinimum(-100);
        speechRateSlider->setMaximum(100);
        speechRateSlider->setValue(rate);
        connect(speechRateSlider, &QSlider::valueChanged, this, &MainWindow::setRate);
    }
    QSpinBox * spinBox;
    {
        spinBox = new QSpinBox;
        spinBox->setMinimum(10);
        spinBox->setMaximum(__INT_MAX__);
        spinBox->setValue(range);
        connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(setRange(int)));
    }
    {
        repeatCheckBox = new QCheckBox(tr("Repeat failed"));
        repeatCheckBox->setChecked(settings.value(repeatKey, true).toBool());

        connect(repeatCheckBox, &QCheckBox::toggled, this, &MainWindow::checked);
    }
    {
        repeatProbabilitySlider = new QSlider{Qt::Horizontal};
        repeatProbabilitySlider->setMinimum(1);
        repeatProbabilitySlider->setMaximum(100);
        repeatProbabilitySlider->setValue(rate);
        repeatProbabilitySlider->setValue(settings.value(probabilityKey, 30).toInt());
        connect(repeatProbabilitySlider, &QSlider::valueChanged, this, &MainWindow::checked);
    }
    {
        statusLabel = new QLabel;
    }

    {// Layouts
        QVBoxLayout *mainLayout = new QVBoxLayout{centralWidget()};
        {
            QGridLayout *layout = new QGridLayout;
            mainLayout->addLayout(layout);
            for(int i = NUMBER; i < PROVIDERS_COUNT; ++i)
            {
                layout->addWidget(providerCheckBoxes[i], 0, i);
                layout->addWidget(labels[i], 1, i);
            }
        }
        mainLayout->addWidget(hintLabel = new QLabel);
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel(tr("Answer")));

            layout->addWidget(answerEdit);
            layout->addWidget(answerButton);
            layout->addWidget(repeatButton);
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel{tr("Speech rate")});
            layout->addWidget(speechRateSlider);
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel{tr("Numbers range")});
            layout->addWidget(spinBox);
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(repeatCheckBox);
            layout->addWidget(new QLabel{tr("Probability")});
            layout->addWidget(repeatProbabilitySlider);
        }
        {
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
        QString word = providerFactory.get(providerType,
                                           range,
                                           repeatCheckBox->isChecked(),
                                           repeatProbabilitySlider->value());
        hintLabel->setText(QString{"Format: %1"}.arg(providerFactory.formatHint()));
        pronounce(word);
    }
}

void MainWindow::repeat()
{
    pronounce(providerFactory.repeat());
    answerEdit->setFocus();
}

void MainWindow::pronounce(QString word)
{
    speaker.say(word);
}

void MainWindow::answer()
{
    bool ok;
    {
        QString rightAnswer = providerFactory.check(answerEdit->text(), ok);
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
    QString onList;
    int i = 0;
    for(auto cb : providerCheckBoxes)
    {
        on |= cb->isChecked();
        if(cb->isChecked())
        {
            onList+=providerTitles[i] + ',';
        }
        ++i;
    }

    if(!oldOn && on)
    {
        speak();
    }

    QSettings settings;
    settings.setValue(onKey, onList);
    settings.setValue(repeatKey, repeatCheckBox->isChecked());
    settings.setValue(probabilityKey, repeatProbabilitySlider->value());
}

void MainWindow::loadImage()
{
    qDebug() << __FUNCTION__;
}
