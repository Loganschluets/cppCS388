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
#include <stdexcept>
#include "constants.hpp"

using namespace std; 

void getCommand(vector<string> arguments);
void getProcesses();
long get_total_cpu_time();
long get_process_cpu_time(pid_t pid);
double get_cpu_usage_percent(pid_t pid, int interval_ms);
void parseCommand(const string& str);
void get_TCP_socket_inodes();
void get_UDP_socket_inodes();
void get_process_for_TCP_inode(string inode);
double get_memory_usage_percent();
long get_idle_cpu_time(); 
double get_cpu_usage_percent(int interval_ms);


int main() {
    cout << Constants::WELCOME_MESSAGE;
    string command = "";

    // terminal program driven by while loop and leaves on exit
    while (command != Constants::EXIT_COMMAND) {
        cout << Constants::TERMINAL_PROMPT;
        cout << "Enter Command: ";
        cin >> command;
        parseCommand(command);
        cout << "\n";
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
    if (arg0 == Constants::PROCESSES) {
        if(args.size() == 1) {
            getProcesses();
        } else if (args[1] == Constants::USER){
            
        }
    } else if (arg0 == Constants::PROCESS_MEMORY && args.size() > 1) {
        try {
            pid_t pid = static_cast<pid_t>(std::stoi(args[1]));
            cout << "Checking CPU usage for PID: " << pid << endl;
            double d = get_cpu_usage_percent(pid, 100);
            cout << args[1] << " memory at " << d << " percent" << endl;
        } catch (const std::exception& e) {
            cerr << "Some other error: " << e.what() << std::endl;
        }
    } else if (args.size() == 1) {
        if (args[0] == Constants::TCP) {
            get_TCP_socket_inodes();
        } else if (args[0] == Constants::UDP) {
            get_UDP_socket_inodes();
        } else if (args[0] == Constants::MEMORY) {
            cout << get_memory_usage_percent();
        } else if (args[0] == Constants::CPU) {
            cout << get_cpu_usage_percent(3000);
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
        cout << buffer;
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
    cout << proc_delta;
    cout << total_delta;
    if (total_delta == 0) return 0.0;

    double cpu_usage = (double)proc_delta / total_delta * 100.0;
    return cpu_usage;
}

//inodes are unique identifiers for sockets being used on an os
//this function prints out all inodes for currently open sockets
void get_TCP_socket_inodes() {
    system("awk 'NR>1{ print $10 }' /proc/net/tcp");
}

void get_UDP_socket_inodes() {
    system("awk 'NR>1{ print $10 }' /proc/net/udp");
}

void get_process_for_TCP_inode(string inode) {
    const string script = "./get_inode_process.sh " + inode;
    system(script.c_str());
}

double get_memory_usage_percent() {
    ifstream file("/proc/meminfo");
    string line;
    long total_memory = 0, free_memory = 0, buffers = 0, cached = 0;

    while (getline(file, line)) {
        istringstream iss(line);
        string key;
        long value;
        string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") total_memory = value;
        else if (key == "MemFree:") free_memory = value;
        else if (key == "Buffers:") buffers = value;
        else if (key == "Cached:") cached = value;

        if (total_memory && free_memory && buffers && cached) break;
    }

    long used_memory = total_memory - free_memory - buffers - cached;
    return (double)used_memory / total_memory * 100.0;
}

long get_idle_cpu_time() {
    ifstream file("/proc/stat");
    string line;
    getline(file, line);
    istringstream ss(line);
    string cpu;
    long user, nice, system, idle;

    ss >> cpu >> user >> nice >> system >> idle;
    return idle;
}

double get_cpu_usage_percent(int interval_ms) {
    long total1 = get_total_cpu_time();
    long idle1 = get_idle_cpu_time();
    this_thread::sleep_for(chrono::milliseconds(interval_ms));
    long total2 = get_total_cpu_time();
    long idle2 = get_idle_cpu_time();

    long delta_total = total2 - total1;
    long delta_idle = idle2 - idle1;

    if (delta_total == 0) return 0.0;

    return 100.0 * (delta_total - delta_idle) / delta_total;
}

