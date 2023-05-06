#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const char* SERVER_ADDRESS = "127.0.0.1";
const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    while (true) {
        std::string message;
        std::cout << "Enter message (or q to quit) > ";
        getline(std::cin, message);

        if (message == "q") {
            sendto(client_socket, message.c_str(), message.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));
            std::cout << "\nSending quit!" << std::endl;
            break;
        }

        sendto(client_socket, message.c_str(), message.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));

        char buffer[BUFFER_SIZE];
        ssize_t received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, nullptr, nullptr);
        buffer[received] = '\0';

        if (strcmp(buffer, "q") == 0) {
            std::cout << "\n*** Buddy has disconnected ***" << std::endl;
            std::cout << "(press enter to exit if stuck)" << std::endl;
            break;
        }

        std::cout << "\nBuddy > " << buffer << std::endl;
    }

    close(client_socket);
    std::cout << "Program exiting ..." << std::endl;
    return 0;
}


