#include "EventParser.h"

EventParser::EventParser() {}

void EventParser::parseEvents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        exit(1);
    }

    std::string line;

    // Пропустить первые 3 строки
    for (int i = 0; i < 3 && std::getline(file, line); ++i) {
        // Пропуск конфигурации
    }

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
                  Validator::IsValidId(id) &&
                  Validator::isValidName(name) &&
                  Validator::IsValidNumTables(table_num))) {
                std::cerr << "Невалидная строка: " << line << std::endl;
                exit(1);
            }
            cout << line << endl; //обдумать тут вывод строки 
            Event event(ConfigLoader::TimeToInt(time), id, name, table_num);
            EventProcessor processor(event);
            processor.process();
        } else {
            std::cerr << "Ошибка чтения строки: " << line << std::endl;
            cout << line << endl;
            exit(1);
        }
    }
}
