#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Utils.hpp"
# define BOTNAME "My_IRC_BOT"

class Bot
{
    private:

        int _fd;
        int _port;
        std::string _password;
        static Bot* singleton;
        

        Bot(): _fd(0), _port(0), _password("") {};
        ~Bot();
        void setPort(int);
        void setPasword(std::string const&);
        void createSocket();
        void run();
        
    public:
        static Bot *getInstance();
        void manageBot(int, std::string const&);
};

#endif