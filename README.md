# SERVER-CLIENT CHAT

This code is a simple UDP client program that sends messages to a server and receives the echoed messages back from the server. Here's a detailed explanation of each part:

## CREATED THE PROJECT

### 1. Created a directory for my project

```bash
mkdir udp_chat
cd udp_chat
```

### 2. Created the source files for the server and the client

```bash
touch server.cpp client.cpp

```

### 3. Opened `client.cpp` in my text editor and added code to do the following

```cpp
// 1) Include necessary headers: These headers provide functionality for socket programming, networking, and string manipulation.

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

// 2) Define constants: These constants define the server's IP address, the port number to connect to, and the buffer size for receiving messages.

const char* SERVER_ADDRESS = "127.0.0.1";
const int PORT = 8080;
const int BUFFER_SIZE = 1024;

// 3) Define the main function:

int main() {

    // 4) Create a UDP socket: This line creates a new UDP socket using the socket() function, with the address family set to AF_INET (IPv4), the socket type set to SOCK_DGRAM (UDP), and the protocol set to 0 (default for UDP).

    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    // 5) Set up the server address structure: This block of code initializes the sockaddr_in structure for the server's address, sets the address family to `AF_INET` (IPv4), converts the port number to network byte order using `htons()`, and sets the server's IP address using `inet_addr()`.

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // 6) Main loop for sending and receiving messages: This loop will continue indefinitely, allowing the user to send multiple messages to the server.

    while (true) {

        // 7) Get user input: This block of code prompts the user to enter a message and stores it in the message variable.

        std::string message;
        std::cout << "Enter message (or q to quit) > ";
        getline(std::cin, message);

        // 8) This code block checks if the user input (message) is equal to "q". If it is, the following steps are executed:

        // a. The message "q" is sent to the server using the sendto() function. This informs the server that the client wants to quit the conversation.

        // b. A message "Sending quit!" is printed to the console, indicating that the client is about to terminate the conversation.

        // c. The break statement is executed to exit the while loop, effectively ending the client's participation in the conversation.
        
        if (message == "q") {
          sendto(client_socket, message.c_str(), message.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));
          std::cout << "\nSending quit!" << std::endl;
          break;
        }

        // 9) Send the message to the server: This line sends the user's message to the server using the sendto() function. It sends the message using the previously created client_socket, and specifies the server's address and its size.

        sendto(client_socket, message.c_str(), message.length(), 0, (sockaddr*)&server_addr, sizeof(server_addr));

        // 10) Receive the echoed message from the server: This block of code receives the echoed message from the server into a buffer. The recvfrom() function returns the number of bytes received. After receiving the message, it appends a null terminator to the end of the buffer.

        char buffer[BUFFER_SIZE];
        ssize_t received = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, nullptr, nullptr);
        buffer[received] = '\0';

        // 11) This code block checks if the received message (buffer) is equal to "q". If it is, the following steps are executed:

        // a. A message "*** Buddy has disconnected ***" is printed to the console, indicating that the client has terminated the conversation.

        // b. A message "(press enter to exit if stuck)" is printed to the console, providing a hint to the server user on how to exit the program if it is stuck for any reason.

        // c. The break statement is executed to exit the while loop, effectively ending the server's participation in the conversation.

        if (strcmp(buffer, "q") == 0) {
          std::cout << "\n*** Buddy has disconnected ***" << std::endl;
          std::cout << "(press enter to exit if stuck)" << std::endl;
          break;
        }

        // 12) Print the echoed message: This line prints the echoed message received from the server.

        std::cout << "\nBuddy > " << buffer << std::endl;
    }

    // 13) Close the main loop and the main function: This block of code closes the client_socket and returns 0 from the main() function, indicating successful execution.

    close(client_socket);
    std::cout << "Program exiting ..." << std::endl;
    return 0;
}
```

### 4. Opened `server.cpp` in my text editor and added code to do the following

