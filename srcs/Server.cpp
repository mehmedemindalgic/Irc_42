#include "../includes/Utils.hpp"

Server* Server::singleton = NULL;  

Server::Server(): _botFd(0), _fdCount(0) {}

Server::~Server() {}

Server* Server::getInstance()
{
    try {
        if (singleton == NULL) 
            singleton = new Server; 
        return singleton;
    } catch (std::exception & e) { 
        std::cerr << e.what() << std::endl;
        delete singleton;
        exit(1);
    }
}

void Server::signalHandler(int number)
{
    std::cout << RED << "\nServer is shutting down!" << RESET << std::endl;
    delete singleton;
    close(singleton->_serverFd);
    exit(number);
}

void Server::initCommands()
{
    this->_commands["PASS"] =     &Server::Pass;
    this->_commands["NICK"] =     &Server::Nick;
    this->_commands["JOIN"] =     &Server::Join;
    this->_commands["CAP"] =      &Server::Cap;   
    this->_commands["USER"] =     &Server::User;
    this->_commands["WHO"] =      &Server::Who;
    this->_commands["QUIT"] =     &Server::Quit;
    this->_commands["PART"] =     &Server::Part;
    this->_commands["PRIVMSG"] =  &Server::Privmsg;
    this->_commands["NOTICE"] =   &Server::Notice;
    this->_commands["KICK"] =     &Server::Kick;
    this->_commands["MODE"] =     &Server::Mode;
    this->_commands["TOPIC"] =    &Server::Topic;
    this->_commands["INVITE"] =   &Server::Invite;
    this->_commands["bot"] =      &Server::Bot;
}

void Server::createSocket()
{
    if ((this->_serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Socket");
    fcntl(this->_serverFd, F_SETFL, O_NONBLOCK);
    const int enable = 1;
    if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) 
        throw std::runtime_error("Setsockopt");
}

void Server::bindSocket()
{
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->_port);

    if (bind(this->_serverFd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Bind");
  
    if (listen(this->_serverFd, SOMAXCONN) < 0)
        throw std::runtime_error("Listen");
}

void Server::acceptRequest()
{
    Client tmp;
    sockaddr_in cliAddr;
    socklen_t cliSize = sizeof(cliAddr);

    tmp._cliFd = accept(this->_serverFd, (sockaddr *)&cliAddr, &cliSize);
    fcntl(tmp._cliFd, F_SETFL, O_NONBLOCK);
    if (tmp._cliFd <= 0) 
        throw std::runtime_error("Accept failed"); 
    tmp._port = ntohs(cliAddr.sin_port);
    inet_ntop(AF_INET, &(cliAddr.sin_addr), tmp._ipAddr, INET_ADDRSTRLEN);
    FD_SET(tmp._cliFd, &this->_readFds);
    std::cout << GREEN << "New client connected!" << RESET << std::endl;
    this->_fdCount++;
    this->_clients.push_back(tmp);
}

std::map<std::string, std::vector<std::string> > Server::getParams(std::string const& str)
{
    std::stringstream ss(str);
    std::string tmp;
    std::map<std::string, std::vector<std::string> > ret;
    std::vector<std::string> params;
    ss >> tmp;
    std::string cmd;
    while (1)
    {
        cmd = tmp;
        if (ret.find(cmd) != ret.end())
            return ret;
        params.clear();
        ss >> tmp;
        while (this->_commands.find(tmp) == this->_commands.end())
        {
            params.push_back(tmp);
            ss >> tmp;
            if (ss.eof())
            {
                ret[cmd] = params;
                return ret;
            }
        }
        if (ss.eof())
        {
            params.push_back("");
            ret[cmd] = params;
            return ret;
        }
        if (params.empty())
            params.push_back(""); 
        ret[cmd] = params;
    }
    return ret;
}

void Server::commandHandler(std::string& str, Client& cli)
{
    std::map<std::string, std::vector<std::string> > params = getParams(str);

    for (std::map<std::string, std::vector<std::string> >::iterator it = params.begin(); it != params.end(); ++it)
    {
        if (this->_commands.find(it->first) == this->_commands.end())
        {
            Utils::writeMessage(cli._cliFd, "421 : " + it->first + " :Unknown command!\r\n");
            std::cout << RED << it->first << " command not found!" << RESET << std::endl;
        }
        else
            (this->*_commands[it->first])(it->second, cli);
    }
}

void Server::readEvent(int* state)
{
    for (cliIt it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (FD_ISSET(it->_cliFd, &this->_readFdsSup))
        {
            *state = 0;
            int readed = read(it->_cliFd, this->_buffer, 1024);
            if (readed <= 0)
            {
                std::vector<std::string> tmp;
                tmp.push_back("");
                (this->*_commands["QUIT"])(tmp, *it);
            }
            else
            {
                this->_buffer[readed] = 0;
                std::string tmp = this->_buffer;
                if (tmp == "\n")
                {
                    *state = 0; break;
                }
                if (tmp[tmp.length() - 1] != '\n')
                {
                    it->_buffer += tmp;
                    *state = 0; break;
                }
                else
                {
                    it->_buffer = it->_buffer + tmp;
                }
                std::cout << YELLOW << it->_buffer << RESET;
                commandHandler(it->_buffer, *it);
                it->_buffer.clear();
            }
            break;
        }
    }
}

void Server::writeEvent()
{
    for (cliIt it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        if (FD_ISSET(it->_cliFd, &this->_writeFdsSup))
        {
            int writed = write(it->_cliFd, it->_messageBox[0].c_str(), it->_messageBox[0].size());
            it->_messageBox.erase(it->_messageBox.begin());
            if (it->_messageBox.empty())
                FD_CLR(it->_cliFd, &this->_writeFds);
            if (writed <= 0)
            {
                std::vector<std::string> tmp;
                tmp.push_back("");
                (this->*_commands["QUIT"])(tmp, *it);
            }
            break ;
        }
    }
}

void Server::initFds()
{
    FD_ZERO(&this->_readFds);
    FD_ZERO(&this->_writeFds);
    FD_ZERO(&this->_readFdsSup);
    FD_ZERO(&this->_writeFdsSup);
    FD_SET(this->_serverFd, &this->_readFds);
}

void Server::run()
{
    int state = 0;

    initFds();
    while (1)
    {
        this->_readFdsSup = this->_readFds;
        this->_writeFdsSup = this->_writeFds;
        state = select(_fdCount + 4, &this->_readFdsSup, &this->_writeFdsSup, NULL, 0);
        if (state == 0)
            throw std::runtime_error("Select");
        if (FD_ISSET(this->_serverFd, &this->_readFdsSup)) {
            acceptRequest();
            state = 0; continue;
        }
        if (state) {
            readEvent(&state);
            if (state == 0)
                continue;
        }
        if (state) {
            writeEvent();
            state = 0;
            continue;
        }
    }
}

void Server::manageServer(size_t const& port, std::string const& password)
{
    setPort(port);
    setPassword(password);
    initCommands();
    createSocket();
    bindSocket();
    printStatus();
    run();
}
