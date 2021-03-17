#
#
#There is maybe mistake inside makefile I can t verif we didn t have make for now it is just a structure to do it
#
#

EXEC = webserv

SERVER_DIR = server/

SRCS = $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp

OBJS = $(SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = include/
FLAGS = -Wall -Wextra -Werror

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) -o $@ $(INCLUDE) $(FLAGS) $^

clean : 
	rm -f $(OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all