#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "RequestLine.hpp"
# include "HttpHeaders.hpp"
# include "RequestParams.hpp"
# include "HttpRequestReader.hpp"

class HttpRequest
{
	private:
		HttpRequestReader *reader;
		RequestLine *request_line;
		HttpHeaders *request_headers;
		RequestParams *request_params;

		void parseRequestLine();
		void parseRequestHeaders();
		void parseRequestParams();

	public:
		HttpRequest(int socket_fd);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getQueryString() const;
		std::string getHeader(std::string header) const;
		std::string getParameter(std::string param) const;

		// 테스트용
		std::map<std::string, std::string>::iterator getHeadersBegin() const;
		std::map<std::string, std::string>::iterator getHeadersEnd() const;
		std::map<std::string, std::string>::iterator getParamsBegin() const;
		std::map<std::string, std::string>::iterator getParamsEnd() const;
};

#endif