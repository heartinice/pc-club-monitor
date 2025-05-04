#include "ClientManager.h"

ClientManager& ClientManager::Get() {
    static ClientManager instance;
    return instance;
}

bool ClientManager::AddClient(const string& name) {
    if (clientsInClub.count(name)) return false; 
    clientsInClub.insert({name, Client{name, nullopt, nullopt, nullopt}});
    return true;
}


bool ClientManager::HasClient(const string& name) const {
    return clientsInClub.count(name);
}

bool ClientManager::IsAtTable(const string& name) const {
    auto it = clientsInClub.find(name);
    return it != clientsInClub.end() && it->second.table != nullopt;
}

bool ClientManager::AssignTable(const string& name, int table, int currentTime) {
    auto it = clientsInClub.find(name);
    if (it == clientsInClub.end()) return false;

    it->second.table = table;
    it->second.st_time = currentTime;
    return true;
}

bool ClientManager::Leave(const std::string& name) {
    auto queueIt = std::find(queue.begin(), queue.end(), name);
    if (queueIt != queue.end()) {
        queue.erase(queueIt);
    }
    return clientsInClub.erase(name) > 0;
}


bool ClientManager::AddToQueue(const std::string& name) {
    if (std::find(queue.begin(), queue.end(), name) != queue.end())
        return false;
    // const Config& config = ConfigLoader::getConfig();
    // if (queue.size() < config.tables)
    queue.push_back(name);
    return true;
}

bool ClientManager::QueueMax() const {
    const Config& config = ConfigLoader::getConfig();
    return queue.size() > config.tables;
}

bool ClientManager::HasWaitingClients() const {
    return !queue.empty();
}

std::optional<std::string> ClientManager::PopFromQueue() {
    if (queue.empty()) return std::nullopt;
    std::string next = queue.front();
    queue.pop_front();
    return next;
}

std::vector<std::string> ClientManager::GetRemainingClients() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : clientsInClub) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::optional<int64_t> ClientManager::GetClientStartTime(const std::string& name) const {
    auto it = clientsInClub.find(name);
    if (it != clientsInClub.end()) {
        return it->second.st_time;
    }
    return std::nullopt;
}
