#ifndef __H_02A3D9EFFE5D4114AD1DD17CC5B29223__
#define __H_02A3D9EFFE5D4114AD1DD17CC5B29223__

#include <memory>
#include "Curl/CurlClient.hpp"
#include "Ruter/Place.hpp"

namespace Ruter
{
	class Ruter {
	public:
		Ruter();
		virtual ~Ruter();
		
		std::shared_ptr<Place> GetStop(const std::string& id);
	private:
		Curl::CurlClient m_curl;
	};
}

#endif
