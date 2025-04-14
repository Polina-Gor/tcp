#include "../include/server.hpp"

int main() {
    ChatServer server(12345);
    server.start();
    return 0;
}
