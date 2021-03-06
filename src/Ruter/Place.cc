#include <cinttypes>
#include <string>
#include <utility>
#include "Curl/Parameter.h"
#include "Ruter/Place.h"
#include "Ruter/PlaceType.h"

using std::move;
using std::string;
using std::uint32_t;

namespace Ruter {

Place::Place(uint32_t id, string name, string district, PlaceType type) :
	m_id(id),
	m_name(name),
	m_district(district),
	m_type(type)
{
	return;
}

Place::Place(const Place& other) :
	m_id(other.m_id),
	m_name(other.m_name),
	m_district(other.m_district),
	m_type(other.m_type)
{
	return;
}

Place::Place(Place&& other) :
	m_id(move(other.m_id)),
	m_name(move(other.m_name)),
	m_district(move(other.m_district)),
	m_type(move(other.m_type))
{
	return;
}

Place::~Place()
{
	return;
}

Place& Place::operator=(const Place& other)
{
	if (this != &other) {
		m_id = other.m_id;
		m_name = other.m_name;
		m_district = other.m_district;
		m_type = other.m_type;
	}
	
	return *this;
}

Place& Place::operator=(Place&& other)
{
	if (this != &other) {
		m_name = move(other.m_id);
		m_name = move(other.m_name);
		m_district = move(other.m_district);
		m_type = move(other.m_type);
	}
	
	return *this;
}

void Place::SetId(uint32_t id)
{
	m_id = id;
}

uint32_t Place::GetId()
{
	return m_id;
}

void Place::SetName(string name)
{
	m_name = name;
}

string Place::GetName()
{
	return m_name;
}

void Place::SetDistrict(string district)
{
	m_district = district;
}

string Place::GetDistrict()
{
	return m_district;
}

void Place::SetType(PlaceType type)
{
	m_type = type;
}

PlaceType Place::GetType()
{
	return m_type;
}

} /* namespace Ruter */
