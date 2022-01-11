/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#include "Server.h"

// create the server socket
Server::Server(int port)throw (const char*) {
    server_id = socket(AF_INET, SOCK_STREAM, 0);
    if (server_id < 0)
        throw "failed to open socket";
    server_sockaddr.sin_family = AF_INET;   // IP format
    server_sockaddr.sin_port = htons(port);     // port to listen
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;   // IP address
    if(bind(server_id, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr)) == -1) {
        throw "bind failed!";
    }
    if(listen(server_id, 5) == -1) {
        throw "listen failed!";
    }
}

// run the server socket in another thread
void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this](){
        while(flag) {
            socklen_t size = sizeof(client_sockaddr);
            int client_ID = accept(server_id, (struct sockaddr* ) &client_sockaddr, &size);
            ch.handle(client_ID);
            close(client_ID);
        }
    });
}

void Server::stop(){
    flag = false;
    close(server_id);
	t->join(); // do not delete this!
}

Server::~Server() {
}

