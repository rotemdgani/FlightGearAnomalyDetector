#include "Server.h"

    string SocketIO::read() {
        char c;
        string input="";
        ::read(clientID,&c,sizeof(char));
        while(c!='\n'){
            input+=c;
            ::read(clientID,&c,sizeof(char));
        }
        return input;
    }

    void SocketIO::write(string text) {
        ::write(clientID, text.c_str(), strlen(text.c_str()));
    }

    void SocketIO::write(float f) {
        string num = to_string(f);
        ::write(clientID, &num, num.length());
    }

    void SocketIO::read(float* f) {
        ::read(clientID, f, sizeof(float));
    }


Server::Server(int port)throw (const char*) {
    open = true;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw "Socket failed!";

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw "Bind failure!";

    if(listen(fd, 3) < 0)
        throw "Listen failure!";
}

void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this]() {
        while(open) {
            socklen_t clientSize = sizeof(client);
            alarm(1);
            int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
            if (aClient < 0)
                throw "Accept failure!";
            else if (aClient >= 1) {
                ch.handle(aClient);
                close(aClient);
            }
            alarm(1);
        }
        close(fd);
    });
}

void Server::stop(){
    open = false;
	t->join(); // do not delete this!
}

Server::~Server() {
}

