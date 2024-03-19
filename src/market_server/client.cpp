#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <thread>

#define PORT 8080
#define MAX_RETRY_COUNT 5

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string firstMessage = "Hello, Server 1!";
    std::string secondMessage = "Hello, Server 2!";

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    int retry_count = 0;
    bool connected = false;

    while (retry_count < MAX_RETRY_COUNT && !connected) {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cout << "\n Socket creation error \n";
            return -1;
        }

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            std::cout << "\nInvalid address/ Address not supported \n";
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) >= 0) {
            connected = true;
            break;
        }

        std::cout << "Connection Failed. Retrying in 5 seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        retry_count++;
    }

    if (!connected) {
        std::cout << "Failed to connect to the server." << std::endl;
        return -1;
    }

    send(sock, firstMessage.c_str(), firstMessage.size(), 0);
    std::cout << "Sent: " << firstMessage << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(10));

    send(sock, secondMessage.c_str(), secondMessage.size(), 0);
    std::cout << "Sent: " << secondMessage << std::endl;

    // Loop to keep receiving messages from the server
    char buffer[1024] = {0};
    while (true) {
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            // Break the loop if the server disconnects or an error occurs
            break;
        }
        std::cout << "Received: " << buffer << std::endl;
        memset(buffer, 0, sizeof(buffer)); 
    }

    std::cout << "Server disconnected." << std::endl;
    close(sock);

    return 0;
}
