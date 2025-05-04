#include "Application.h"

Application::Application(const std::string& filename)
    : configloader(filename), filename(filename) {}

void Application::run() {
    configloader.base_info();
    eventparser.parseEvents(filename);
    EventProcessor::handleClubClosing();
    TableManager::Get().PrintReport();
}
