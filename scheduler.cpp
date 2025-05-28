#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <csignal>
#include <ctime>
#include <map>
#include "task.hpp"

using namespace std;

bool running = true;

void signalHandler(int signum) {
    running = false;
    cout << "Scheduler shutting down..." << endl;
}

struct Task {
    int interval;
    string taskId;
    time_t lastRun;
};

vector<Task> loadTasks(const string& filename) {
    vector<Task> tasks;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        int interval;
        string id;
        if (ss >> interval >> id) {
            tasks.push_back({interval, id, 0});
        }
    }
    return tasks;
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    system("mkdir -p logs"); // Ensure logs folder exists

    vector<Task> tasks = loadTasks("tasks.txt");

    cout << "Advanced task scheduler running in background..." << endl;

    while (running) {
        time_t now = time(nullptr);

        for (auto& task : tasks) {
            if (difftime(now, task.lastRun) >= task.interval) {
                pid_t pid = fork();
                if (pid == 0) {
                    // Child: run the task
                    runTask(task.taskId);
                    exit(0);
                } else if (pid > 0) {
                    // Parent
                    task.lastRun = now;
                }
            }
        }

        sleep(1);
    }

    cout << "Scheduler exited cleanly." << endl;
    return 0;
}
