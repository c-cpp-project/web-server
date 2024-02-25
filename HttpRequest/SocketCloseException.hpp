#ifndef SOCKET_CLOSE_EXCEPTION_HPP
# define SOCKET_CLOSE_EXCEPTION_HPP

# include <exception>

class SocketCloseException400 : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "400";
		}
};

class SocketCloseException500 : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "500";
		}
};

class ClientSocketCloseException : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "client close socket.";
		}
}

#endif