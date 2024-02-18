# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sangwoki <sangwoki@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/09 20:34:46 by sangwoki          #+#    #+#              #
#    Updated: 2023/09/12 21:29:31 by sangwoki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
SRCS = test_main.cpp \
	HttpRequest/HttpHeaders.cpp	HttpRequest/HttpRequest.cpp HttpRequest/HttpRequestHandler.cpp \
	HttpRequest/HttpRequestParser.cpp HttpRequest/RequestLine.cpp HttpRequest/RequestParams.cpp \
	HttpRequest/RequestUtility.cpp HttpRequest/HttpRequestFactory.cpp \
	controllers/Controller.cpp controllers/ControllerMapping.cpp controllers/MyController.cpp controllers/DefaultController/cpp\
	ResponseHandler/FrontController.cpp ResponseHandler/HttpResponse.cpp ResponseHandler/InterruptApi.cpp ResponseHandler/MultiRequest.cpp \
	ResponseConfig.cpp \
	Bean/BeanFactory.cpp Bean/ReadEventBean.cpp Bean/RecvEventBean.cpp Bean/SendEventBean.cpp Bean/WriteEventBean.cpp Bean/HttpHandler.cpp \
	
OBJS = $(SRCS:.cpp=.o)
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
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