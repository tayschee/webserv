EXEC = webserv

SERVER_DIR = server/
MESSAGE_DIR = message/
PARSER_DIR = parser/
REQUEST_DIR = request/
RESPONSE_DIR = response/

UTILS_DIR = utils/

#SRCS = main.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp \
				$(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)public.cpp $(REQUEST_DIR)private.cpp\
				$(RESPONSE_DIR)public.cpp $(RESPONSE_DIR)utils.cpp $(RESPONSE_DIR)add.cpp $(RESPONSE_DIR)method.cpp $(RESPONSE_DIR)get.cpp\
				$(UTILS_DIR)utils.cpp

SRCS = main.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)coplien.cpp $(SERVER_DIR)operator.cpp $(SERVER_DIR)public.cpp $(SERVER_DIR)private.cpp \
				$(MESSAGE_DIR)coplien.cpp $(MESSAGE_DIR)parse.cpp $(MESSAGE_DIR)getter.cpp $(MESSAGE_DIR)utils.cpp $(MESSAGE_DIR)exchange_management.cpp\
				$(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)public.cpp $(REQUEST_DIR)getter.cpp $(REQUEST_DIR)parse.cpp $(REQUEST_DIR)private.cpp \
				$(RESPONSE_DIR)coplien.cpp $(RESPONSE_DIR)static_variable.cpp $(RESPONSE_DIR)getter.cpp $(RESPONSE_DIR)method.cpp \
				$(RESPONSE_DIR)find.cpp $(RESPONSE_DIR)utils.cpp $(RESPONSE_DIR)error.cpp $(RESPONSE_DIR)add.cpp \
				$(UTILS_DIR)utils.cpp
	
#$(PARSER_DIR)public.cpp $(PARSER_DIR)coplien.cpp $(PARSER_DIR)private.cpp $(PARSER_DIR)operator.cpp

OBJS = $(SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = -I ./include
CPPFLAGS = $(INCLUDE) -std=c++98 -D DEBUG=1 -Wall -Wextra -Werror

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) -o $(EXEC) $(INCLUDE) $^

clean :
	make fclean -C libft
	rm -f $(OBJS) $(DEBUG_OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all
