#include "Settings.h"

#include <array>

#include <QObject>
#include <QDebug>
#include <QSettings>

namespace
{
const QString rateKey = "rate";
const QString rangeKey = "range";
const QString onKey = "on";
const QString repeatKey = "repeat";
const QString probabilityKey = "probability";

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

Settings::Settings()
{

}

void Settings::saveActive(ProvidersFlags flags)
{
    int i = 0;
    QString onList;
    for(auto cb : flags.on)
    {
        if(cb)
        {
            onList+=providerTitles[i] + ',';
        }
        ++i;
    }

    QSettings settings;
    settings.setValue(onKey, onList);
    qDebug() << __PRETTY_FUNCTION__ << onList;
}

void Settings::saveRepeat(const bool on, const int probability)
{
    QSettings settings;
    settings.setValue(repeatKey, on);
    settings.setValue(probabilityKey, probability);
}

ProvidersFlags Settings::getActive()
{
    ProvidersFlags flags;
    QSettings settings;
    QString turnedOnProviders = settings.value(onKey, QString()).toString();
    qDebug() << __PRETTY_FUNCTION__ << turnedOnProviders;

    {
        for(int i = NUMBER; i < PROVIDERS_COUNT; ++i)
        {
            if(turnedOnProviders.isEmpty() ||
                    turnedOnProviders.contains(providerTitles[i]))
            {
                flags.on[i] = true;
            }else
            {
                flags.on[i] = false;
            }
        }
    }
    return flags;
}

void Settings::getRepeat(bool &on, int &probability)
{
    QSettings settings;

    on = settings.value(repeatKey, 0).toBool();
    probability = settings.value(probabilityKey, 999).toInt();
}

int Settings::getRate()
{
    QSettings settings;
    return settings.value(rateKey, 999).toInt();
}

int Settings::getRange()
{
    QSettings settings;
    return settings.value(rangeKey, 999).toInt();
}

void Settings::setRate(int val)
{
    QSettings settings;
    settings.setValue(rateKey, val);
}

void Settings::setRange(int val)
{
    QSettings settings;
    settings.setValue(rangeKey, val);
}
