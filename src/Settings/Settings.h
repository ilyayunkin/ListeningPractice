#ifndef SETTINGS_H
#define SETTINGS_H

#include "AbstractSettings.h"

class Settings : public AbstractSettings
{
public:
    Settings();

    // AbstractSettings interface
public:
    void saveActive(ProvidersFlags flags) override;
    void saveRepeat(const bool on, const int probability) override;

    ProvidersFlags getActive() override;
    void getRepeat(bool &on, int &probability) override;

    int getRate() override;
    int getRange() override;

    // AbstractSettings interface
public:
    void setRate(int val) override;
    void setRange(int val) override;
};

#endif // SETTINGS_H
