#include "Curl/CurlClient.hpp"
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include "Curl/CurlException.hpp"
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

CurlClient::CurlClient()
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

	SetHttpHeaders(hdr);
	SetSslVerifyHost(true);
	SetSslVerifyPeer(true);
	SetSslVersion(CURL_SSLVERSION_SSLv3);
	SetUserAgent("PosixRuter++/0.1");
	SetWriteCallback(write);
	SetWriteData(&m_buffer);

	m_header = hdr;
}

void CurlClient::SetHttpHeaders(struct curl_slist *headers)
{
	CurlException::OnFailure(
		curl_easy_setopt(
			m_curl,
			CURLOPT_HTTPHEADER,
			headers
		)
	);
}

void CurlClient::SetSslVerifyHost(bool verify)
{
	const CURLoption option = CURLOPT_SSL_VERIFYHOST;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, verify ? 2L : 0L)
	);
}

void CurlClient::SetSslVerifyPeer(bool verify)
{
	const CURLoption option = CURLOPT_SSL_VERIFYPEER;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, verify ? 1L : 0L)
	);
}

void CurlClient::SetSslVersion(int version)
{
	const CURLoption option = CURLOPT_SSLVERSION;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, version)
	);
}

void CurlClient::SetUserAgent(const string& useragent)
{
	const CURLoption option = CURLOPT_USERAGENT;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, useragent.c_str())
	);
}

unique_ptr<string> CurlClient::Request(const WebRequest& request)
{
	OnRequest(request);
	
	CurlException::OnFailure(curl_easy_perform(m_curl));

	unique_ptr<string> data = unique_ptr<string>(
		new string(m_buffer.begin(), m_buffer.end()));
	m_buffer.clear();
	
	OnRequestCompleted(request);

	return data;
}

/***** PRIVATE METHODS ***************************************/

void CurlClient::OnRequest(const WebRequest& request)
{
	string url = request.ToString();
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str())
	);
}

void CurlClient::OnRequestCompleted(const WebRequest& request)
{
	return;
}

void CurlClient::SetReadCallback(CurlReadCallback read)
{
	const CURLoption option = CURLOPT_READFUNCTION;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, read)
	);
}

void CurlClient::SetReadData(void *userdata)
{
	const CURLoption option = CURLOPT_READDATA;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, userdata)
	);
}

void CurlClient::SetWriteCallback(CurlWriteCallback write)
{
	const CURLoption option = CURLOPT_WRITEFUNCTION;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, write)
	);
}

void CurlClient::SetWriteData(void *userdata)
{
	const CURLoption option = CURLOPT_WRITEDATA;
	CurlException::OnFailure(
		curl_easy_setopt(m_curl, option, userdata)
	);
}

CurlClient::~CurlClient()
{
	curl_easy_cleanup(m_curl);
	curl_global_cleanup();
	curl_slist_free_all(m_header);
}

} /* namespace Curl */
