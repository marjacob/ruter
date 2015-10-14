#include <cstddef>
#include <curl/curl.h>
#include <memory>
#include <string>
#include "Curl/CurlException.hpp"
#include "Curl/WebClient.hpp"
#include "Curl/WebRequest.hpp"

using std::size_t;
using std::string;
using std::unique_ptr;
using std::vector;

static size_t write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t cbChunk = (size * nmemb);
	auto *buf = static_cast<std::vector<char>*>(userdata);
	buf->insert(buf->end(), ptr, ptr + cbChunk);
	return cbChunk;
}

static size_t read(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	return 0;
}

namespace Curl {

WebClient::WebClient() :
	m_buffer(),
	m_curl(nullptr),
	m_header(nullptr)
{
	CURLcode code = CURLE_OK;
	struct curl_slist *hdr = NULL;

	if (0 != (code = curl_global_init(CURL_GLOBAL_ALL))) {
		throw CurlException(code);
	}

	if (!(m_curl = curl_easy_init())) {
		curl_global_cleanup();
		throw CurlException(CURLE_FAILED_INIT);
	}

	hdr = curl_slist_append(hdr, "Accept: application/json");
	hdr = curl_slist_append(hdr, "Accept-Charset: utf-8");
	
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, m_error_buffer));

	SetHttpHeaders(hdr);
	SetSslVerifyHost(true);
	SetSslVerifyPeer(true);
	//SetSslVersion(CURL_SSLVERSION_SSLv3);
	SetUserAgent("PosixRuter++/0.1");
	SetWriteCallback(write);
	SetWriteData(&m_buffer);
}

WebClient::~WebClient()
{
	curl_easy_cleanup(m_curl);
	curl_global_cleanup();
	curl_slist_free_all(m_header);
}

void WebClient::SetHttpHeaders(struct curl_slist *headers)
{
	if (m_header) {
		curl_slist_free_all(m_header);
	}

	m_header = headers;
	const CURLoption option = CURLOPT_HTTPHEADER;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, m_header));
}

void WebClient::SetSslVerifyHost(bool do_verify)
{
	const CURLoption option = CURLOPT_SSL_VERIFYHOST;
	const long verify = do_verify ? 2L : 0L;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, verify));
}

void WebClient::SetSslVerifyPeer(bool do_verify)
{
	const CURLoption option = CURLOPT_SSL_VERIFYPEER;
	const long verify = do_verify ? 1L : 0L;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, verify));
}

void WebClient::SetSslVersion(int version)
{
	const CURLoption option = CURLOPT_SSLVERSION;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, version));
}

void WebClient::SetUserAgent(const string& user_agent)
{
	const CURLoption option = CURLOPT_USERAGENT;
	const char *agent = user_agent.c_str();
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, agent));
}

unique_ptr<string> WebClient::Request(const WebRequest& request)
{
	SetMethod(request.GetMethod());
	SetUrl(request.ToString());
	m_error_buffer[0] = 0;
	CURLcode rc = curl_easy_perform(m_curl);

	if (CURLE_OK != rc) {
		if (m_error_buffer[0]) {
			throw CurlException(rc, m_error_buffer);
		} else {
			throw CurlException(rc);
		}
	}

	/* Extract the data returned from the server. */
	unique_ptr<string> data = unique_ptr<string>(
		new string(m_buffer.begin(), m_buffer.end()));
	
	m_buffer.clear();

	return data;
}

/***** PRIVATE METHODS ********************************************************/

void WebClient::SetMethod(const string& method)
{
	const CURLoption option = CURLOPT_CUSTOMREQUEST;
	CURLcode code = curl_easy_setopt(m_curl, option, method.c_str());
	CurlException::OnFailure(code);
}

void WebClient::SetReadCallback(CurlReadCallback read)
{
	const CURLoption option = CURLOPT_READFUNCTION;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, read));
}

void WebClient::SetReadData(void *userdata)
{
	const CURLoption option = CURLOPT_READDATA;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, userdata));
}

void WebClient::SetUrl(const string& url)
{
	const CURLoption option = CURLOPT_URL;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, url.c_str()));
}

void WebClient::SetWriteCallback(CurlWriteCallback write)
{
	const CURLoption option = CURLOPT_WRITEFUNCTION;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, write));
}

void WebClient::SetWriteData(void *user_data)
{
	const CURLoption option = CURLOPT_WRITEDATA;
	CurlException::OnFailure(curl_easy_setopt(m_curl, option, user_data));
}

} /* namespace Curl */
