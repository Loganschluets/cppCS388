#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

vector<int> server_sockets;

void create_server(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        cerr << "Error creating server socket" << endl;
        return;
    }

    //allow socket reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Error binding server socket to port " << port << endl;
        close(server_fd);
        return;
    }

    if (listen(server_fd, 3) < 0) {
        cerr << "Error listening on port " << port << endl;
        close(server_fd);
        return;
    }

    server_sockets.push_back(server_fd);
    cout << "Server listening on port " << port << endl;
}

void create_connection(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error creating client socket" << endl;
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Error connecting to port " << port << endl;
        close(sock);
        return;
    }

    cout << "Connected to port " << port << endl;
    
    //keep the connection open
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void cleanup() {
    for (int sock : server_sockets) {
        close(sock);
    }
}

int main() {
    int base_port = 8000;
    int num_connections = 0;
    
    cout << "Starting TCP connection test. Press Ctrl+C to stop." << endl;
    
    while (true) {
        //first create a server on the port
        create_server(base_port + num_connections);
        
        //then create a client connection
        thread t(create_connection, base_port + num_connections);
        t.detach();
        
        num_connections++;
        cout << "Created connection #" << num_connections << " to port " << (base_port + num_connections - 1) << endl;
        
        this_thread::sleep_for(chrono::seconds(10));
    }
    
    cleanup();
    return 0;
} 