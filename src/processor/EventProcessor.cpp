#include "EventProcessor.h"

EventProcessor::EventProcessor(Event event): event(event) {};


void EventProcessor::process(){
    switch(event.id){
        case 1:
            handleClientArrived(event);
            break;
        case 2:
            handleClientTakeTable(event);
            break;
        case 3:
            handleClientWait(event);
            break;
        case 4:
            handleClientLeave(event);
            break;

        default:
            std::cerr << "Неизвестный ID события: " << event.id << std::endl;
            break;
    }
}

void EventProcessor::handleClientArrived(Event event){
    const Config& config = ConfigLoader::getConfig();
    if (ClientManager::Get().HasClient(event.name)){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 13 << " "
          << "YouShallNotPass" << std::endl;
    }
    else if(event.time < config.st_time || event.time > config.fin_time){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 13 << " "
          << "NotOpenYet" << std::endl;
    }
    
    else{
        ClientManager::Get().AddClient(event.name);
    }
}

void EventProcessor::handleClientTakeTable(Event event) {
    if (!ClientManager::Get().HasClient(event.name)) {
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                  << 13 << " "
                  << "ClientUnknown" << std::endl;
        return;
    }

    if (TableManager::Get().IsFree(event.tables)) {
        
        if (ClientManager::Get().IsAtTable(event.name)) {
            optional<int> currentTable = ClientManager::Get().GetClientTable(event.name);
            TableManager::Get().ClientLeave(event.name,event.time);
        }

        TableManager::Get().AssignClient(event.tables, event.name, event.time);
    } else {
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                  << 13 << " "
                  << "PlaceIsBusy" << std::endl;
    }
}

void EventProcessor::handleClientLeave(Event event) {
    if (!ClientManager::Get().HasClient(event.name)) {
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                  << 13 << " "
                  << "ClientUnknown" << std::endl;
        return;
    }

    
    TableManager::Get().ClientLeave(event.name, event.time);
    ClientManager::Get().Leave(event.name);


    auto nextClientOpt = ClientManager::Get().PopFromQueue();
    if (nextClientOpt.has_value()) {

        auto freeTableOpt = TableManager::Get().FindFreeTable();
        if (freeTableOpt.has_value()) {

            bool assigned = TableManager::Get().AssignClient(freeTableOpt, nextClientOpt.value(), event.time);
            if (assigned) {
                std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                          << 12 << " "
                          << nextClientOpt.value() << " "
                          << freeTableOpt.value() << std::endl;
            } 
        } else {
            ClientManager::Get().AddToQueue(nextClientOpt.value());
        }
    } 
}


void EventProcessor::handleClientWait(Event event) {
    const Config& config = ConfigLoader::getConfig();

    if (!ClientManager::Get().HasClient(event.name)) {
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                  << 13 << " "
                  << "ClientUnknown" << std::endl;
        return;
    }

    else if (TableManager::Get().FindFreeTable().has_value()) {
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                  << 13 << " "
                  << "ICanWaitNoLonger!" << std::endl;
        return;
    }

    else if (ClientManager::Get().QueueSize() > config.tables) {
        ClientManager::Get().Leave(event.name); 
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                  << 11 << " "
                  << event.name << std::endl;
        return;
    }

    else if (!ClientManager::Get().IsInQueue(event.name)) {
        ClientManager::Get().AddToQueue(event.name);
    }
}


void EventProcessor::handleClubClosing() {
    const Config& config = ConfigLoader::getConfig();
    std::string closingTime = ConfigLoader::StrFormatTime(config.fin_time);
    std::vector<std::string> remainingClients = ClientManager::Get().GetRemainingClients();

    for (const std::string& name : remainingClients) {
        std::cout << closingTime << " 11 " << name << std::endl;
        TableManager::Get().ClientLeave(name, ConfigLoader::TimeToInt(closingTime));
        ClientManager::Get().Leave(name);
    }
    cout << ConfigLoader::StrFormatTime(config.fin_time) << endl;
}
