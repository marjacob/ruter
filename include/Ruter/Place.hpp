#ifndef __H_65983CBE181D4825A81B0601BEA3A973__
#define __H_65983CBE181D4825A81B0601BEA3A973__

#include <cinttypes>
#include <string>
#include "Curl/Parameter.hpp"
#include "Curl/IParameter.hpp"
#include "Ruter/PlaceType.hpp"

namespace Ruter {

class Place {
public:
	explicit Place();
	explicit Place(const Place& other);
	explicit Place(Place&& other);
	
	virtual ~Place();
	
	Place& operator=(const Place& other);
	Place& operator=(Place&& other);
	
	void SetId(std::uint32_t id);
	std::uint32_t GetId();
	
	void SetName(std::string name);
	std::string GetName();
	
	void SetDistrict(std::string district);
	std::string GetDistrict();
	
	void SetType(PlaceType type);
	PlaceType GetType();
private:
	std::uint32_t m_id;
	std::string m_name;
	std::string m_district;
	PlaceType m_type;
};

} /* namespace Ruter */


#endif
