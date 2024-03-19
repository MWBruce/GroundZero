#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <chrono>
#include <cstring>
#include <errno.h>

#define PORT 8080

class StringProcessor {
public:
    void testing(const std::string& input) {
        std::cout << "Testing: " << input << std::endl;
    }
};

void handleClient(int clientSocket, StringProcessor& processor) {
    char buffer[1024] = {0};
    int recv_size;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        recv_size = recv(clientSocket, buffer, sizeof(buffer), MSG_DONTWAIT);

        if (recv_size > 0) {
            std::string receivedData(buffer);
            processor.testing(receivedData);
        } else if (recv_size == 0) {
            std::cout << "Client " << clientSocket << " disconnected." << std::endl;
            break;
        } else if (recv_size < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Error receiving data from client " << clientSocket << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(clientSocket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::vector<std::thread> threads;
    StringProcessor processor;

    while (true) {
        std::cout << "Waiting for new connection..." << std::endl;
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        std::cout << "Client connected: " << client_socket << std::endl;
        threads.push_back(std::thread(handleClient, client_socket, std::ref(processor)));
    }

    for (auto& th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    close(server_fd);
    return 0;
}
