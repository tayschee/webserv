EXEC = webserv

SRCS_DIR = srcs/

CLUSTER_DIR = $(SRCS_DIR)cluster/
CGI_DIR = $(SRCS_DIR)cgi/
MESSAGE_DIR = $(SRCS_DIR)message/
UTILS_DIR = $(SRCS_DIR)utils/
PARSER_DIR = $(SRCS_DIR)parser/

SERVER_DIR = $(CLUSTER_DIR)server/
CLIENT_DIR = $(CLUSTER_DIR)client/

EXCHANGE_DIR = $(MESSAGE_DIR)exchange_management/
REQUEST_DIR = $(MESSAGE_DIR)request/
RESPONSE_DIR = $(MESSAGE_DIR)response/

SRCS = main.cpp $(EXCHANGE_DIR)receive_function.cpp $(EXCHANGE_DIR)receive_header.cpp $(EXCHANGE_DIR)receive_tf.cpp $(EXCHANGE_DIR)receive_cl.cpp\
				$(EXCHANGE_DIR)exchange_management.cpp $(EXCHANGE_DIR)receive_management.cpp $(EXCHANGE_DIR)receive_function.cpp\
				$(REQUEST_DIR)coplien.cpp $(REQUEST_DIR)static_variable.cpp $(REQUEST_DIR)public.cpp $(REQUEST_DIR)getter.cpp $(REQUEST_DIR)parse.cpp $(REQUEST_DIR)private.cpp $(REQUEST_DIR)exception.cpp\
				$(RESPONSE_DIR)find.cpp $(RESPONSE_DIR)static_variable.cpp $(RESPONSE_DIR)utils.cpp $(RESPONSE_DIR)add.cpp \
				$(RESPONSE_DIR)coplien.cpp $(RESPONSE_DIR)error.cpp $(RESPONSE_DIR)getter.cpp $(RESPONSE_DIR)method.cpp\
				$(PARSER_DIR)coplien.cpp $(PARSER_DIR)operator.cpp $(PARSER_DIR)private.cpp $(PARSER_DIR)public.cpp\
				$(MESSAGE_DIR)coplien.cpp $(MESSAGE_DIR)parse.cpp $(MESSAGE_DIR)getter.cpp $(MESSAGE_DIR)utils.cpp\
				$(CLIENT_DIR)coplien.cpp $(CLIENT_DIR)getter.cpp $(CLIENT_DIR)private.cpp $(CLIENT_DIR)public.cpp\
				$(CLUSTER_DIR)public.cpp $(CLUSTER_DIR)private.cpp $(CLUSTER_DIR)coplien.cpp\
				$(SERVER_DIR)utils.cpp $(SERVER_DIR)coplien.cpp \
				$(CGI_DIR)coplien.cpp $(CGI_DIR)private.cpp\
				$(UTILS_DIR)utils.cpp

OBJS = $(SRCS:.cpp=.o)

#To activate implicit rules to compile in cpp use CXX
CXX = clang++
INCLUDE = -I ./include
CPPFLAGS = $(INCLUDE) -std=c++98 -D DEBUG=1 -Wall -Wextra -Werror -fsanitize=address -g
# -fsanitize=address -g

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(CXX) -o $(EXEC) $(INCLUDE) $^ -fsanitize=address -g
#-fsanitize=address -g
clean :
	rm -f $(OBJS) $(DEBUG_OBJS)

fclean : clean
	rm -f $(EXEC)

re : fclean all
