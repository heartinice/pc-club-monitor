#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include "../src/config/ConfigLoader.h"
#include "../src/config/Config.h"
#include "../src/parser/EventParser.h"

std::string create_temp_input_file(const std::vector<std::string>& lines) {
    static int test_counter = 0;
    std::string filename = "temp_test_" + std::to_string(test_counter++) + ".txt";
    std::ofstream outfile(filename);
    for (const auto& line : lines) {
        outfile << line << std::endl;
    }
    outfile.close();
    return filename;
}

void delete_temp_file(const std::string& filename) {
    std::remove(filename.c_str());
}

// Тест для ConfigLoader::loadConfig
void testLoadConfig() {
    std::cout << "--- Тест LoadConfig ---" << std::endl;
    std::vector<std::string> input = {"3", "09:00 19:00", "10"};
    std::string inputFile = create_temp_input_file(input);
    ConfigLoader configloader(inputFile);
    EventParser eventparser;
    configloader.base_info();
    eventparser.parseEvents(inputFile);
    Config config = ConfigLoader::getConfig();
    delete_temp_file(inputFile);

    if (config.tables == 3 && ConfigLoader::StrFormatTime(config.st_time) == "09:00" &&
        ConfigLoader::StrFormatTime(config.fin_time) == "19:00" && config.coast == 10) {
        std::cout << "Результат: PASS" << std::endl;
    } else {
        std::cout << "Результат: FAIL" << std::endl;
        std::cout << "  Ожидалось: 3, 09:00, 19:00, 10" << std::endl;
        std::cout << "  Получено:  " << config.tables << ", " << ConfigLoader::StrFormatTime(config.st_time)
                  << ", " << ConfigLoader::StrFormatTime(config.fin_time) << ", " << config.coast << std::endl;
    }
}

int main() {
    testLoadConfig();
    return 0;
}