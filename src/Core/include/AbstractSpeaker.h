#ifndef ABSTRACTSPEAKER_H
#define ABSTRACTSPEAKER_H

#include <QStringView>
#include <QString>
#include <QVector>

class AbstractSpeaker
{
public:
    virtual ~AbstractSpeaker() = default;

    virtual void say(const QString &text) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    virtual void setPitch(double val) = 0;
    virtual void setRate(double val) = 0;
    virtual void setVolume(double val) = 0;

    virtual double getPitch() const = 0;
    virtual double getRate() const = 0;
    virtual double getVolume() const = 0;

    virtual QVector<QString> availableEngines() const = 0;
    virtual void setEngine(const QString &name) = 0;

    virtual QVector<QString> availableLanguages() const = 0;
    virtual QString currentLanguage() const = 0;
    virtual void setLanguage(const QString &name) = 0;

    virtual QVector<QString> availableVoices() const = 0;
    virtual QString currentVoice() const = 0;
    virtual void setVoice(const QString &name) = 0;
};

#endif // ABSTRACTSPEAKER_H
