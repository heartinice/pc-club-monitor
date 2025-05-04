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

    // Добавление нового клиента
    bool AddClient(const std::string& name);

    // Проверка, есть ли клиент в клубе
    bool HasClient(const std::string& name) const;

    // Проверка, сидит ли клиент за столом
    bool IsAtTable(const std::string& name) const;

    // Назначение клиенту стола и установка времени
    bool AssignTable(const std::string& name, int table, int currentTime);

    // Удаление клиента из клуба (и со стола)
    bool Leave(const std::string& name);

    // Добавление в очередь
    bool AddToQueue(const std::string& name);

    // Проверка заполненности очереди
    bool QueueMax() const;

    // Проверка, есть ли кто-то в очереди
    bool HasWaitingClients() const;

    // Извлечение первого клиента из очереди
    std::optional<std::string> PopFromQueue();

    // Получение списка клиентов, оставшихся в клубе
    std::vector<std::string> GetRemainingClients() const;

    // Время начала использования стола
    std::optional<int64_t> GetClientStartTime(const std::string& name) const;

private:
    ClientManager() = default; // приватный конструктор для Singleton

    std::unordered_map<string, Client> clientsInClub;
    std::deque<std::string> queue;
};

#endif // CLIENTMANAGER_H
