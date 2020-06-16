#ifndef SPEECHCONFIGDIALOG_H
#define SPEECHCONFIGDIALOG_H

#include <QDialog>
#include <QVector>

#include "AbstractSpeaker.h"

namespace Ui {
class SpeechConfigDialog;
}

class SpeechConfigDialog : public QDialog
{
    Q_OBJECT
    AbstractSpeaker *speaker;
    void selectRate(int rate);
    void selectPitch(int pitch);
    void selectVolume(int volume);

    void engineSelected(int index);
    void languageSelected(int language);
    void voiceSelected(int index);

    void localeChanged(const QLocale &locale);

public:
    explicit SpeechConfigDialog(AbstractSpeaker *speaker, QWidget *parent = nullptr);
    ~SpeechConfigDialog();

private:
    Ui::SpeechConfigDialog *ui;
};

#endif // SPEECHCONFIGDIALOG_H
