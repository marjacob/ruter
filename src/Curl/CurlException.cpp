#include <curl/curl.h>
#include "Curl/CurlException.hpp"

namespace Curl
{
	CurlException::CurlException(CURLcode errornum)
		: runtime_error(curl_easy_strerror(errornum))
	{
		m_errornum = errornum;
	}
	
	CURLcode CurlException::errornum() const
	{
		return m_errornum;
	}
	
	void CurlException::OnFailure(CURLcode errornum)
	{
		if (CURLE_OK != errornum) {
			throw CurlException(errornum);
		}
	}
}
