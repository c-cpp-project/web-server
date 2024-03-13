#ifndef SOCKET_CLOSE_EXCEPTION_HPP
# define SOCKET_CLOSE_EXCEPTION_HPP

# include <exception>

class EscapeHandleException400 : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "400";
		}
};

class EscapeHandleException411 : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "411";
		}
};

class EscapeHandleException413 : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "413";
		}
};

class EscapeHandleException431 : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "431";
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

class ClientCloseSocketException : public std::exception
{
	public:
		const char *what() const throw()
		{
			return "client close socket.";
		}
};

#endif