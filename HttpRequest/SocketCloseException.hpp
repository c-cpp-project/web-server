#ifndef SOCKET_CLOSE_EXCEPTION_HPP
# define SOCKET_CLOSE_EXCEPTION_HPP

# include <exception>

class SocketCloseException : public std::exception
{
	public:
		const char *what() const noexcept override
		{
			return "disconnect from the client!";
		}
};

#endif