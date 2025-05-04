cmake_minimum_required(VERSION 3.10)
project(PC_Club_Monitor_Yadro)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем исходные файлы
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

# Добавляем исполнимый файл
add_executable(start ${SOURCES})

# Включаем необходимые директории для заголовочных файлов
include_directories(src)
