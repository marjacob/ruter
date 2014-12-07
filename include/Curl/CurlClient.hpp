#ifndef __H_7A00FBFB63404EB2BE886CDE02CDC19D__
#define __H_7A00FBFB63404EB2BE886CDE02CDC19D__

#include <cstddef>
#include <curl/curl.h>
#include <memory>
#include <string>
#include "Curl/Parameter.hpp"
#include "Curl/WebRequest.hpp"

namespace Curl {

typedef std::size_t (*CurlReadCallback)
	(void*, std::size_t, std::size_t, void*);

typedef std::size_t (*CurlWriteCallback)
	(char*, std::size_t, std::size_t, void*);

class CurlClient {
public:
	CurlClient();
	
	void SetHttpHeaders(struct curl_slist *headers);
	void SetSslVerifyHost(bool verify);
	void SetSslVerifyPeer(bool verify);
	void SetSslVersion(int version);
	void SetUserAgent(const std::string& useragent);
	std::unique_ptr<std::string> Request(const WebRequest& request);
	
	~CurlClient();
private:
	CURL *m_curl;
	std::vector<char> m_buffer;
	struct curl_slist *m_header;
	
	void SetReadCallback(CurlReadCallback read);
	void SetReadData(void *userdata);
	void SetWriteCallback(CurlWriteCallback write);
	void SetWriteData(void *userdata);
	
	void OnRead(char *data, std::size_t size);
	void OnWrite(char *data, std::size_t size);
	void OnRequest(const WebRequest& request);
	void OnRequestCompleted(const WebRequest& request);
};

} /* namespace Curl */

#endif
