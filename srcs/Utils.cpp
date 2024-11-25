#include "../includes/Utils.hpp"

void Utils::writeMessage(int socket, std::string const& message)
{
    if ((write(socket, message.c_str(), message.length())) < 0)
        std::cout << "Message cannot send!" << std::endl;
}

void Utils::writeAllMessage(std::vector<int> const& fds, std::string const& message)
{
    for (std::vector<int>::const_iterator it = fds.begin(); it != fds.end(); ++it) {
        writeMessage(*it, message);
    }
}

std::string Utils::intToString(int n)
{
    std::stringstream ss;
    ss << n;
    std::string str = ss.str();
    return str;
}

int Utils::portIsValid(std::string const& port)
{
    for (size_t i = 0; i < port.size(); ++i)
        if (!isdigit(port[i]))
            return 0;
	int portInt = std::atoi(port.c_str());
    if (portInt < 1024 || portInt > 49151)
        return 0;
    return 1;
}

std::string Utils::welcome()
{
    std::string data;

    data += "           _____     _____       _____                    \n";
    data += "          |_   _|   |  __ \\     / ____|                   \n";
    data += "            | |     | |__) |   | |                        \n";
    data += "            | |     |  _  /    | |                       \n";
    data += "           _| |_    | | \\ \\    | |____                   \n";
    data += "          |_____|   |_|  \\_\\    \\_____|                  \n";
    data += "                                                         \n";
    data += "                                                         \n";
    data += "    _____   ______   _____   __      __  ______   _____  \n";
    data += "   / ____| |  ____| |  __ \\  \\ \\    / / |  ____| |  __ \\ \n";
    data += "  | (___   | |__    | |__) |  \\ \\  / /  | |__    | |__) |\n";
    data += "   \\___ \\  |  __|   |  _  /    \\ \\/ /   |  __|   |  _  / \n";
    data += "   ____) | | |____  | | \\ \\     \\  /    | |____  | | \\ \\ \n";
    data += "  |_____/  |______| |_|  \\_\\     \\/     |______| |_|  \\_\\\n";
    data += "                                                         \n";
    return data;
}
