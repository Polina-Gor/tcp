#pragma once
#include <winsock2.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <vector>
#include <iostream>
#include <limits>

#pragma comment(lib, "ws2_32.lib")


void st_m(std::string a, int* m);
int razmer(std::string s);
void sh(const std::string &sl, char c, int* m);
std::string ch_st(char* m);

class Cl {
public:
    Cl();
    ~Cl();
    void connServ(const char* serverAddress, int port);
    virtual void run();
    SOCKET clsoc;
protected:
    static std::string m_st(int* m, int r);
};

class AdminCl : public Cl {
public:
    AdminCl();
    friend std::ostream& operator<<(std::ostream& os, const AdminCl& client);
    void run() override;
    int isadm(std::string n, std::string p);
private:
    static std::string m_st(int* m, int r);
};
