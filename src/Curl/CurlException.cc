#include <curl/curl.h>
#include <string>
#include "Curl/CurlException.h"

using std::string;

namespace Curl {

CurlException::CurlException(CURLcode errornum)
	: CurlException(errornum, curl_easy_strerror(errornum))
{
	
}

CurlException::CurlException(CURLcode errornum, const string& message)
	: runtime_error(message)
{
	m_errornum = errornum;
}

CurlException::~CurlException()
{
	/* default */
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

} /* namespace Curl */
