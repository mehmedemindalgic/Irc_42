#ifndef UTILS_HPP
#define UTILS_HPP

#include "../includes/Server.hpp"
#include <sstream>

# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define PURPLE		"\033[0;35m"
# define CYAN		"\033[3;36m"
# define END		"\033[m"
# define RESET		"\033[0m"


#define RPL_JOIN(nick, ip, channel)					":" + nick + "!" + nick + "@" + ip + " JOIN " + channel + "\r\n"
#define RPL_NAMREPLY(nick, channel, users)			": 353 " + nick + " = " + channel + " :" + users + "\r\n"
#define RPL_TOPIC(nick, ip, channel, topic)         ":" + nick + "!" + nick + "@" + ip + " TOPIC " + channel + " :" + topic + "\r\n"       
#define RPL_INVITE(nick, ip, channel, target)       ":" + nick + "!" + nick + "@" + ip + " INVITE " + target + " " + channel + "\r\n"  
#define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"
#define RPL_NOTICE(source, target, message)			":" + source + " NOTICE " + target + " :" + message + "\r\n"
#define RPL_NICK(nick, user, ip, newnick)			":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"
#define RPL_KICK(source, channel, target, reason)	":" + source + " KICK " + channel + " " + target + " :" + reason + "\r\n"
#define RPL_PART(source, channel)					":" + source + " PART :" + channel + "\r\n"
#define RPL_QUIT(source, message)                   ":" + source + " QUIT :" + message + "\r\n"
#define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"


#define RPL_WELCOME(source, msg)				     + msg + "\r\n" + source
#define RPL_USERHOST(usr, host, server, realname)   "\n* User Created *" "\nUser: " + usr + "\nHostname: " + host + "\nServername: " + server + "\nRealname: " + realname + "\r\n"
#define RPL_USERERROR(msg)                          "Expected input: <" + msg + ">, <hostname>, <servername>, <realname>" + "\r\n"


#define ERR_CHANNELISFULL(source, channel)			": 471 " + source + " " + channel + " :Cannot join channel (+l)" + "\r\n"
#define ERR_BADCHANNELKEY(source, channel)			": 475 " + source + " " + channel + " :Cannot join channel (+k)" + "\r\n"
#define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"
#define ERR_CHANOPRIVSNEEDED(source, channel)		": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"
#define ERR_CHANOPKICK(source, channel)		        ": 482 " + source + " " + channel + " :You cannot KICK yourself" + "\r\n"
#define ERR_NOSUCHCHANNEL(source, channel)			": 403 " + source + " " + channel + " :No such channel" + "\r\n"
#define ERR_NICKNAMEINUSE(source)				    ": 433 " + source + " " + source  + " :Nickname is already in use" + "\r\n"
#define ERR_NICKNAMEEMPTY(source)				    ": 433 " + source + " " + source  + " :Nickname cannot empty" + "\r\n"
#define ERR_UNKNOWNMODE(source, channel, mode)	    ": 472 " + source + " " +  channel + " :Unknown Mode " + mode + "\r\n"
#define RPL_NOTOPIC(nick, channel)                  ": 331 " + nick + " " + channel + " :No topic is set" + "\r\n"

#define ERR_NICKNAMEINUSE(source) ": 433 " + source + " " + source  + " :Nickname is already in use" + "\r\n"
#define ERR_NICKNAMEEMPTY(source) ": 433 " + source + " " + source  + " :Nickname cannot empty" + "\r\n"
#define RPL_MODEE(source, channel, modes, args)     ":" + source + " MODE " + channel + " " + modes + "\r\n"

#define ERR_ALREADYREGISTRED "Already registered\r\n"
#define ERR_PASSWDMISMATCH "Password mismatch\r\n"
#define ERR_NONICKNAMEGIVEN "No nickname given\r\n"
#define ERR_INVCHANNAME "Invalid channel name, must be: #<channel name> <?key>\r\n"
#define ERR_NOSUCHNICK ":No such nick\r\n"
#define RPL_MODUL "/PASS\n/USER\n/NICK\n/JOIN\n/PART\n/PRIVMSG\n/NOTICE\n/KICK\n/MODE\n/TIME\n/LIST\n/TOPIC\n/WHO\n/USERHOST\n/QUIT\n"


class Utils
{
    public:
        static void writeMessage(int, std::string const&);
        static void writeAllMessage(std::vector<int> const&, std::string const&);
        static std::string intToString(int);
        static int portIsValid(std::string const& port);
        static std::string welcome();
};

#endif
