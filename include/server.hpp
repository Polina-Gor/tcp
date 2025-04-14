#pragma once
#include <winsock2.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <thread>

class ClH {
public:
    ClH(SOCKET socket, int id);
    ~ClH();
    void handleClient(std::unordered_map<int, ClH*>& clients, std::mutex& clientsMutex);
private:
    SOCKET clsoc;
    int clId;
    static std::string m_st(char* m, int r);
};

class ChatServer {
public:
    ChatServer(int port);
    void start();
    ~ChatServer();
private:
    SOCKET serverSocket;
    int port;
    int nextClientId;
    std::unordered_map<int, ClH*> clients;
    std::mutex clientsMutex;
};
