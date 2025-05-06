#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <string>
#include <unordered_map>
#include <deque>
#include <vector>
#include <optional>
#include <algorithm>
#include "../models/Client/Client.h"
#include "../config/ConfigLoader.h"
#include "../config/Config.h"

class ClientManager {
public:
    static ClientManager& Get();

    bool AddClient(const std::string& name);
    bool HasClient(const std::string& name) const;
    bool IsAtTable(const std::string& name) const;
    bool AssignTable(const std::string& name, int table, int currentTime);
    bool Leave(const std::string& name);
    bool AddToQueue(const std::string& name);
    int QueueSize() const;
    bool HasWaitingClients() const;
    std::optional<std::string> PopFromQueue();
    std::vector<std::string> GetRemainingClients() const;
    std::optional<int64_t> GetClientStartTime(const std::string& name) const;
    optional<int> GetClientTable(const string& name)const;
    bool IsInQueue(const std::string& clientName);

private:
    ClientManager() = default; 

    std::unordered_map<string, Client> clientsInClub;
    std::deque<std::string> queue;
};

#endif // CLIENTMANAGER_H
