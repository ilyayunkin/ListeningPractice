#ifndef PROVIDERFACTORY_H
#define PROVIDERFACTORY_H

#include <QObject>

#include "AbstractListeningProvider.h"
#include "WordsStorage.h"
#include "PhrasesStorage.h"

class ProviderFactory : public QObject
{
    Q_OBJECT
    const WordsStorage &wordsStorage;
    const PhrasesStorage &phrasesStorage;
public:
    explicit ProviderFactory (const WordsStorage &wordsStorage,
                              const PhrasesStorage &phrasesStorage,
                              QObject *parent = nullptr);
    QSharedPointer<AbstractListeningProvider> getProvider(
            const ProviderType providerType, const int range) const;

signals:

};

#endif // PROVIDERFACTORY_H
