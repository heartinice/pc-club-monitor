#ifndef EVENTPARSER_H
#define EVENTPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include "../models/Event/Event.h"
#include "../processor/EventProcessor.h"
#include "Validator.h"

class EventParser {
public:
    EventParser();
    void parseEvents(const std::string& filename);

};

#endif // EVENTPARSER_H
