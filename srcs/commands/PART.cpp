#include "../../includes/Server.hpp"

void Server::Part(std::vector<std::string>& params, Client& cli)
{
    passChecker(cli);

    if (params.size() > 2 || params[0].empty()) {
        Utils::writeMessage(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        return;
    }

    std::vector<std::string> channels;
    std::stringstream ss(params[0]);
    std::string item;
    while (std::getline(ss, item, ',')) {
        channels.push_back(item);
    }

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string chan = channels[i];

        if (isChannelExist(chan)) {
            for (chanIt it = _channels.begin(); it != _channels.end(); ++it) {
                if (it->_name == chan) {
                    bool clientFound = false;
                    for (cliIt it2 = it->_channelClients.begin(); it2 != it->_channelClients.end(); ++it2) {
                        if (it2->_nick == cli._nick) {
                            Utils::writeMessage(cli._cliFd, RPL_PART(cli._nick, chan));
                            it->_channelClients.erase(it2);
                            if (it->_channelClients.size() > 0)
                                it->_opNick = it->_channelClients[0]._nick;
                            std::cout << RED << "Client " << cli._nick << " has left channel " << chan << RESET << std::endl;
                            clientFound = true;
                            break;
                        }
                    }
                    if (clientFound) {
                        if (it->_channelClients.size() == 0) {
                            std::cout << RED << "Channel " << it->_name << " is deleted" << RESET << std::endl;
                            _channels.erase(it);
                        } else {
                            showRightGui(cli, *it);
                        }
                        break;
                    }
                }
            }
        } else {
            Utils::writeMessage(cli._cliFd, ERR_NOSUCHCHANNEL(cli._nick, chan));
        }
    }
}
