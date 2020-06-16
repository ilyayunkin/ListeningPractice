#ifndef SPEAKER_H
#define SPEAKER_H

#include <QObject>
#include <QTextToSpeech>
#include <QSharedPointer>

#include "AbstractSpeaker.h"

class Speaker : public QObject, public AbstractSpeaker
{
    Q_OBJECT
    typedef QSharedPointer<QTextToSpeech> SpeakerPtr;
    SpeakerPtr speaker;
public:
    explicit Speaker(QObject *parent = nullptr);

    QVector<QString> availableLanguages() const override;
    void setLanguage(const QString &currentLanguage) override;
    QString currentLanguage() const override;

    QVector<QString> availableVoices() const override;
    QString currentVoice() const override;

    QVector<QString> availableEngines() const override;
    void setEngine(const QString &name) override;

    double getPitch() const override;
    double getRate() const override;
    double getVolume() const override;

    QTextToSpeech::State state() const;

    void say(const QString &text) override;
    void pause() override;
    void resume() override;
    void stop() override;

    /**
     * @brief setPitch
     * @param pitch - a number from -1.0 to 1.0.
     */
    void setPitch(double getPitch) override;
    /**
     * @brief setRate
     * @param pitch - a number from -1.0 to 1.0.
     */
    void setRate(double getRate) override;
    void setVoice(const QString &currentVoice) override;
    void setVolume(double getVolume) override;
};

#endif // SPEAKER_H
