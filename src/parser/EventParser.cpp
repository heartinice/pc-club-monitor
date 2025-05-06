#include "EventParser.h"

EventParser::EventParser() {}

void EventParser::parseEvents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        exit(1);
    }

    std::string line;

    for (int i = 0; i < 3 && std::getline(file, line); ++i) {
    }
    optional<std::string> previousEventTimeStr;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string time;
        int id;
        std::string name;
        std::optional<int> table_num;
        int temp;

        if (iss >> time >> id >> name) {
            if (iss >> temp) {
                table_num = temp;
            }
            if (!(Validator::IsValidTimeFormat(time) &&
                  Validator::IsTimeSequential(time,previousEventTimeStr) &&
                  Validator::IsValidId(id) &&
                  Validator::isValidName(name) &&
                  Validator::IsValidNumTables(table_num))) {
                std::cerr << "Невалидная строка: " << line << std::endl;
                exit(1);
            }
            cout << line << endl; 
            Event event(ConfigLoader::TimeToInt(time), id, name, table_num);
            EventProcessor processor(event);
            processor.process();
            previousEventTimeStr = time;
        } else {
            std::cerr << "Ошибка чтения строки: " << line << std::endl;
            cout << line << endl;
            exit(1);
        }
    }
}
