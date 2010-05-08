#include "middleman.h"
#include "enduser.h"
#include "view/view.h"
#include "ais/aiinfo.h"
#include "ais/ai.h"
#include "ais/daniel/aiadapter.h"

#include <vector>
#include <string>

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
    return 0;
}
