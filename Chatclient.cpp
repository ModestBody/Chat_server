#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

SOCKET clientSocket;

void receiveMessages() {
    char buffer[1024];
    int result;
    while (true) {
        result = recv(clientSocket, buffer, 1024, 0);
        if (result > 0) {
            buffer[result] = '\0';
            cout << buffer << endl;
        }
        else {
            break;
        }
    }
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        cout << "WSAStartup failed: " << iResult << endl;
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "192.168.92.1", &serverAddr.sin_addr); 

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Connect failed: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    string nickname;
    cout << "Choose username: ";
    getline(cin, nickname);

    thread(receiveMessages).detach();

    string message;
    while (true) {
        getline(cin, message);
        message = nickname + ": " + message;
        send(clientSocket, message.c_str(), message.length(), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
