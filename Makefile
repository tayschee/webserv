#
#
#There is maybe mistake inside makefile I can t verif we didn t have make for now it is just a structure to do it
#
#

EXEC = webserv

SERVER_DIR = server/

REQUEST_DIR = request/

UTILS_DIR = utils/

SRCS = main.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp \
				$(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)public.cpp $(REQUEST_DIR)private.cpp \
				$(UTILS_DIR)utils.cpp

#file with fonction use to debug
DEBUG_SRCS = $(REQUEST_DIR)debug.cpp

OBJS = $(SRCS:.cpp=.o)
DEBUG_OBJS = $(DEBUG_SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = -I ./include -I ./libft
CPPFLAGS = $(INCLUDE) -std=c++98 -D DEBUG=1 #-Wall -Wextra -Werror

all : $(EXEC)

$(EXEC) : $(OBJS)
	make -C libft
	$(CXX) -o $(EXEC) $(INCLUDE) $^ libft/libft.a

#compile with debug mode
debug : $(OBJS) $(DEBUG_OBJS)
	make -C libft
	$(CXX) -o $(EXEC) $(INCLUDE) $^ libft/libft.a

clean : 
	rm -f $(OBJS) $(DEBUG_OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all

re2 : fclean debug