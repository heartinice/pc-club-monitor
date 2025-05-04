#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "../models/Event/Event.h"
#include "../config/Config.h"
#include "../config/ConfigLoader.h"
#include "../manager/ClientManager.h"
#include "../manager/TableManager.h"
#include <optional>
using namespace std;


class EventProcessor{
public:
    EventProcessor(Event event);
    void process();
    void handleClientArrived(Event event);
    void handleClientTakeTable(Event event);
    void handleClientLeave(Event event);
    void handleClientWait(Event event);
    static void handleClubClosing();
    Event event;
};

#endif // EVENTPROCESSOR_H