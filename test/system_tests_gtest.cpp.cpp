#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <regex>
#include <iomanip>

namespace fs = std::filesystem;

fs::path project_root = fs::absolute("/Users/macbook14/Projects/pc-club-monitor_yadro");


std::string run_program(const std::string& input_file_path, bool expect_failure = false) {
    fs::path absolute_input_path = project_root / input_file_path;
    std::string command = fs::absolute("./start").string() + " " + absolute_input_path.string() + (expect_failure ? " 2>&1" : "");
    std::string result = "";
    char buffer[128];
#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
#else
    FILE* pipe = popen(command.c_str(), "r");
#endif
    if (pipe) {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
    } else {
        std::cerr << "Ошибка при запуске программы." << std::endl;
    }
    return result;
}

std::string read_file(const std::string& file_path) {
    std::ifstream file_stream((project_root / file_path).string());
    if (!file_stream.is_open()) {
        std::cerr << "Не удалось открыть файл: " << file_path << std::endl;
        return ""; 
    }
    std::stringstream ss;
    ss << file_stream.rdbuf();
    return ss.str();
}

bool check_output_contains(const std::string& output, const std::string& expected) {
    return output.find(expected) != std::string::npos;
}

class ConciseListener : public ::testing::EmptyTestEventListener {
    void OnTestStart(const ::testing::TestInfo& test_info) override {
        std::cout << "* " << test_info.name() << ": ";
    }

    void OnTestEnd(const ::testing::TestInfo& test_info) override {
        if (test_info.result()->Passed()) {
            std::cout << "[OK]" << std::endl;
        } else {
            std::cout << "[FAIL]" << std::endl;
        }
    }
};


TEST(SystemTests, ValidInput) {
    std::cout << "* ValidInput: ";
    std::string input_file = "test/configs/valid_input.txt";
    std::string expected_output_file = "test/configs/valid_output.txt";
    std::string actual_output = run_program(input_file);
    std::string expected_output = read_file(expected_output_file);
    ASSERT_EQ(actual_output, expected_output);
    std::cout << "[OK]" << std::endl;
}

TEST(SystemTests, InvalidFormatFirstLine) {
    std::cout << "* InvalidFormatFirstLine: ";
    std::string input_file = "test/configs/invalid_format_first_line.txt";
    std::string actual_output = run_program(input_file, true);
    ASSERT_TRUE(check_output_contains(actual_output, "некорректный формат количества столов"));
    std::cout << "[OK]" << std::endl;
}

TEST(SystemTests, InvalidFormatTime) {
    std::cout << "* InvalidFormatTime: ";
    std::string input_file = "test/configs/invalid_format_time.txt";
    std::string actual_output = run_program(input_file, true);
    ASSERT_TRUE(check_output_contains(actual_output, "некорректный формат времени"));
    std::cout << "[OK]" << std::endl;
}

// Тест на корректное прибытие клиента
TEST(SystemTests, ArriveClient) {
    std::cout << "* ArriveClient: ";
    std::string input_file = "test/configs/arrive_client.txt";
    std::string expected_output_file = "test/configs/expected_arrive_client.txt";
    std::string actual_output = run_program(input_file);
    std::string expected_output = read_file(expected_output_file);
    ASSERT_EQ(actual_output, expected_output);
    std::cout << "[OK]" << std::endl;
}

// Тест на попытку прибытия уже существующего клиента
TEST(SystemTests, ArriveExistingClient) {
    std::cout << "* ArriveExistingClient: ";
    std::string input_file = "test/configs/arrive_existing_client.txt";
    std::string actual_output = run_program(input_file);
    ASSERT_TRUE(check_output_contains(actual_output, "10:10 13 YouShallNotPass"));
    std::cout << "[OK]" << std::endl;
}

// Тест на попытку прибытия клиента в нерабочее время
TEST(SystemTests, ArriveBeforeOpen) {
    std::cout << "* ArriveBeforeOpen: ";
    std::string input_file = "test/configs/arrive_before_open.txt";
    std::string actual_output = run_program(input_file);
    ASSERT_TRUE(check_output_contains(actual_output, "09:55 13 NotOpenYet"));
    std::cout << "[OK]" << std::endl;
}

