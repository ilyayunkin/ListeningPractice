#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

#include <QStringView>
#include "GameCounters.h"
#include "ProvidersFlags.h"

class AbstractView
{
public:
    virtual ~AbstractView() = default;
    virtual void showAnswerFormat(const QString &text) = 0;
    virtual void showError(const QString &title, const QString &text) = 0;
    virtual void showGameCounters(const GameCounters &counters) = 0;
    virtual void showFlags(const ProvidersFlags &counters) = 0;
    virtual void showRange(int val) = 0;
    virtual void showRate(int val) = 0;
    virtual void showRepeat(const bool on, const int probability) = 0;
};

#endif // ABSTRACTVIEW_H
