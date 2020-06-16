#include "Speaker.h"

Speaker::Speaker(QObject *parent) : QObject(parent)
{
    speaker = QSharedPointer<QTextToSpeech>::create();
}

QVector<QString> Speaker::availableLanguages() const
{
    QVector<QString> ret;
    for(auto l : speaker->availableLocales())
    {
        ret.push_back(l.name());
    }
    return ret;
}

QVector<QString> Speaker::availableVoices() const
{
    QVector<QString> ret;
    for(auto v : speaker->availableVoices())
    {
        ret.push_back(v.name());
    }
    return ret;
}

QString Speaker::currentLanguage() const
{
    return speaker->locale().name();
}

double Speaker::getPitch() const
{
    return speaker->pitch();
}

double Speaker::getRate() const
{
    return speaker->rate();
}

QTextToSpeech::State Speaker::state() const
{
    return speaker->state();
}

QString Speaker::currentVoice() const
{
    return speaker->voice().name();
}

double Speaker::getVolume() const
{
    return speaker->volume();
}

QVector<QString> Speaker::availableEngines() const
{
    QVector<QString> ret;
    for(auto e : QTextToSpeech::availableEngines())
    {
        ret.push_back(e);
    }
    return ret;
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

void Speaker::setPitch(double pitch)
{
    speaker->setPitch(pitch);
}

void Speaker::setRate(double rate)
{
    speaker->setRate(rate);
}

void Speaker::setVoice(const QString &voice)
{
    for(auto v : speaker->availableVoices())
    {
        if(v.name() == voice)
        {
            speaker->setVoice(v);
        }
    }
}

void Speaker::setLanguage(const QString &language)
{
    for(auto l : speaker->availableLocales())
    {
        if(l.name() == language)
        {
            speaker->setLocale(l);
        }
    }
}

void Speaker::setVolume(double volume)
{
    speaker->setVolume(volume);
}

void Speaker::stop()
{
    speaker->stop();
}
