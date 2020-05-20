#ifndef SPEAKER_H
#define SPEAKER_H

#include <QObject>
#include <QTextToSpeech>
#include <QSharedPointer>

class Speaker : public QObject
{
    Q_OBJECT
    typedef QSharedPointer<QTextToSpeech> SpeakerPtr;
    SpeakerPtr speaker;
public:
    explicit Speaker(QObject *parent = nullptr);

    QVector<QLocale> availableLocales() const;
    QVector<QVoice> availableVoices() const;
    QLocale locale() const;
    double pitch() const;
    double rate() const;
    QTextToSpeech::State state() const;
    QVoice voice() const;
    double volume() const;
    static QStringList availableEngines();

public slots:
    void setEngine(const QString &name);
    void say(const QString &text);
    void pause();
    void resume();
    void setLocale(const QLocale &locale);
    /**
     * @brief setPitch
     * @param pitch - a number from -1.0 to 1.0.
     */
    void setPitch(double pitch);
    /**
     * @brief setPitch
     * @param pitch - a number between -100 and 100.
     */
    void setPitch(int pitch);
    /**
     * @brief setRate
     * @param pitch - a number from -1.0 to 1.0.
     */
    void setRate(double rate);
    /**
     * @brief setRate
     * @param value - a number between -100 and 100.
     */
    void setRate(int rate);
    void setVoice(const QVoice &voice);
    void setVolume(double volume);
    void stop();

signals:

};

#endif // SPEAKER_H
