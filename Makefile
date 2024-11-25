NAME = ircserv
CC = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror
SRC_DIR = srcs
OBJ_DIR = obj
SRC_FILES := ./srcs/Channel.cpp \
		./srcs/main.cpp \
		./srcs/Server.cpp \
		./srcs/Utils.cpp \
		./srcs/UtilsServer.cpp \
		./srcs/commands/BOT.cpp \
		./srcs/commands/CAP.cpp \
		./srcs/commands/INVITE.cpp \
		./srcs/commands/JOIN.cpp \
		./srcs/commands/KICK.cpp \
		./srcs/commands/MODE.cpp \
		./srcs/commands/NICK.cpp \
		./srcs/commands/NOTICE.cpp \
		./srcs/commands/PART.cpp \
		./srcs/commands/PASS.cpp \
		./srcs/commands/PRIVMSG.cpp \
		./srcs/commands/QUIT.cpp \
		./srcs/commands/TOPIC.cpp \
		./srcs/commands/USER.cpp \
		./srcs/commands/WHO.cpp
			 

OBJ_FILES := ./obj/Channel.o \
             ./obj/main.o \
             ./obj/Server.o \
             ./obj/Utils.o \
             ./obj/UtilsServer.o \
             ./obj/commands/BOT.o \
             ./obj/commands/CAP.o \
             ./obj/commands/INVITE.o \
             ./obj/commands/JOIN.o \
             ./obj/commands/KICK.o \
             ./obj/commands/MODE.o \
             ./obj/commands/NICK.o \
             ./obj/commands/NOTICE.o \
             ./obj/commands/PART.o \
             ./obj/commands/PASS.o \
             ./obj/commands/PRIVMSG.o \
             ./obj/commands/QUIT.o \
             ./obj/commands/TOPIC.o \
             ./obj/commands/USER.o \
             ./obj/commands/WHO.o

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "\n\033[32m   Server Ready... \033[0m\n  "

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(dir $(OBJ_FILES))

bonus:
	@$(CC) $(CFLAGS) $(SRC_DIR)/bot/*.cpp $(SRC_DIR)/Utils.cpp $^ -o bot
	@echo "\n\033[32m   Bot Ready... \033[0m\n  "

clean:
	@rm -rf $(NAME) bot
	@echo "\n\033[32m   Clean Working... \033[0m\n  "

fclean:
	@rm -rf $(NAME) bot
	@rm -rf $(OBJ_DIR)
	@echo "\n\033[34m   Fclean Working... \033[0m\n  "

re: fclean all

.PHONY: all clean fclean re bonus
