#include "SpeechConfigDialog.h"
#include "ui_SpeechConfigDialog.h"

SpeechConfigDialog::SpeechConfigDialog(AbstractSpeaker *speaker, QWidget *parent) :
    QDialog(parent),
    speaker(speaker),
    ui(new Ui::SpeechConfigDialog)
{
    ui->setupUi(this);

    ui->engine->addItem("Default", QString("default"));
    const auto engines = speaker->availableEngines();
    for (const auto &engine : engines)
        ui->engine->addItem(engine, engine);
    ui->engine->setCurrentIndex(0);
    engineSelected(0);

    connect(ui->sayButton, &QPushButton::clicked,
            [this](){ this->speaker->say(ui->plainTextEdit->toPlainText());});
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
    speaker->setRate(rate / 10.0);
}

void SpeechConfigDialog::selectPitch(int pitch)
{
    speaker->setPitch(pitch / 10.0);
}

void SpeechConfigDialog::selectVolume(int volume)
{
    speaker->setVolume(volume / 100.0);
}

void SpeechConfigDialog::engineSelected(int index)
{
    QString engineName = ui->engine->itemData(index).toString();
    speaker->setEngine(engineName);
    disconnect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::languageSelected);
    ui->language->clear();
    // Populate the languages combobox before connecting its signal.
    const auto locales = speaker->availableLanguages();
    auto current = speaker->currentLanguage();
    for (const auto &locale : locales) {
        ui->language->addItem(locale, locale);
    }
    selectRate(ui->rate->value());
    selectPitch(ui->pitch->value());
    selectVolume(ui->volume->value());

    connect(ui->language, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::languageSelected);
    localeChanged(current);
}

void SpeechConfigDialog::languageSelected(int language)
{
    auto locale = ui->language->itemData(language).toString();
    speaker->setLanguage(locale);
}

void SpeechConfigDialog::voiceSelected(int index)
{
    speaker->setVoice(ui->voice->itemText(index));
}

void SpeechConfigDialog::localeChanged(const QLocale &locale)
{
    QVariant localeVariant(locale);
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));

    disconnect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::voiceSelected);
    ui->voice->clear();

    auto m_voices = speaker->availableVoices();
    auto currentVoice = speaker->currentVoice();
    for (const auto &voice : qAsConst(m_voices)) {
        ui->voice->addItem(voice);
        if (voice == currentVoice)
            ui->voice->setCurrentIndex(ui->voice->count() - 1);
    }
    connect(ui->voice, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SpeechConfigDialog::voiceSelected);
}
