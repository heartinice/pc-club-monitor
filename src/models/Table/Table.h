#ifndef TABLE_H
#define TABLE_H
#include <optional>
#include <string>
using namespace std;
struct Table
{
    int number;
    optional<string> currentClient;
    int totalRevenue = 0;
    int totalOccupiedTime = 0; // в минутах
    optional<int> startTime;   // когда клиент сел, для расчета времени
};

#endif // TABLE_H