#include "Speaker.h"

Speaker::Speaker(QObject *parent) : QObject(parent)
{
    speaker = QSharedPointer<QTextToSpeech>::create();
}

QVector<QLocale> Speaker::availableLocales() const
{
    return speaker->availableLocales();
}

QVector<QVoice> Speaker::availableVoices() const
{
    return speaker->availableVoices();
}

QLocale Speaker::locale() const
{
    return speaker->locale();
}

double Speaker::pitch() const
{
    return speaker->pitch();
}

double Speaker::rate() const
{
    return speaker->rate();
}

QTextToSpeech::State Speaker::state() const
{
    return speaker->state();
}

QVoice Speaker::voice() const
{
    return speaker->voice();
}

double Speaker::volume() const
{
    return speaker->volume();
}

QStringList Speaker::availableEngines()
{
    return QTextToSpeech::availableEngines();
}

void Speaker::setEngine(const QString &name)
{
    if(name.toLower() == "default"){
        speaker = SpeakerPtr::create();
    }else{
        speaker = SpeakerPtr::create(name);
    }
}

void Speaker::say(const QString &text)
{
    speaker->say(text);
}

void Speaker::pause()
{
    speaker->pause();
}

void Speaker::resume()
{
    speaker->resume();
}

void Speaker::setLocale(const QLocale &locale)
{
    speaker->setLocale(locale);
}

void Speaker::setPitch(double pitch)
{
    speaker->setPitch(pitch);
}

void Speaker::setPitch(int pitch)
{
    speaker->setPitch(pitch * 0.01);
}

void Speaker::setRate(double rate)
{
    speaker->setRate(rate);
}

void Speaker::setRate(int rate)
{
    speaker->setRate(rate * 0.01);
}

void Speaker::setVoice(const QVoice &voice)
{
    speaker->setVoice(voice);
}

void Speaker::setVolume(double volume)
{
    speaker->setVolume(volume);
}

void Speaker::stop()
{
    speaker->stop();
}
