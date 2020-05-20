#include "SpeechConfigDialog.h"
#include "ui_SpeechConfigDialog.h"

SpeechConfigDialog::SpeechConfigDialog(Speaker &speaker, QWidget *parent) :
    QDialog(parent),
    speaker(speaker),
    ui(new Ui::SpeechConfigDialog)
{
    ui->setupUi(this);

    ui->engine->addItem("Default", QString("default"));
    const auto engines = Speaker::availableEngines();
    for (const QString &engine : engines)
        ui->engine->addItem(engine, engine);
    ui->engine->setCurrentIndex(0);
    engineSelected(0);

    connect(ui->sayButton, &QPushButton::clicked,
            [this](){ this->speaker.say(ui->plainTextEdit->toPlainText());});
    connect(ui->pitch, &QSlider::valueChanged, this, &SpeechConfigDialog::selectPitch);
    connect(ui->rate, &QSlider::valueChanged, this, &SpeechConfigDialog::selectRate);
    connect(ui->volume, &QSlider::valueChanged, this, &SpeechConfigDialog::selectVolume);
    connect(ui->engine, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::engineSelected);
}

SpeechConfigDialog::~SpeechConfigDialog()
{
    delete ui;
}

void SpeechConfigDialog::selectRate(int rate)
{
    speaker.setRate(rate / 10.0);
}

void SpeechConfigDialog::selectPitch(int pitch)
{
    speaker.setPitch(pitch / 10.0);
}

void SpeechConfigDialog::selectVolume(int volume)
{
    speaker.setVolume(volume / 100.0);
}

void SpeechConfigDialog::engineSelected(int index)
{
    QString engineName = ui->engine->itemData(index).toString();
    speaker.setEngine(engineName);
    disconnect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::languageSelected);
    ui->language->clear();
    // Populate the languages combobox before connecting its signal.
    const QVector<QLocale> locales = speaker.availableLocales();
    QLocale current = speaker.locale();
    for (const QLocale &locale : locales) {
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        QVariant localeVariant(locale);
        ui->language->addItem(name, localeVariant);
        if (locale.name() == current.name())
            current = locale;
    }
    selectRate(ui->rate->value());
    selectPitch(ui->pitch->value());
    selectVolume(ui->volume->value());

    connect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::languageSelected);
    localeChanged(current);
}

void SpeechConfigDialog::languageSelected(int language)
{
    QLocale locale = ui->language->itemData(language).toLocale();
    speaker.setLocale(locale);
}

void SpeechConfigDialog::voiceSelected(int index)
{
    speaker.setVoice(m_voices.at(index));
}

void SpeechConfigDialog::localeChanged(const QLocale &locale)
{
    QVariant localeVariant(locale);
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));

    disconnect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::voiceSelected);
    ui->voice->clear();

    m_voices = speaker.availableVoices();
    QVoice currentVoice = speaker.voice();
    for (const QVoice &voice : qAsConst(m_voices)) {
        ui->voice->addItem(QString("%1 - %2 - %3").arg(voice.name())
                          .arg(QVoice::genderName(voice.gender()))
                          .arg(QVoice::ageName(voice.age())));
        if (voice.name() == currentVoice.name())
            ui->voice->setCurrentIndex(ui->voice->count() - 1);
    }
    connect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::voiceSelected);
}
