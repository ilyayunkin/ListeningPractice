#include "Controller.h"

#include <QString>
#include <QDebug>
#include <assert.h>

Controller::Controller()
{

}

void Controller::init()
{
    assert(settings);

    {
        providersFlags = settings->getActive();
        int i = 0;
        for(bool cb : providersFlags.on)
        {
            if(cb)
            {
                if(!on)
                {
                    on = true;
                    providerType = static_cast<ProviderType>(i);
                }
            }
            ++i;
        }
        view->showFlags(providersFlags);
    }
    view->showRate(settings->getRate());
    view->showRange(settings->getRange());
    {
        settings->getRepeat(repeat, probability);
        view->showRepeat(repeat, probability);
    }
}

void Controller::play()
{
    assert(providerFactory);
    assert(view);

    if(providerFactory->inWork()){
        speaker->say(providerFactory->repeat());
    }else{
        QString word = providerFactory->get(providerType,
                                           range,
                                           repeat,
                                           probability);
        view->showAnswerFormat(QString{"Format: %1"}.arg(providerFactory->formatHint()));
        speaker->say(word);
    }
}

void Controller::answer(const QString &text)
{
    assert(speaker);
    assert(view);
    assert(providerFactory);

    if(providerFactory->inWork())
    {
        bool ok;
        {
            QString rightAnswer = providerFactory->check(text, ok);
            ++(counters.answersCnt[providerType]);
            if(ok)
            {
                speaker->say("Right!");
                counters.positiveCommonCnt++;
                ++(counters.positiveCnt[providerType]);
            }else
            {
                QString text = QString("Wrong! It was \n %1").arg(rightAnswer);
                speaker->say(text);
                view->showError("Wrong", text);
                counters.negativeCommonCnt++;
            }
            view->showGameCounters(counters);
        }
        {
            if(std::any_of(std::begin(providersFlags.on), std::end(providersFlags.on),
                           [](bool c){return c;}))
            {
                do
                {
                    providerType = static_cast<ProviderType>((providerType + 1) % PROVIDERS_COUNT);
                }while(!providersFlags.on[providerType]);
                QString word = providerFactory->get(providerType,
                                                    range,
                                                    repeat,
                                                    probability);
                view->showAnswerFormat(QString{"Format: %1"}.arg(providerFactory->formatHint()));
                speaker->say(word);
            }
        }
    }
}

void Controller::say(const QString &text)
{
    assert(speaker);

    speaker->say(text);
}

void Controller::config(ProvidersFlags flags)
{
    assert(settings);

    bool oldOn = on;
    on = false;
    QString onList;
    int i = 0;
    for(auto cb : flags.on)
    {
        on |= cb;
        ++i;
        qDebug() << cb;
    }

    providersFlags = flags;

    if(!oldOn && on)
    {
        play();
    }

    settings->saveActive(flags);
}

void Controller::setSpeaker(AbstractSpeaker *speaker)
{
    assert(this->speaker == nullptr);
    this->speaker = speaker;
}

void Controller::setView(AbstractView *view)
{
    assert(this->view == nullptr);
    this->view = view;
}

void Controller::setProviderFactory(AbstractListeningFactory *providerFactory)
{
    assert(this->providerFactory == nullptr);
    this->providerFactory = providerFactory;
}

void Controller::setSettings(AbstractSettings *settings)
{
    assert(this->settings == nullptr);
    this->settings = settings;
}

void Controller::setRange(int val)
{
    assert(settings);

    settings->setRange(val);
    range = val;
}

void Controller::setRate(int val)
{
    assert(settings);
    assert(speaker);

    settings->setRate(val);
    speaker->setRate(val * 0.01);
}

void Controller::setRepeat(const bool on, int probability)
{
    assert(settings);
    settings->saveRepeat(on, probability);
}
