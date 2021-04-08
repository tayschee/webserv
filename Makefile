#
#
#There is maybe mistake inside makefile I can t verif we didn t have make for now it is just a structure to do it
#
#

EXEC = webserv

SERVER_DIR = server/
PARSER_DIR = parser/

SRCS = main.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp $(PARSER_DIR)public.cpp $(PARSER_DIR)coplien.cpp $(PARSER_DIR)private.cpp $(PARSER_DIR)operator.cpp

OBJS = $(SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = -I ./include
CPPFLAGS = $(INCLUDE) -std=c++98 -Wall -Wextra -Werror
CXXFLAGS = -fsanitize=address -g
LDFLAGS = -fsanitize=address -g

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) -o $@ $(INCLUDE) $(LDFLAGS) $(FLAGS) $^

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all
