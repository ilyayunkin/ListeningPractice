#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H

#include "ProvidersFlags.h"

class AbstractSettings
{
public:
    virtual ~AbstractSettings() = default;
    virtual void saveActive(ProvidersFlags flags) = 0;
    virtual void saveRepeat(const bool on, const int probability) = 0;

    virtual ProvidersFlags getActive() = 0;
    virtual void getRepeat(bool &on, int &probability) = 0;

    virtual int getRate() = 0;
    virtual void setRate(int val) = 0;

    virtual int getRange() = 0;
    virtual void setRange(int val) = 0;
};

#endif // ABSTRACTSETTINGS_H
