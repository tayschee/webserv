#
#
#There is maybe mistake inside makefile I can t verif we didn t have make for now it is just a structure to do it
#
#

EXEC = webserv

SERVER_DIR = server/

REQUEST_DIR = request/

SRCS = main.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp \
				#$(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)public.cpp $(REQUEST_DIR)private.cpp

OBJS = $(SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = -I ./include -I ./libft
CPPFLAGS = $(INCLUDE) -std=c++98 #-Wall -Wextra -Werror

all : $(EXEC)

$(EXEC) : $(OBJS)
	make -C libft
	$(CXX) -o $@ $(INCLUDE) -L libft/ $(FLAGS) $^

clean : 
	rm -f $(OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all