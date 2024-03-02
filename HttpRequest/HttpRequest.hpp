
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

	public:
		HttpRequest();
		HttpRequest(const HttpRequest& ref);
		HttpRequest& operator=(const HttpRequest& ref);
		~HttpRequest();

		std::string getMethod() const;
		std::string getPath() const;
		std::string getQueryString() const;
		std::string getProtocolString() const;
		std::string getHeader(const std::string& header) const;
		std::string getBody() const;
		std::string getParameter(const std::string& param) const;

		void setRequestLine(RequestLine *line);
		void setMethod(const std::string& method);
		void setPath(const std::string& path);
		void setRequestHeaders(HttpHeaders *headers);
		int addHeader(const std::string& line);
		void setHeader(const std::string& field, const std::string& value);
		void removeHeader(const std::string& field);
		void setRequestBody(const std::string& body);
		void addRequestBody(const std::string& body);
		void setRequestParams(RequestParams *params);
		int addRequestParamsToQueryString(const std::string& query_string);

		// 테스트용
		void printAllHeader() const;
		void printAllParams() const;
};

#endif