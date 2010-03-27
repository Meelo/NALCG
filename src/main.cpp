#include <boost/thread.hpp>

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
    try {
        view.go();
    } catch( Exception& e ) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }

    return 0;
}