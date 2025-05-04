#include <string>
#include <iostream>
#include "Application.h"
using namespace std;

int main(int argc, char* argv[]){
    if (argc != 2){
        cerr << "Пересмотрите выбор аргументов";
    }
    Application(argv[1]).run();
    return 0;
}