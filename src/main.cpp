#include "view/view.h"
#include "middleman.h"

int main(int argc, char **argv)
{
    // Create application object
    View view;
    std::vector<AI*> aiList;
    std::vector<AIInfo> aiInfos;
    Middleman middleman(aiList, aiInfos);
    middleman.addView(&view);
    middleman.startGame();
    return 0;
}