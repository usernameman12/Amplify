#include "task.hpp"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

void logToFile(const std::string& filename, const std::string& message) {
    std::ofstream file("logs/" + filename, std::ios::app);
    file << message << std::endl;
}

void runTask(const std::string& taskId) {
    if (taskId == "logtime") {
        time_t now = time(nullptr);
        logToFile("logtime.log", std::string("Time: ") + ctime(&now));
    } else if (taskId == "customtask") {
        int result = system("echo 'Running task' >> logs/customtask.log");
        logToFile("customtask.log", "Custom task completed: " + std::to_string(result));
    } else {
        logToFile("error.log", "Unknown task ID: " + taskId);
    }
}
