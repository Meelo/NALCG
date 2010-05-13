#include "middleman.h"
#include "enduser.h"
#include "view/view.h"
#include "ais/aiinfo.h"
#include "ais/ai.h"
#include "ais/daniel/aiadapter.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>

void createRemoteConfig(const std::string& remoteConf);

int main(int argc, char **argv)
{
    struct stat stFileInfo;
    const std::string remoteConf = "../data/remote.cfg";

    // stat returns 0 when file info can be accessed.
    if (stat(remoteConf.c_str(), &stFileInfo))
    {
        createRemoteConfig(remoteConf);

    }
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

void createRemoteConfig(const std::string& remoteConf)
{
    std::string name;
    std::string address;
    int port;

    std::cout   << "Insert your player name."           << std::endl;
    std::cin    >> name;
    std::cout   << "Insert remote server address."
                << " (dzarg.mine.nu is public server)"  << std::endl;
    std::cin    >> address;
    std::cout   << "Insert remote server port."
                << " (6668 if you use dzarg.mine.nu)"   << std::endl;
    std::cin    >> port;

    std::ofstream confFile;
    confFile.open(remoteConf.c_str());
    confFile << "name="     << name << "\n";
    confFile << "address="  << address << "\n";
    confFile << "port="     << port << "\n";
    confFile.close();

    std::cout << "Name, address and port has been written to "
        << remoteConf << "\nIf the data was invalid, you can change it by "
        << "directly accessing the file or by removing the file and "
        << "then re-running the binary." << std::endl;
}
