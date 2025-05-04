#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <optional>
using namespace std;

struct Client {
    string name;
    optional<int> table;
    optional<int> st_time;
    optional<int> fin_time;
    // Конструктор для удобного создания объекта события
    Client(string n, optional<int> tab, optional<int> st, optional<int> fn) : name(n), table(tab), st_time(st), fin_time(fn) {};
};

#endif // CLIENT_H