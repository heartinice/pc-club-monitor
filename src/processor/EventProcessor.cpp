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
    if(event.time < config.st_time || event.time > config.fin_time){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 13 << " "
          << "NotOpenYet" << std::endl;
    }
    else if (ClientManager::Get().HasClient(event.name)){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 13 << " "
          << "YouShallNotPass" << std::endl;
    }
    else{
        ClientManager::Get().AddClient(event.name);
    }
}

 void EventProcessor::handleClientTakeTable(Event event){
    if(!(ClientManager::Get().HasClient(event.name))){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 13 << " "
          << "ClientUnknown" << std::endl;
    }
    if (TableManager::Get().IsFree(event.tables)){
        TableManager::Get().AssignClient(event.tables,event.name,event.time);
    }
    else{
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
            TableManager::Get().AssignClient(freeTableOpt.value(), nextClientOpt.value(), event.time);
            std::cout << ConfigLoader::StrFormatTime(event.time) << " "
                      << 12 << " "
                      << nextClientOpt.value() << " "
                      << freeTableOpt.value() << std::endl;
        }
    }
}


void EventProcessor::handleClientWait(Event event){
    if (TableManager::Get().FindFreeTable().has_value()){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 13 << " "
          << "ICanWaitNoLonger!" << std::endl;
    }
    if (ClientManager::Get().QueueMax()){
        std::cout << ConfigLoader::StrFormatTime(event.time) << " "
          << 11 << " "
          << event.name << std::endl;
    }
    else{
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
