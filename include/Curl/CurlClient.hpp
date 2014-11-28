#ifndef __H_7A00FBFB63404EB2BE886CDE02CDC19D__
#define __H_7A00FBFB63404EB2BE886CDE02CDC19D__

#include <cstddef>
#include <curl/curl.h>
#include <memory>
#include <string>
#include "Curl/Parameter.hpp"
#include "Curl/ParameterCollection.hpp"

namespace Curl {

typedef std::size_t (*CurlReadCallback)
	(void*, std::size_t, std::size_t, void*);

typedef std::size_t (*CurlWriteCallback)
	(char*, std::size_t, std::size_t, void*);

class CurlClient {
public:
	CurlClient(std::string url);
	
	void SetHttpHeaders(struct curl_slist *headers);
	void SetRequestParameters(ParameterCollection *params);
	void SetRequestUri(const std::string& uri);
	void SetSslVerifyHost(bool verify);
	void SetSslVerifyPeer(bool verify);
	void SetSslVersion(int version);
	void SetUserAgent(const std::string& useragent);
	std::shared_ptr<std::string> Request();
	
	~CurlClient();
private:
	CURL *m_curl;
	ParameterCollection *m_params;
	std::string m_baseUrl;
	std::string m_resourceUri;
	std::vector<char> m_buffer;
	struct curl_slist *m_header;
	
	void SetReadCallback(CurlReadCallback read);
	void SetReadData(void *userdata);
	void SetWriteCallback(CurlWriteCallback write);
	void SetWriteData(void *userdata);
	
	void OnRead(char *data, std::size_t size);
	void OnWrite(char *data, std::size_t size);
	void OnRequest();
	void OnRequestCompleted(std::shared_ptr<std::string> data);
};

} /* namespace Curl */

#endif
