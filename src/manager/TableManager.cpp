#include "TableManager.h"


TableManager& TableManager::Get() {
    static TableManager instance;

    if (!instance.initialized) {
        const Config& config = ConfigLoader::getConfig();
        instance.Initialize(config.tables, config.coast);
    }

    return instance;
}

void TableManager::Initialize(int count, int rate) {
    if (initialized) return;

    hourRate = rate;
    tables.reserve(count);
    for (int i = 0; i < count; ++i) {
        Table t;
        t.number = i + 1;
        tables.push_back(t);
    }

    initialized = true;
}

bool TableManager::IsFree(optional<int> tableNumber) const {
    if (tableNumber == nullopt || *tableNumber <= 0 || *tableNumber > static_cast<int>(tables.size()))
        return false;

    const Table& tab = tables[*tableNumber - 1];
    return (!tab.currentClient.has_value());
}

bool TableManager::AssignClient(optional<int> tableNumber, const string& clientName, int currentTime) {
    if (tableNumber == nullopt || *tableNumber <= 0 || *tableNumber > static_cast<int>(tables.size()))
        return false;

    Table& table = tables[*tableNumber - 1];

    if (table.currentClient.has_value()) {
        return false; 
    }

    table.currentClient = clientName;
    table.startTime = currentTime;
    ClientManager::Get().AssignTable(clientName, *tableNumber,currentTime);
    return true;
}

optional<int> TableManager::FindFreeTable() const{
    for(const Table& tab: tables){
        if (!tab.currentClient.has_value()){
            return tab.number;
        }
    }
    return nullopt;
}
void TableManager::ClientLeave(const string& name, int time) {
    for (Table& tab : tables) {
        if (tab.currentClient.has_value() && tab.currentClient.value() == name) {
            if (!tab.startTime.has_value()) return;

            int duration = time - tab.startTime.value();
            if (duration <= 0) return;

            int hoursToCharge = (duration + 59) / 60;  
            int payment = hoursToCharge * hourRate;

            tab.totalOccupiedTime += duration;
            tab.totalRevenue += payment;

            tab.currentClient = nullopt;
            tab.startTime = nullopt;

            return;  
        }
    }
}


void TableManager::PrintReport() {
    for (const Table& table : tables) {
        std::cout << table.number << " "
                  << table.totalRevenue << " "
                  << ConfigLoader::StrFormatTime(table.totalOccupiedTime) << std::endl;
    }
}

