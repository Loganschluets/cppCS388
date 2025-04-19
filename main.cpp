#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include "constants.hpp"

using namespace std; 

void getCommand(vector<string> arguments);
void getProcesses();
long get_total_cpu_time();
long get_process_cpu_time(pid_t pid);
double get_cpu_usage_percent(pid_t pid, int interval_ms);
void parseCommand(const string& str);


int main() {
    cout << Constants::WELCOME_MESSAGE;
    string command = "";

    // terminal program driven by while loop and leaves on exit
    while (command != Constants::EXIT_COMMAND) {
        cout << Constants::TERMINAL_PROMPT;
        cout << "Type a number: ";
        cin >> command;
        parseCommand(command);
    }
    return 0;
}

void parseCommand(const string& str) {
    vector<string> arguments;
    istringstream iss(str);
    string token;
    
    while (iss >> token) {
        arguments.push_back(token);
    }
    
    getCommand(arguments);
}

//this function will be used to parse the different commands we add
//these conditionals can lead to other functions depending on how much logic the command includes
//any example of this is the conditional for processes, if met this executes the function to show all processes
void getCommand(vector<string> args) {
    const string arg0 = args[0];
    if (arg0 == Constants::ALL_PROCESSES) {
        getProcesses();
    } else if (arg0 == Constants::PROCESS_MEMORY && args.size() > 1) {
        pid_t pid = static_cast<pid_t>(std::stoi(args[1]));
        double d = get_cpu_usage_percent(pid, 100);
        printf("%s memory at %f percent", args[1].c_str(), d);
    } else if (args.size() == 1) {
        if (args[0] == Constants::TCP) {
            getTCPSocketINodes();
        } else if (args[0] == Constants::UDP) {
            getUDPSocketINodes();
        }
    } else if (args.size() == 2) {
        if (args[0] == Constants::TCP) {

        } else if (args[0] == Constants::UDP) {
            
        }
    }
}

void getProcesses() {
    FILE* pipe = popen("ps -e", "r");
    if (!pipe) {
        std::cerr << "Error opening pipe" << std::endl;
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::cout << buffer;
    }

    pclose(pipe);
}

long get_total_cpu_time() {
    ifstream file("/proc/stat");
    string line;
    getline(file, line);
    istringstream ss(line);
    string cpu;
    long total = 0, value;

    ss >> cpu;
    while (ss >> value) {
        total += value;
    }
    return total;
}

long get_process_cpu_time(pid_t pid) {
    string path = "/proc/" + to_string(pid) + "/stat";
    ifstream file(path);
    string line;
    getline(file, line);
    istringstream ss(line);
    vector<string> fields;
    string field;

    while (ss >> field) {
        fields.push_back(field);
    }

    if (fields.size() < 17) return -1;

    long utime = stol(fields[13]);
    long stime = stol(fields[14]);
    return utime + stime;
}

double get_cpu_usage_percent(pid_t pid, int interval_ms) {
    long clk_tck = sysconf(_SC_CLK_TCK);

    long proc_time1 = get_process_cpu_time(pid);
    long total_time1 = get_total_cpu_time();

    std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));

    long proc_time2 = get_process_cpu_time(pid);
    long total_time2 = get_total_cpu_time();

    long proc_delta = proc_time2 - proc_time1;
    long total_delta = total_time2 - total_time1;

    if (total_delta == 0) return 0.0;

    double cpu_usage = (double)proc_delta / total_delta * 100.0;
    return cpu_usage;
}

//inodes are unique identifiers for sockets being used on an os
//this function prints out all inodes for currently open sockets
void getTCPSocketINodes() {
    system("awk 'NR>1{ print $10 }' /proc/net/tcp")
}

void getUDPSocketINodes() {
    system("awk 'NR>1{ print $10 }' /proc/net/udp")
}

void getProcessForTCPINode(string inode) {
    const string script = "./get_inode_process.sh " + inode;
    system(script);
}

