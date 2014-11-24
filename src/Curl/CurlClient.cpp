#include <cstddef>
#include <memory>
#include <string>
#include "Curl/CurlClient.hpp"
#include "Curl/CurlBuffer.hpp"
#include "Curl/CurlException.hpp"
#include "Curl/ParameterCollection.hpp"

using std::make_shared;
using std::shared_ptr;
using std::size_t;
using std::string;
using std::vector;

static size_t write(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	Curl::CurlBuffer *buf = static_cast<Curl::CurlBuffer*>(userdata);

	size_t cbChunk = (size * nmemb);
	size_t cbTotal = (cbChunk + buf->BufferSize);

	if (cbTotal + 1 > buf->BufferCapacity) {
		buf->Buffer.resize(cbTotal + 1, '\0');
		buf->BufferCapacity = cbTotal;
	}

	buf->Buffer.insert(buf->Buffer.end(), ptr, ptr + cbChunk);
	buf->BufferSize += cbChunk;
	
	return cbChunk;
}

static size_t read(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	return 0;
}

namespace Curl
{
	CurlClient::CurlClient(string url) :
		m_baseUrl(url), 
		m_params(nullptr),
		m_resourceUri("")
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
		SetWriteData(&m_buf);

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

	void CurlClient::SetRequestParameters(ParameterCollection *params)
	{
		m_params = params;
	}

	void CurlClient::SetRequestUri(const string& uri)
	{
		m_resourceUri = uri;
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

	shared_ptr<string> CurlClient::Request()
	{
		OnRequest();

		m_buf.BufferSize = 0;
		CurlException::OnFailure(curl_easy_perform(m_curl));

		shared_ptr<string> data = make_shared<string>(
			m_buf.Buffer.begin(),
			m_buf.Buffer.end());

		OnRequestCompleted(data);

		return data;
	}

	/***** PRIVATE METHODS ***************************************/

	void CurlClient::OnRequest()
	{
		string url = m_baseUrl + "/" + m_resourceUri;
		
		if (m_params && m_params->Size() > 0) {
			url += "/" + m_params->ToString();
		}

		CurlException::OnFailure(
			curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str())
		);
	}

	void CurlClient::OnRequestCompleted(shared_ptr<string> data)
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
}

