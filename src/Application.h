#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include "config/Config.h"
#include "config/ConfigLoader.h"
#include "parser/EventParser.h"
#include "processor/EventProcessor.h"

class Application {
public:
    Application(const std::string& filename); 
    void run();

private:
    std::string filename;
    ConfigLoader configloader;
    Config config;
    EventParser eventparser;

};

#endif // APPLICATION_H
