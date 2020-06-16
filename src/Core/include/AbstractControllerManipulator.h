#ifndef ABSTRACTCONTROLLERMANIPULATOR_H
#define ABSTRACTCONTROLLERMANIPULATOR_H

#include <QStringView>
#include "ProviderType.h"
#include "ProvidersFlags.h"

class AbstractControllerManipulator
{
public:
    virtual ~AbstractControllerManipulator() = default;
    virtual void play() = 0;
    virtual void answer(const QString &text) = 0;
    virtual void say(const QString &text) = 0;
    virtual void config(ProvidersFlags flags) = 0;
    virtual void setRange(int val) = 0;
    virtual void setRate(int val) = 0;
    virtual void setRepeat(const bool on, int probability) = 0;
};

#endif // ABSTRACTCONTROLLERMANIPULATOR_H
