#include "../include/server.hpp"

ClH::ClH(SOCKET socket, int id) : clsoc(socket), clId(id) {
    std::cout << "Client " << clId << " connected!" << std::endl;
}

ClH::~ClH() {
    closesocket(clsoc);
    std::cout << "Client " << clId << " disconnected!" << std::endl;
}

std::string ClH::m_st(char* m, int r) {
    std::string a="";
    for (int i = 0; i < r; i++) {
        a+=m[i];
    }
    return a;
}

void ClH::handleClient(std::unordered_map<int, ClH*>& clients, std::mutex& clientsMutex) {
    char buffer[102400];
    while (true) {
        int bytesReceived = recv(clsoc, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string o1 = m_st(buffer,bytesReceived);
            if(o1.find("reg_admin")==0){
                if(o1.substr(10)=="Polina 05102006" || o1.substr(10)=="Julia 02052006" || o1.substr(10)=="Thebeststarosta 22022006"){
                    for (auto const& [id, client] : clients) {
                        if (client == this) {
                            char iq[1]={'1'};
                            send(client->clsoc, iq, 1, 0);
                        }
                    }
                } else {
                    for (auto const& [id, client] : clients) {
                        if (client == this) {
                            char iq[1]={'0'};
                            send(client->clsoc, iq, 1, 0);
                        }    
                    }
                }
                continue;
            } 
            std::cout << buffer << std::endl;
            for (auto const& [id, client] : clients) {
                if (client != this) {
                    send(client->clsoc, buffer, bytesReceived, 0);
                } 
            }
        } else {
            delete this;
            return;
        }
    }
}

ChatServer::ChatServer(int port) : port(port), nextClientId(1) {}

void ChatServer::start() {
    WSADATA wsaData;
    struct sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Server listening on port " << port << "..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientSize);
        ClH* clientHandler = new ClH(clientSocket, nextClientId);
        {
            std::lock_guard<std::mutex> lock(clientsMutex); 
            clients[nextClientId] = clientHandler; 
        }

        std::thread clientThread(&ClH::handleClient, clientHandler, std::ref(clients), std::ref(clientsMutex));
        clientThread.detach();

        nextClientId++;
    }
}

ChatServer::~ChatServer() {
    closesocket(serverSocket);
    WSACleanup();
}
