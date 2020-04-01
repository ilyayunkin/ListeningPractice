#ifndef PROVIDERFACTORY_H
#define PROVIDERFACTORY_H

#include <QObject>

#include "AbstractListeningProvider.h"
#include "WordsStorage.h"
#include "PhrasesStorage.h"

class ProviderFactory : public QObject
{
    Q_OBJECT
    WordsStorage &wordsStorage;
    PhrasesStorage &phrasesStorage;
public:
    explicit ProviderFactory (WordsStorage &wordsStorage,
                              PhrasesStorage &phrasesStorage,
                              QObject *parent = nullptr);
    QSharedPointer<AbstractListeningProvider> getProvider(ProviderType providerType, int range);

signals:

};

#endif // PROVIDERFACTORY_H
