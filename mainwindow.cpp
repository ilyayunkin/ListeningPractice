#include "mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>
#include <QIntValidator>
#include <QMessageBox>

#include <time.h>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      num(0),
      range(100)
{
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
        }
        {
            QHBoxLayout *layout = new QHBoxLayout;
            mainLayout->addLayout(layout);
            layout->addWidget(new QLabel(tr("Speech rate")));
            {
                QSlider * slider = new QSlider(Qt::Horizontal);
                slider->setMinimum(-100);
                slider->setMaximum(100);
                layout->addWidget(slider);
                connect(slider, QSlider::valueChanged, this, MainWindow::setRate);

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
    }

    randomDevice.seed(time(0));
    {
        const double speechRate = 0;
        speaker.setRate(speechRate);
    }
    speak();
}

MainWindow::~MainWindow()
{
}

void MainWindow::speak()
{
    num = randomDevice() % (range + 1);
    speaker.say(QString::number(num));
}

void MainWindow::answer()
{
    int answerI = answerEdit->text().toInt();
    if(answerI == num)
    {
        speaker.say(tr("Right!"));
    }else
    {
        QString text = tr("Auch! It was %1").arg(num);
        speaker.say(text);
        QMessageBox::information(this, tr("Mistake"), text);
    }
    speak();
}

void MainWindow::setRate(int rate)
{
    speaker.setRate(rate * 0.01);
}

void MainWindow::setRange(int range)
{
    this->range = range;
}
