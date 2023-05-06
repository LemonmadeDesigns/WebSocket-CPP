#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void receive_and_echo(int *server_socket)
{
  while (true)
  {
    char buffer[BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    ssize_t received = recvfrom(*server_socket, buffer, BUFFER_SIZE, 0, (sockaddr *)&client_addr, &client_addr_len);
    buffer[received] = '\0';

    if (strcmp(buffer, "q") == 0)
    {
      std::cout << "\n*** Buddy has disconnected ***" << std::endl;
      std::cout << "(press enter to exit if stuck)" << std::endl;
      break;
    }

    std::cout << "\nBuddy > " << buffer << std::endl;
    std::string message;
    std::cout << "Enter message (or q to quit) > ";
    getline(std::cin, message);

    if (message == "q")
    {
      sendto(*server_socket, message.c_str(), message.length(), 0, (sockaddr *)&client_addr, client_addr_len);
      std::cout << "\nSending quit!" << std::endl;
      break;
    }

    // Send the message to the client
    sendto(*server_socket, message.c_str(), message.length(), 0, (sockaddr *)&client_addr, client_addr_len);
  }
}

int main()
{
  int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr));

  std::thread receive_thread([&] { receive_and_echo(&server_socket); });
  receive_thread.join();

  close(server_socket);
  std::cout << "Program exiting ..." << std::endl;
  return 0;
}
