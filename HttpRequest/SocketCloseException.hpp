#ifndef SOCKET_CLOSE_EXCEPTION_HPP
# define SOCKET_CLOSE_EXCEPTION_HPP

# include <exception>

class SocketCloseException400 : public std::exception
{
	public:
		const char *what() const noexcept override
		{
			return "disconnect from the client with 400 response!";
		}
};

class SocketCloseException500 : public std::exception
{
	public:
		const char *what() const noexcept override
		{
			return "disconnect from the client with 500 response!";
		}
};

#endif