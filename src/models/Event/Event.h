#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <optional>
using namespace std;

struct Event {
    int time;
    int id;
    string name;
    optional<int> tables;
    // Конструктор для удобного создания объекта события
    Event(int t, int i, string n, optional<int> table) : time(t), id(i), name(n), tables(table) {}
};

#endif // EVENT_H