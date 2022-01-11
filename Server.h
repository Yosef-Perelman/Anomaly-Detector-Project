/*
 * anomaly_detection_util.cpp
 *
 * Author: Yosef Perelman 206344814 and Ariel Mantel 313450249
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"

using namespace std;

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class socket_IO: public DefaultIO {
    int client_ID;

public:
    socket_IO(int clientID):client_ID(clientID){}

    string read() override{
        string str;
        char buf = 0;
        recv(client_ID, &buf, sizeof(char), 0);
        // Read from the client line by line
        while(buf != '\n'){
            str += buf;
            recv(client_ID, &buf, sizeof(char), 0);
        }
        return str;
    }

    // probably not in use
    void read(float* f) override{
        recv(client_ID, f, sizeof(float), 0);
    }

    // send the strings in chars
    void write(string str) override{
        for (auto buf:str){
            send(client_ID, &buf, sizeof (char), 0);
        }
    }

    // convert float to string and call write(string)
    void write(float f) override{
        ostringstream ss;
        ss << f;
        std::string str(ss.str());
        this->write(str);
    }

    virtual ~socket_IO(){}
};


class AnomalyDetectionHandler:public ClientHandler{
	public:
    // create IO and call the command line interface
    virtual void handle(int clientID){
        socket_IO sio(clientID);
        CLI cli(&sio);
        cli.start();
    }
};


class Server {
	thread* t;  // the thread to run the start() method in
    int server_id;  // file descriptor (actually identifier) of the server socket
    bool flag = true;   // set the run state
    sockaddr_in server_sockaddr;    // details of the server socket
    sockaddr_in client_sockaddr;    // details of the client socket

public:
    // implement in Server.cpp
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch) throw (const char*);
	void stop();
};

#endif /* SERVER_H_ */
