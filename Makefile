# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuikim <yuikim@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/09 20:34:46 by sangwoki          #+#    #+#              #
#    Updated: 2024/02/19 15:34:06 by yuikim           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
SRCS = TestBeanMain.cpp server/setting/ConfigFunctions.cpp server/setting/ConfigParser.cpp server/setting/Location.cpp server/setting/Server.cpp server/ServerConfiguration.cpp server/WebServer.cpp \
	 server/Event.cpp server/StringUtils.cpp server/SocketUtils.cpp \
	 server/mock/Handler.cpp \
	 HttpRequest/HttpRequestFactory.cpp \
	 HttpRequest/HttpRequest.cpp \
	 HttpRequest/HttpRequestParser.cpp \
	 HttpRequest/RequestLine.cpp \
	 HttpRequest/HttpHeaders.cpp \
	 HttpRequest/RequestParams.cpp \
	 HttpRequest/RequestUtility.cpp \
	 HttpRequest/HttpRequestHandler.cpp \
	 controllers/Controller.cpp \
	 controllers/ControllerMapping.cpp \
	 Controllers/DefaultController.cpp \
	 Controllers/MyController.cpp \
	 Controllers/ChildProcess.cpp \
	 ResponseHandler/FrontController.cpp \
	 ResponseHandler/HttpResponse.cpp \
	 ResponseHandler/MultiRequest.cpp \
	 ResponseConfig.cpp \
     Bean/Bean.cpp \
	 Bean/BeanFactory.cpp \
	 Bean/HttpHandler.cpp \
	 Bean/ReadEventBean.cpp \
	 Bean/RecvEventBean.cpp \
	 Bean/SendEventBean.cpp \
	 Bean/WriteEventBean.cpp
	
OBJS = $(SRCS:.cpp=.o)
CXX = c++
CXXFLAGS = -Werror -std=c++98
all : $(NAME)
$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@
clean :
	rm -f $(OBJS)
fclean : clean
	rm -f $(NAME)
re : fclean all
.PHONY: all clean fclean re 