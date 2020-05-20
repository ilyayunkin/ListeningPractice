#ifndef SPEECHCONFIGDIALOG_H
#define SPEECHCONFIGDIALOG_H

#include <QDialog>
#include <QVector>

#include <Speaker.h>
namespace Ui {
class SpeechConfigDialog;
}

class SpeechConfigDialog : public QDialog
{
    Q_OBJECT
    Speaker &speaker;
    void selectRate(int rate);
    void selectPitch(int pitch);
    void selectVolume(int volume);

    void engineSelected(int index);
    void languageSelected(int language);
    void voiceSelected(int index);

    void localeChanged(const QLocale &locale);

public:
    explicit SpeechConfigDialog(Speaker &speaker, QWidget *parent = nullptr);
    ~SpeechConfigDialog();

private:
    Ui::SpeechConfigDialog *ui;
    QVector<QVoice> m_voices;
};

#endif // SPEECHCONFIGDIALOG_H
