#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Config.h"
#include <format>
using namespace std;

class ConfigLoader {
public:
    ConfigLoader(const string& filename);  
    void base_info();  
    static int TimeToInt(const string& time);  
    static const Config& getConfig(); 
    static string StrFormatTime(int totalMinutes);

private:
    string filename;  
    static Config config;  
    static bool loaded;  
    static bool load_failed;
};

#endif // CONFIGLOADER_H
