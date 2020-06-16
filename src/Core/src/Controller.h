#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "AbstractControllerManipulator.h"
#include "AbstractSpeaker.h"
#include "AbstractSettings.h"
#include "AbstractView.h"
#include "AbstractListeningFactory.h"

class Controller : public AbstractControllerManipulator
{
    bool on = false;
    bool repeat = false;
    int probability = 20;
    int range = 999;
    GameCounters counters;
    ProvidersFlags providersFlags;
    ProviderType providerType = NUMBER;

    AbstractSpeaker *speaker = nullptr;
    AbstractView *view = nullptr;
    AbstractSettings *settings = nullptr;
    AbstractListeningFactory *providerFactory = nullptr;
public:
    Controller();

public:
    void init();

    void play();
    void answer(const QString &text);
    void say(const QString &text);
    void config(ProvidersFlags flags);

    void setSpeaker(AbstractSpeaker *speaker);
    void setView(AbstractView *view);
    void setProviderFactory(AbstractListeningFactory *providerFactory);
    void setSettings(AbstractSettings *settings);

    void setRange(int val);
    void setRate(int val);
    void setRepeat(const bool on, int probability);
};

#endif // CONTROLLER_H
