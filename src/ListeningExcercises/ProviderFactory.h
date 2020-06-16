#ifndef PROVIDERFACTORY_H
#define PROVIDERFACTORY_H

#include <QObject>
#include <QStringList>
#include <QSharedPointer>

#include <array>

#include "AbstractListeningProvider.h"
#include "WordsStorage.h"
#include "PhrasesStorage.h"
#include "AbstractListeningFactory.h"

class ProviderFactory : public QObject, public AbstractListeningFactory
{
    Q_OBJECT
    const WordsStorage &wordsStorage;
    const PhrasesStorage &phrasesStorage;
    bool repeatFails = false;
    std::array<QStringList, PROVIDERS_COUNT> failed;
    QSharedPointer<AbstractListeningProvider> numberProvider;

    QSharedPointer<AbstractListeningProvider> getProvider(
            const ProviderType providerType, const int range) const;
    QSharedPointer<AbstractListeningProvider> getProvider(
            const ProviderType providerType, const QString &str) const;
public:
    explicit ProviderFactory (const WordsStorage &wordsStorage,
                              const PhrasesStorage &phrasesStorage,
                              QObject *parent = nullptr);
    ~ProviderFactory();
    QString get(const ProviderType providerType, int range, bool repeatFails, int probability);
    QString repeat();
    QString check(const QString &input, bool &ok);
    QString formatHint();
    bool inWork();
};

#endif // PROVIDERFACTORY_H
