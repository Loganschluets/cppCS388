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
#include <ctime>
#include <dirent.h>
#include <cstring>
#include "constants.hpp"

using namespace std; 

void getCommand(vector<string> arguments);
void getProcesses();
void getProcessesForUser();
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
void display_tcp_connections();
void display_udp_connections();
void display_process_details(pid_t pid);
void killProcess(const string& pid);
void list_process_threads(pid_t pid);


int main() {
    cout << Constants::WELCOME_MESSAGE;
    string command = "";

    //terminal program driven by while loop and leaves on exit
    while (command != Constants::EXIT_COMMAND) {
        cout << Constants::TERMINAL_PROMPT;
        cout << "Enter Command: ";
        getline(cin, command);
        if (command != Constants::EXIT_COMMAND) {
            parseCommand(command);
        }
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
            getProcessesForUser();
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
        } else if (args[0] == Constants::TCP_CONNECTIONS) {
            display_tcp_connections();
        } else if (args[0] == Constants::UDP_CONNECTIONS) {
            display_udp_connections();
        }
    } else if (args.size() == 2) {
        if (args[0] == Constants::TCP) {

        } else if (args[0] == Constants::UDP) {
            
        } else if (args[0] == Constants::PROCESS_DETAILS) {
            try {
                pid_t pid = static_cast<pid_t>(std::stoi(args[1]));
                display_process_details(pid);
            } catch (const std::exception& e) {
                cerr << "Invalid PID: " << e.what() << std::endl;
            }
        } else if (args[0] == Constants::THREAD_LIST) {
            try {
                pid_t pid = static_cast<pid_t>(std::stoi(args[1]));
                list_process_threads(pid);
            } catch (const std::exception& e) {
                cerr << "Invalid PID: " << e.what() << std::endl;
            }
        }
    }
}

void getProcesses() {
    //show all processes by default
    system("ps aux");
}

void getProcessesForUser() {
    //get the current user's processes
    string command = "ps -u " + string(getenv("USER"));
    FILE* pipe = popen(command.c_str(), "r");
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

void display_tcp_connections() {
    string user_input;
    
    while (true) {
        cout << "Active TCP Connections (Type 'stop' to exit, Ctrl+C to exit):\n\n";
        system("netstat -tuna | grep ESTABLISHED");
        
        if (cin.peek() != EOF) {
            cin >> user_input;
            if (user_input == "stop") {
                cout << "\nTCP connections display stopped.\n";
                return;
            }
        }
        
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void display_udp_connections() {
    string user_input;
    
    while (true) {
        cout << "Active UDP Connections (Type 'stop' to exit, Ctrl+C to exit):\n\n";
        system("netstat -una | grep UDP");
        
        if (cin.peek() != EOF) {
            cin >> user_input;
            if (user_input == "stop") {
                cout << "\nUDP connections display stopped.\n";
                return;
            }
        }
        
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void list_process_threads(pid_t pid) {
    //first check if the process exists and we have permission to access it
    string proc_path = "/proc/" + to_string(pid);
    if (access(proc_path.c_str(), R_OK) == -1) {
        if (errno == ENOENT) {
            cerr << "Error: Process " << pid << " does not exist" << endl;
        } else if (errno == EACCES) {
            cerr << "Error: Permission denied. You don't have access to process " << pid << endl;
            cerr << "This is normal on shared systems - you can only view information about your own processes." << endl;
        } else {
            cerr << "Error accessing process " << pid << ": " << strerror(errno) << endl;
        }
        return;
    }

    string task_dir = "/proc/" + to_string(pid) + "/task";
    DIR* dir = opendir(task_dir.c_str());
    
    if (!dir) {
        cerr << "Error: Cannot access thread information for process " << pid << endl;
        cerr << "This might be because:" << endl;
        cerr << "1. The process has terminated" << endl;
        cerr << "2. You don't have permission to access the process" << endl;
        cerr << "3. The process is owned by another user" << endl;
        return;
    }

    cout << "Thread IDs (TIDs) for process " << pid << ":" << endl;
    
    struct dirent* entry;
    bool found_threads = false;
    
    while ((entry = readdir(dir)) != nullptr) {
        //skip . and .. directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        //each directory in task/ represents a thread
        cout << "TID: " << entry->d_name << endl;
        found_threads = true;
    }
    
    if (!found_threads) {
        cout << "No threads found for this process" << endl;
    }
    
    closedir(dir);
}

void display_process_details(pid_t pid) {
    //read process status
    string status_path = "/proc/" + to_string(pid) + "/status";
    ifstream status_file(status_path);
    if (!status_file.is_open()) {
        cerr << "Error: Process " << pid << " does not exist" << endl;
        return;
    }

    string name, state, threads;
    string line;
    
    while (getline(status_file, line)) {
        if (line.find("Name:") == 0) {
            name = line.substr(6);
        } else if (line.find("State:") == 0) {
            state = line.substr(7);
        } else if (line.find("Threads:") == 0) {
            threads = line.substr(9);
        }
    }
    status_file.close();

    //read process start time from stat
    string stat_path = "/proc/" + to_string(pid) + "/stat";
    ifstream stat_file(stat_path);
    if (!stat_file.is_open()) {
        cerr << "Error reading process stat" << endl;
        return;
    }

    string stat_line;
    getline(stat_file, stat_line);
    istringstream stat_iss(stat_line);
    vector<string> stat_fields;
    string field;
    
    while (stat_iss >> field) {
        stat_fields.push_back(field);
    }
    stat_file.close();

    if (stat_fields.size() < 22) {
        cerr << "Error: Invalid stat file format" << endl;
        return;
    }

    //calculate start time
    long start_time = stol(stat_fields[21]);
    long uptime = 0;
    
    ifstream uptime_file("/proc/uptime");
    if (uptime_file.is_open()) {
        uptime_file >> uptime;
        uptime_file.close();
    }

    long start_seconds = uptime - (start_time / sysconf(_SC_CLK_TCK));
    time_t start_time_t = time(nullptr) - start_seconds;
    string start_time_str = ctime(&start_time_t);
    start_time_str = start_time_str.substr(0, start_time_str.length() - 1); //remove newline

    //print process details
    cout << "Process Details for PID " << pid << ":" << endl;
    cout << "Name: " << name << endl;
    cout << "State: " << state << endl;
    cout << "Threads: " << threads << endl;
    cout << "Start Time: " << start_time_str << endl;
}

void killProcess(const string& pid) {
     string cmd = "kill -9 " + pid;
     int result = system(cmd.c_str());
     if (result != 0) cout << "Failed to kill process " << pid << "\n";
 }

