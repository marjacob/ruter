#ifndef __H_02A3D9EFFE5D4114AD1DD17CC5B29223__
#define __H_02A3D9EFFE5D4114AD1DD17CC5B29223__

#include <list>
#include <memory>
#include "Curl/CurlClient.hpp"
#include "Ruter/Place.hpp"

namespace Ruter {

class Ruter {
public:
	Ruter();
	virtual ~Ruter();
	
	std::shared_ptr<Place>
	GetStop(const std::string& id);
	
	std::shared_ptr<std::list<std::shared_ptr<Place>>>
	GetPlaces(const std::string& search, Location *location);
private:
	Curl::CurlClient m_curl;
};

} /* namespace Ruter */

#endif
