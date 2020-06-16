#ifndef GAMECOUNTERS_H
#define GAMECOUNTERS_H

#include "ProviderType.h"

#include <array>

struct GameCounters
{
    std::array<int, PROVIDERS_COUNT> answersCnt;
    std::array<int, PROVIDERS_COUNT> positiveCnt;
    int positiveCommonCnt;
    int answersCommonCnt;
    int negativeCommonCnt;

    GameCounters() :
        answersCnt{{0}},
        positiveCnt{{0}},
        positiveCommonCnt(0),
        answersCommonCnt(0),
        negativeCommonCnt(0)
    {}
};

#endif // GAMECOUNTERS_H
