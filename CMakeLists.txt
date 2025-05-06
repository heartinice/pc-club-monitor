# # cmake_minimum_required(VERSION 3.10)
# # project(PC_Club_Monitor_Yadro)

# # set(CMAKE_CXX_STANDARD 17)
# # set(CMAKE_CXX_STANDARD_REQUIRED True)

# # set(SOURCES
# #     src/main.cpp
# #     src/application.cpp
# #     src/config/ConfigLoader.cpp
# #     src/parser/EventParser.cpp
# #     src/processor/EventProcessor.cpp
# #     src/parser/Validator.cpp
# #     src/manager/ClientManager.cpp
# #     src/manager/TableManager.cpp
# # )

# # add_executable(start ${SOURCES})

# # include_directories(src)

# cmake_minimum_required(VERSION 3.10)
# project(PC_Club_Monitor_Yadro)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Основной исполняемый файл (название: start)
# set(SOURCES
# src/main.cpp
#     src/application.cpp
#     src/config/ConfigLoader.cpp
#     src/parser/EventParser.cpp
#     src/processor/EventProcessor.cpp
#     src/parser/Validator.cpp
#     src/manager/ClientManager.cpp
#     src/manager/TableManager.cpp
# )

# add_executable(start ${SOURCES})
# target_include_directories(start PRIVATE src)

# # Отдельный исполняемый файл для тестов (название: run_tests)
# set(TEST_SOURCES
#     test/unit_tests.cpp
#     src/config/ConfigLoader.cpp
#     src/parser/EventParser.cpp
#     src/processor/EventProcessor.cpp
#     src/parser/Validator.cpp
#     src/manager/ClientManager.cpp
#     src/manager/TableManager.cpp
# )

# add_executable(run_tests ${TEST_SOURCES})
# target_include_directories(run_tests PRIVATE src)

cmake_minimum_required(VERSION 3.11)
project(PC_Club_Monitor_Yadro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Основной исполняемый файл (название: start)
set(SOURCES
    src/main.cpp
    src/application.cpp
    src/config/ConfigLoader.cpp
    src/parser/EventParser.cpp
    src/processor/EventProcessor.cpp
    src/parser/Validator.cpp
    src/manager/ClientManager.cpp
    src/manager/TableManager.cpp
)

add_executable(start ${SOURCES})
target_include_directories(start PRIVATE src)

# Юнит-тесты (название: run_unit_tests)
set(UNIT_TEST_SOURCES
    test/unit_tests.cpp
    src/config/ConfigLoader.cpp
    src/parser/EventParser.cpp
    src/processor/EventProcessor.cpp
    src/parser/Validator.cpp
    src/manager/ClientManager.cpp
    src/manager/TableManager.cpp
)

add_executable(run_unit_tests ${UNIT_TEST_SOURCES})
target_include_directories(run_unit_tests PRIVATE src)

# Интеграция Google Test для системных тестов (название: run_system_tests)
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0 # Или другая нужная версия
)

FetchContent_MakeAvailable(googletest)

set(SYSTEM_TEST_SOURCES
    test/system_tests_gtest.cpp
    src/config/ConfigLoader.cpp
    src/parser/EventParser.cpp
    src/processor/EventProcessor.cpp
    src/parser/Validator.cpp
    src/manager/ClientManager.cpp
    src/manager/TableManager.cpp
)

add_executable(run_system_tests ${SYSTEM_TEST_SOURCES})
target_include_directories(run_system_tests PRIVATE src)
target_link_libraries(run_system_tests gtest_main gtest)

# Добавляем цели для запуска тестов через CTest
enable_testing()
add_test(NAME unit_tests COMMAND run_unit_tests)
add_test(NAME system_tests COMMAND run_system_tests)
# Добавляем зависимости (опционально)
add_dependencies(run_unit_tests start)
add_dependencies(run_system_tests start)