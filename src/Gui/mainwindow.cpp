#include "mainwindow.h"

#include <QPushButton>
#include <QSpinBox>
#include <QMessageBox>
#include <QGroupBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QDebug>
#include <QRegularExpressionValidator>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

namespace
{
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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new QWidget);
    hintLabel = new QLabel;
    {
        for(int i = NUMBER; i < PROVIDERS_COUNT; ++i)
        {
            {
                QCheckBox *cb = new QCheckBox(providerTitles[i]);
                providerCheckBoxes[i] = cb;
                connect(cb, &QCheckBox::clicked, this, &MainWindow::checked);
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
    QPushButton *playButton;
    {
        playButton = new QPushButton(tr("Play"));
        connect(playButton, &QPushButton::clicked, [this]{controller->play();});
    }
    {
        spinBox = new QSpinBox;
        spinBox->setMinimum(10);
        spinBox->setMaximum(__INT_MAX__);
        connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(setRange(int)));
    }
    {
        repeatCheckBox = new QCheckBox(tr("Repeat failed"));

        connect(repeatCheckBox, &QCheckBox::clicked, this, &MainWindow::checked);
    }
    {
        repeatProbabilitySlider = new QSlider{Qt::Horizontal};
        repeatProbabilitySlider->setMinimum(1);
        repeatProbabilitySlider->setMaximum(100);
        connect(repeatProbabilitySlider, &QSlider::valueChanged, this, &MainWindow::checked);
    }
    {
        statusLabel = new QLabel;
    }

    {// Layouts
        QGridLayout *centralLayout = new QGridLayout{centralWidget()};
        {
            QGroupBox *checksBox = new QGroupBox(this);
            QGridLayout *layout = new QGridLayout(checksBox);
            centralLayout->addWidget(checksBox, 0, 0, 2, 1);
            for(int i = NUMBER; i < PROVIDERS_COUNT; ++i)
            {
                layout->addWidget(providerCheckBoxes[i], i, 0);
                layout->addWidget(labels[i], i, 1);
            }
        }
        {
            QGroupBox *playBox = new QGroupBox(this);
            centralLayout->addWidget(playBox, 0, 1);
            QVBoxLayout *playLay = new QVBoxLayout(playBox);
            {
                QHBoxLayout *layout = new QHBoxLayout;
                playLay->addLayout(layout);
                layout->addWidget(new QLabel(tr("Answer")));

                layout->addWidget(answerEdit);
                layout->addWidget(answerButton);
                layout->addWidget(playButton);
            }
            playLay->addWidget(hintLabel);
        }
        {
            QGroupBox *controlsBox = new QGroupBox(this);
            QVBoxLayout *controlsLay = new QVBoxLayout(controlsBox);
            centralLayout->addWidget(controlsBox, 1, 1);
            {
                QHBoxLayout *layout = new QHBoxLayout;
                controlsLay->addLayout(layout);
                layout->addWidget(new QLabel{tr("Numbers range")});
                layout->addWidget(spinBox);
            }
            {
                QHBoxLayout *layout = new QHBoxLayout;
                controlsLay->addLayout(layout);
                layout->addWidget(repeatCheckBox);
                layout->addWidget(new QLabel{tr("Probability")});
                layout->addWidget(repeatProbabilitySlider);
            }
        }
        {
            centralLayout->addWidget(statusLabel, 2, 0, 1, 2);
            //            statusLabel->setAlignment(Qt::AlignCenter);
        }
    }
    {
        QMenuBar *bar = new QMenuBar(this);
        setMenuBar(bar);
        {
            QMenu *toolsMenu = bar->addMenu(tr("Tools"));
            QAction *sayAction = toolsMenu->addAction(tr("Say"));

            connect(sayAction, &QAction::triggered, this, &MainWindow::showSayDialog);
        }
        setMenuBar(bar);
        {
            QMenu *configMenu = bar->addMenu(tr("Config"));
            QAction *speechConfigAction = configMenu->addAction(tr("Speech config"));

            connect(speechConfigAction, &QAction::triggered, this, &MainWindow::showSpeechConfigDialog);
        }
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::setController(AbstractControllerManipulator *controller)
{
    assert(this->controller == nullptr);
    this->controller = controller;
}

void MainWindow::answer()
{
    assert(controller);

    controller->answer(answerEdit->text());
    answerEdit->clear();
    answerEdit->setFocus();
}

void MainWindow::setRange(int range)
{
    assert(controller);

    controller->setRange(range);
}

void MainWindow::showAnswerFormat(const QString &text)
{
    hintLabel->setText(text);
}

void MainWindow::showError(const QString &title, const QString &text)
{
    QMessageBox::information(this, title, text);
}

void MainWindow::showGameCounters(const GameCounters &counters)
{
    for(size_t i = 0; i < labels.size(); ++i)
    {
        labels[i]->setText(tr("%1/%2").
                           arg(counters.positiveCnt[i]).
                           arg(counters.answersCnt[i]));
    }
    static const QString statusFormat = QObject::tr("Positive: %1 Negative: %2");
    QString text = statusFormat.arg(counters.positiveCommonCnt).arg(counters.negativeCommonCnt);
    statusLabel->setText(text);
}

void MainWindow::showFlags(const ProvidersFlags &flags)
{
    for(size_t i = 0; i < labels.size(); ++i)
    {
        providerCheckBoxes[i]->setChecked(flags.on[i]);
    }
}

void MainWindow::showRange(int val)
{
    spinBox->setValue(val);
}

void MainWindow::showRate(int val)
{
    (void)val;
    //repeatProbabilitySlider->setValue(val);
}

void MainWindow::showRepeat(const bool on, const int probability)
{
    repeatProbabilitySlider->setValue(probability);
    repeatCheckBox->setChecked(on);
}

void MainWindow::checked()
{
    qDebug() << __PRETTY_FUNCTION__ ;
    assert(controller);

    ProvidersFlags flags;
    for(int i = 0; i < PROVIDERS_COUNT; ++i){
        flags.on[i] = providerCheckBoxes[i]->isChecked();
    }
    controller->config(flags);
}

void MainWindow::setRate(int rate)
{
    spinBox->setValue(rate);
}
