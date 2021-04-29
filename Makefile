EXEC = webserv

SERVER_DIR = server/
PARSER_DIR = parser/

REQUEST_DIR = request/

RESPONSE_DIR = response/

UTILS_DIR = utils/

SRCS = main.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp \
				$(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)public.cpp $(REQUEST_DIR)private.cpp\
				$(RESPONSE_DIR)public.cpp $(RESPONSE_DIR)utils.cpp $(RESPONSE_DIR)add.cpp $(RESPONSE_DIR)method.cpp $(RESPONSE_DIR)get.cpp\
				$(UTILS_DIR)utils.cpp $(PARSER_DIR)coplien.cpp $(PARSER_DIR)operator.cpp $(PARSER_DIR)private.cpp $(PARSER_DIR)public.cpp

#file with fonction use to debug
DEBUG_SRCS = $(REQUEST_DIR)debug.cpp $(RESPONSE_DIR)debug.cpp

OBJS = $(SRCS:.cpp=.o)
DEBUG_OBJS = $(DEBUG_SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = -I ./include -I ./libft
DEBUG_ARG = -fsanitize=address -g
CPPFLAGS = $(INCLUDE) -std=c++98 -DDEBUG=1 -Wall -Wextra -Werror $(DEBUG_ARG)

all : $(EXEC)

$(EXEC) : $(OBJS)
	make -C libft
	$(CXX) -o $(EXEC) $(DEBUG_ARG) $(INCLUDE) $^ libft/libft.a

#compile with debug mode
debug : $(OBJS) $(DEBUG_OBJS)
	make -C libft
	$(CXX) -o $(EXEC) $(INCLUDE) $^ libft/libft.a

clean :
	make fclean -C libft
	rm -f $(OBJS) $(DEBUG_OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all

re2 : fclean debug
