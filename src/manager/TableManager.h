#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include "../config/ConfigLoader.h"
#include "../config/Config.h"
#include <cmath>
#include <stdexcept>
#include "../models/Table/Table.h"
#include "ClientManager.h"
#include <vector>
#include <optional>
#include <string>
using namespace std;

class TableManager {
public:
    static TableManager& Get();

    bool IsFree(optional<int> tableNumber) const;
    std::optional<int> FindFreeTable() const;
    bool AssignClient(optional<int> tableNumber, const std::string& clientName, int currentTime);
    std::optional<std::string> FreeTable(int tableNumber, int currentTime);
    void ForceFreeAll(int currentTime, ClientManager& clientManager);
    void ClientLeave(const string& name, int time);

    const std::vector<Table>& GetTables() const;
    void PrintReport();
    std::vector<Table> tables;

private:
    
    TableManager() = default;
    TableManager(const TableManager&) = delete;
    TableManager& operator=(const TableManager&) = delete;

    void Initialize(int count, int rate);
    const Config& config = ConfigLoader::getConfig();
    
    int hourRate = config.coast;
    bool initialized = false;
};

#endif // TABLEMANAGER_H
