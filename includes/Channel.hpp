#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Client; 
class Channel
{
    public:

        size_t _userLimit;
        std::string _name;
        std::string _topic;
        std::string _key;
        std::string _opNick;
        std::vector<Client> _channelClients;
        Channel() : _userLimit(0), _name(""), _topic(""), _key(""), _opNick("") {}

        std::vector<int> getFds() const;
};

#endif