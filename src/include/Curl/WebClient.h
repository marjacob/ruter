#ifndef __H_7A00FBFB63404EB2BE886CDE02CDC19D__
#define __H_7A00FBFB63404EB2BE886CDE02CDC19D__

#include <cstddef>
#include <curl/curl.h>
#include <memory>
#include <string>
#include "Parameter.h"
#include "WebRequest.h"

namespace Curl {

typedef std::size_t (*CurlReadCallback)
	(void*, std::size_t, std::size_t, void*);

typedef std::size_t (*CurlWriteCallback)
	(char*, std::size_t, std::size_t, void*);

class WebClient {
public:
	WebClient();
	virtual ~WebClient();
	
	void SetHttpHeaders(struct curl_slist *headers);
	void SetSslVerifyHost(bool do_verify);
	void SetSslVerifyPeer(bool do_verify);
	void SetSslVersion(int version);
	void SetUserAgent(const std::string& useragent);
	std::unique_ptr<std::string> Request(const WebRequest& request);
private:
	CURL *m_curl;
	char m_error_buffer[CURL_ERROR_SIZE];
	std::vector<char> m_buffer;
	struct curl_slist *m_header;
	
	void OnRead(char *data, std::size_t size);
	void OnWrite(char *data, std::size_t size);
	void OnRequest(const WebRequest& request);
	void OnRequestCompleted(const WebRequest& request);
	
	void SetMethod(const std::string& method);
	void SetReadCallback(CurlReadCallback read);
	void SetReadData(void *userdata);
	void SetUrl(const std::string& url);
	void SetWriteCallback(CurlWriteCallback write);
	void SetWriteData(void *user_data);
};

} /* namespace Curl */

#endif
