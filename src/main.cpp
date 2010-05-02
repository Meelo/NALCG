#include "view/view.h"
#include "middleman.h"
#include "ais/daniel/aiadapter.h"

int main(int argc, char **argv)
{
    View view;
    std::vector<AI*> aiList;
    std::vector<AIInfo> aiInfos;
    aiList.push_back(new AIAdapter());
    aiInfos.push_back(AIInfo("AI (Daniel)", "Daniel's supreme AI"));
    Middleman middleman(aiList, aiInfos);
    middleman.addView(&view);
    middleman.startGame();
    while (true);
    return 0;
}