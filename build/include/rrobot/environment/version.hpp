#ifndef VERSION_H
#define VERSION_H

#include <string>

using namespace std;

namespace rrobot {
    class RrVersion {
        public:
            string getVersion() {return "2.0.0";}
            int getVersonMajor() {return 2;}
            int getVersonMinor() {return 0;}
            int getVersionPath() {return 0;}
            string getAppName() {return "rrobot";}
    };
}

#endif
