#ifndef __H_02A3D9EFFE5D4114AD1DD17CC5B29223__
#define __H_02A3D9EFFE5D4114AD1DD17CC5B29223__

#include <list>
#include <memory>
#include <string>
#include "Curl/CurlClient.hpp"
#include "Ruter/Place.hpp"

namespace Ruter {

class Ruter {
public:
	Ruter();
	virtual ~Ruter();
	
	std::unique_ptr<Place> GetStop(const std::string& id);
	
	std::list<std::shared_ptr<Place>> GetPlaces(
		const std::string& search,
		const Location *location = nullptr);
private:
	std::string m_api;
	Curl::CurlClient m_curl;
};

} /* namespace Ruter */

#endif
