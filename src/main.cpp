#include "view/view.h"

int main(int argc, char **argv)
{
    // Create application object
    View view;
    try {
        view.go();
    } catch( Exception& e ) {
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
    }

    return 0;
}