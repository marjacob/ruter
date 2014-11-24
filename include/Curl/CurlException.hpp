#ifndef __H_2815E86E3640413ABF538FF3F6D57AD4__
#define __H_2815E86E3640413ABF538FF3F6D57AD4__

#include <curl/curl.h>
#include <stdexcept>

namespace Curl
{
	class CurlException : public std::runtime_error {
	private:
		CURLcode m_errornum;
	public:
		CurlException(CURLcode errornum);
		CURLcode errornum() const;
		static void OnFailure(CURLcode errornum);
	};
}

#endif