// Тест на корректную посадку клиента за стол
TEST(SystemTests, SeatClient) {
    std::cout << "* SeatClient: ";
    std::string input_file = "test/configs/seat_client.txt";
    std::string expected_output_file = "test/configs/expected_seat_client.txt";
    std::string actual_output = run_program(input_file);
    std::string expected_output = read_file(expected_output_file);
    ASSERT_EQ(actual_output, expected_output);
    std::cout << "[OK]" << std::endl;
}

// Тест на попытку посадки клиента за занятый стол
TEST(SystemTests, SeatOccupiedTable) {
    std::cout << "* SeatOccupiedTable: ";
    std::string input_file = "test/configs/seat_occupied_table.txt";
    std::string actual_output = run_program(input_file);
    ASSERT_TRUE(check_output_contains(actual_output, "10:15 13 PlaceIsBusy"));
    std::cout << "[OK]" << std::endl;
}

// Тест на попытку посадки несуществующего клиента за стол
TEST(SystemTests, SeatUnknownClient) {
    std::cout << "* SeatUnknownClient: ";
    std::string input_file = "test/configs/seat_unknown_client.txt";
    std::string actual_output = run_program(input_file);
    ASSERT_TRUE(check_output_contains(actual_output, "10:05 13 ClientUnknown"));
    std::cout << "[OK]" << std::endl;
}

// Тест на корректный уход клиента
TEST(SystemTests, LeaveClient) {
    std::cout << "* LeaveClient: ";
    std::string input_file = "test/configs/leave_client.txt";
    std::string expected_output_file = "test/configs/expected_leave_client.txt";
    std::string actual_output = run_program(input_file);
    std::string expected_output = read_file(expected_output_file);
    ASSERT_EQ(actual_output, expected_output);
    std::cout << "[OK]" << std::endl;
}

// Тест на попытку ухода несуществующего клиента
TEST(SystemTests, LeaveUnknownClient) {
    std::cout << "* LeaveUnknownClient: ";
    std::string input_file = "test/configs/leave_unknown_client.txt";
    std::string actual_output = run_program(input_file);
    ASSERT_TRUE(check_output_contains(actual_output, "10:05 13 ClientUnknown"));
    std::cout << "[OK]" << std::endl;
}

// Тест на корректное ожидание клиента
TEST(SystemTests, WaitClient) {
    std::cout << "* WaitClient: ";
    std::string input_file = "test/configs/wait_client.txt";
    std::string expected_output_file = "test/configs/expected_wait_client.txt";
    std::string actual_output = run_program(input_file);
    std::string expected_output = read_file(expected_output_file);
    ASSERT_EQ(actual_output, expected_output);
    std::cout << "[OK]" << std::endl;
}

// Тест на событие "ICanWaitNoLonger!"
TEST(SystemTests, WaitWhenSpace) {
    std::cout << "* WaitWhenSpace: ";
    std::string input_file = "test/configs/wait_when_space.txt";
    std::string actual_output = run_program(input_file);
    ASSERT_TRUE(check_output_contains(actual_output, "10:10 13 ICanWaitNoLonger!"));
    std::cout << "[OK]" << std::endl;
}


// Тест на уход всех клиентов в конце рабочего дня
TEST(SystemTests, LeaveEndOfDayMultiple) {
    std::cout << "* LeaveEndOfDayMultiple: ";
    std::string input_file = "test/configs/leave_end_of_day_multiple.txt";
    std::string expected_output_file = "test/configs/expected_leave_end_of_day_multiple.txt";
    std::string actual_output = run_program(input_file);
    std::string expected_output = read_file(expected_output_file);
    ASSERT_EQ(actual_output, expected_output);
    std::cout << "[OK]" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::UnitTest::GetInstance()->listeners().Append(new ConciseListener);
    return RUN_ALL_TESTS();
}