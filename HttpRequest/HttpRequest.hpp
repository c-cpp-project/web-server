#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "RequestLine.hpp"
# include "HttpHeaders.hpp"
# include "RequestParams.hpp"

class HttpRequest
{
	private:
		RequestLine *request_line; // 요청 라인
		HttpHeaders *request_headers; // 요청 헤더
		std::string request_body; // 요청 본문
		RequestParams *request_params; // 요청 URI에 붙는 쿼리스트링 & 본문 쿼리스트링을 파싱하여 저장

		int parseRequestLine(const std::string& buffer);
		int parseRequestHeaders(const std::string& buffer, int start);
		void parseRequestBody(const std::string& buffer, int start);

	public:
		std::string getMethod() const;
		std::string getPath() const;
		std::string getQueryString() const; // 테스트용
		std::string getHeader(const std::string& header) const;
		std::string getRequestBody() const;
		std::string getParameter(const std::string& param) const;

		void setRequestLine(RequestLine *line);
		void setRequestHeaders(HttpHeaders *headers);
		int addHeader(const std::string& line);
		void setHeader(const std::string& field, const std::string& value);
		void setRequestBody(const std::string& body);
		void addRequestBody(const std::string& body);
		void setRequestParams(RequestParams *params);
		int addRequestParamsToQueryString(const std::string& query_string);

		// 테스트용
		std::map<std::string, std::string>::iterator getHeadersBegin() const;
		std::map<std::string, std::string>::iterator getHeadersEnd() const;
		std::map<std::string, std::string>::iterator getParamsBegin() const;
		std::map<std::string, std::string>::iterator getParamsEnd() const;
};

#endif