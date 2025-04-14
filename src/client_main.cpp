#include "../include/client.hpp"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 12345



int main() {
    std::string pers;
    std::cout << "Are you a client or an admin?" << std::endl;
    std::cin >> pers;
    if(pers=="client"){
        Cl client;
        client.connServ(SERVER_ADDRESS, PORT);
        client.run();
    } else {
        std::string parol, ima, regg;
        AdminCl client;
        client.connServ(SERVER_ADDRESS, PORT);
        std::cout << "Enter your name and password: ";
        std::cin >> regg >> ima >> parol; 
        int r = client.isadm(ima,parol);
        if(r==1){ 
            std::cout << "Current client: " << client << std::endl;
            client.run();
        } else {
            std::cout << "Error" << std::endl;
        }
    }
    return 0;
}
