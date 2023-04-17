#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

void client_thread(int sock) {
    char buffer[1024];
    string message;

    while (true) {
        cout << "Enter message: ";
        getline(cin, message);
        const char* msg = message.c_str();
        send(sock, msg, strlen(msg), 0);
        memset(buffer, 0, sizeof(buffer));
        int recv_bytes = recv(sock, buffer, 1024, 0);
        if (recv_bytes != -1) {
            cout << buffer << endl;
        }
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cout << "Error: Could not create socket." << endl;
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    int connect_result = connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));
    if (connect_result == -1) {
        cout << "Error: Connection failed." << endl;
        return 1;
    }

    thread t(client_thread, sock);
    t.join();

    close(sock);

    return 0;
}