```cpp
// 1) Include necessary headers: These headers provide functionality for socket programming, networking, and string manipulation.

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

// 2) Define constants: These constants define the port number to bind the server to and the buffer size for receiving messages.

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

// 3) Define the receive_and_echo function: This function handles the receiving and echoing of messages. It takes a pointer to the server's socket as a parameter.

void receive_and_echo(int* server_socket) {

    // 4) Receive and echo messages in a loop: This block of code receives messages from clients in a loop, prints the messages along with the client's IP address and port number, and echoes the messages back to the clients.

    // a) This is an infinite loop that will keep running until a break statement is encountered.
    while (true) {

      // b) A character array named buffer is created with a fixed size defined by BUFFER_SIZE.
      char buffer[BUFFER_SIZE];

      // c) A sockaddr_in structure named client_addr is created to store the client's address information.
      sockaddr_in client_addr;

      // d) The length of the client_addr structure is calculated and stored in client_addr_len.
      socklen_t client_addr_len = sizeof(client_addr);

      // e)  The server waits to receive a message from the client. The received message is stored in the buffer, and the number of bytes received is stored in received.
      ssize_t received = recvfrom(*server_socket, buffer, BUFFER_SIZE, 0, (sockaddr*)&client_addr, &client_addr_len);

      // f) A null terminator is added to the end of the buffer to mark the end of the received message.
      buffer[received] = '\0';

      // 5) The "if" block checks if the received message is "q". If it is, a "Buddy has disconnected" message is printed, and the loop is broken, ending the server's participation in the conversation.

      if (strcmp(buffer, "q") == 0)
      {

        // a) If the received message is not "q", it's printed to the console as "Buddy > [message]".
        std::cout << "\n*** Buddy has disconnected ***" << std::endl;
        std::cout << "(press enter to exit if stuck)" << std::endl;
        break;
      }

      // b) The server user is prompted to enter a message to send to the client.
      std::cout << "\nBuddy > " << buffer << std::endl;
      std::string message;
      std::cout << "Enter message (or q to quit) > ";
      getline(std::cin, message);

      // 6) The "if" block checks if the entered message is "q". If it is, the server sends the "q" message to the client and prints "Sending quit!", breaking the loop and ending the server's participation in the conversation.
      if (message == "q")
      {
        // a) If the entered message is not "q", the server sends the message to the client using sendto() and goes back to step 5, waiting for the client's response.
        sendto(*server_socket, message.c_str(), message.length(), 0, (sockaddr *)&client_addr, client_addr_len);
        std::cout << "\nSending quit!" << std::endl;
        break;
      }

      // Send the received message back to the client
      sendto(*server_socket, buffer, received, 0, (sockaddr*)&client_addr, client_addr_len);
    }
}

// 7) Define the main function:

int main() {

    // 8) Create a UDP socket: This line creates a new UDP socket using the socket() function, with the address family set to AF_INET (IPv4), the socket type set to SOCK_DGRAM (UDP), and the protocol set to 0 (default for UDP).

    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // 9) Set up the server address structure: This block of code initializes the sockaddr_in structure for the server's address, sets the address family to AF_INET (IPv4), converts the port number to network byte order using htons(), and sets the server's IP address to INADDR_ANY to bind the server to all available network interfaces.

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 10) Bind the socket to the server address: This line binds the server socket to the specified server address using the bind() function.

    bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr));

    // 11) Create a joined thread for receiving and echoing messages: This block of code creates a new thread to run the receive_and_echo function in parallel with the main function. The thread is joined so that it can run and join it back with the main thread.

    std::thread receive_thread([&] { receive_and_echo(&server_socket); });
    receive_thread.join();

    // 12) Close the server socket and return from the main function: This block of code closes the server socket and returns 0 from the main() function, indicating successful execution.

    close(server_socket);
    std::cout << "Program exiting ..." << std::endl;
    return 0;
}
```

## COMPLIED THE CODE

### Compiled the server and client programs using

```bash
clang++ -std=c++17 -o server server.cpp -lpthread
clang++ -std=c++17 -o client client.cpp

```

## CONNECTED CLIENT & SERVER

### 1. In the second window, I ran the client

```bash
./client
```

### 2. Opened two terminal windows. In the first window, I ran the server

```bash
./server
```
