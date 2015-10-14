#ifndef __H_2815E86E3640413ABF538FF3F6D57AD4__
#define __H_2815E86E3640413ABF538FF3F6D57AD4__

#include <curl/curl.h>
#include <stdexcept>
#include <string>

namespace Curl {

class CurlException : public std::runtime_error {
public:
	explicit CurlException(CURLcode errornum);

	CurlException(CURLcode errornum, const std::string& message);

	virtual ~CurlException();

	CURLcode errornum() const;
	static void OnFailure(CURLcode errornum);
private:
	CURLcode m_errornum;
};

} /* namespace Curl */

#endif
