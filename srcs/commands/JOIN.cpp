#include "../../includes/Server.hpp"

void Server::Join(std::vector<std::string>& params, Client& cli)
{
    if (cli._passChecked == 0)
    {
        passChecker(cli);
        return;
    }

    if (params.empty())
    {
        Utils::writeMessage(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, "JOIN"));
        return;
    }

    std::vector<std::string> channels;
    std::string keys = "";
    
    std::stringstream ss(params[0]);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        channels.push_back(item);
    }

    if (params.size() == 2)
    {
        keys = params[1];
    }

    for (size_t i = 0; i < channels.size(); ++i)
    {
        std::string chan = channels[i];
        std::string key = "";
        
        if (!keys.empty())
        {
            std::stringstream keyStream(keys);
            std::string keyItem;
            std::getline(keyStream, keyItem, ',');
            if (i == 0 || keyStream.eof())
            {
                key = keyItem;
            }
        }

        if (chan.empty())
        {
            Utils::writeMessage(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, "JOIN"));
            return;
        }

        if (chan[0] != '#' && chan[0] != '&')
        {
            Utils::writeMessage(cli._cliFd, ERR_INVCHANNAME);
            return;
        }

        if (clientIsInThere(cli, chan) == 0)
        {
            if (!cli._nick.empty())
            {
                int isThere = 0;
                for (chanIt it = _channels.begin(); it != _channels.end(); ++it)
                {
                    if (it->_name == chan)
                    {
                        if (it->_key == key)
                        {
                            if (it->_userLimit != 0 && it->_channelClients.size() >= it->_userLimit)
                            {
                                Utils::writeMessage(cli._cliFd, ERR_CHANNELISFULL(cli._nick, chan));
                                return;
                            }
                            it->_channelClients.push_back(cli);
                            it->_opNick = it->_channelClients[0]._nick;
                            Utils::writeMessage(cli._cliFd, RPL_JOIN(cli._nick, cli._ip, chan));
                            std::cout << PURPLE << "Client " << cli._nick << " has entered \'" << chan << "\'" << RESET << std::endl;
                            showRightGui(cli, *it);
                        }
                        else
                        {
                            Utils::writeMessage(cli._cliFd, ERR_BADCHANNELKEY(cli._nick, chan));
                        }
                        isThere = 1;
                        break;
                    }
                }
                if (isThere == 0)
                {
                    Channel tmp;
                    tmp._name = chan;
                    if (!key.empty())
                    {
                        tmp._key = key;
                    }
                    tmp._channelClients.push_back(cli);
                    tmp._opNick = tmp._channelClients[0]._nick;
                    _channels.push_back(tmp);
                    Utils::writeMessage(cli._cliFd, RPL_JOIN(cli._nick, cli._ip, chan));
                    if (!key.empty())
                    {
                        std::cout << PURPLE << "Channel " << chan << " created with " << key << RESET << std::endl;
                    }
                    else
                    {
                        std::cout << PURPLE << "Channel " << chan << " created" << RESET << std::endl;
                    }
                    showRightGui(cli, tmp);
                }
            }
            else
            {
                Utils::writeMessage(cli._cliFd, "Set your nick before!\r\n");
            }
        }
        else
        {
            Utils::writeMessage(cli._cliFd, "You are already in this channel\r\n");
        }
    }
}
