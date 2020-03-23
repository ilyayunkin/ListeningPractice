#include "mainwindow.h"

#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QMessageBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>
#include <QIntValidator>
#include <QSettings>

#include <time.h>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      num(0)
{
    QSettings settings;
    int rate = settings.value(rateKey, 0).toInt();
    range = settings.value(rangeKey, 100).toInt();

    setCentralWidget(new QWidget);
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget());
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel(tr("Answer")));
            {
                answerEdit = new QLineEdit;
                layout->addWidget(answerEdit);
                answerEdit->setValidator(new QIntValidator);
                connect(answerEdit, QLineEdit::returnPressed, this, MainWindow::answer);
            }
            {
                QPushButton *answerButton = new QPushButton("OK");
                layout->addWidget(answerButton);
                connect(answerButton, QPushButton::clicked, this, MainWindow::answer);

            }
            {
                QPushButton *repeatButton = new QPushButton(tr("Repeat"));
                layout->addWidget(repeatButton);
                connect(repeatButton, QPushButton::clicked, this, MainWindow::repeat);

            }
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel(tr("Speech rate")));
            {
                QSlider * speechRateSlider = new QSlider(Qt::Horizontal);
                speechRateSlider->setMinimum(-100);
                speechRateSlider->setMaximum(100);
                speechRateSlider->setValue(rate);
                layout->addWidget(speechRateSlider);
                connect(speechRateSlider, QSlider::valueChanged, this, MainWindow::setRate);

            }
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel(tr("Numbers range")));
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

    randomDevice.seed(time(0));
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
    num = randomDevice() % (range + 1);
    pronounce(num);
}

void MainWindow::repeat()
{
    pronounce(num);
}

void MainWindow::pronounce(int num)
{
    speaker.say(QString::number(num));
}

void MainWindow::answer()
{
    int answerI = answerEdit->text().toInt();
    if(answerI == num)
    {
        speaker.say(tr("Right!"));
        positive++;
    }else
    {
        QString text = tr("Auch! It was \n %1").arg(num);
        speaker.say(text);
        QMessageBox::information(this, tr("Mistake"), text);
        negative++;
    }
    {
        QString text = tr("Positive: %1 Negative: %2").arg(positive).arg(negative);
        statusLabel->setText(text);
    }
    speak();
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
