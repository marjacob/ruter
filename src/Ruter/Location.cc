#include <sstream>
#include <string>
#include <utility>
#include "Curl/Parameter.h"
#include "Ruter/Location.h"

using std::move;
using std::string;
using std::ostringstream;

namespace Ruter {

Location::Location(double x, double y) :
	m_x(x),
	m_y(y)
{
	return;
}

Location::Location(const Location& other) : 
	m_x(other.m_x), 
	m_y(other.m_y)
{
	return;
}

Location::Location(Location&& other) : 
	m_x(move(other.m_x)), 
	m_y(move(other.m_y))
{
	other.m_x = 0.0;
	other.m_y = 0.0;
}

Location::~Location()
{
	
}

Location& Location::operator=(const Location& other)
{
	if (this != &other) {
		m_x = other.m_x;
		m_y = other.m_y;
	}
	
	return *this;
}

Location& Location::operator=(Location&& other)
{
	if (this != &other) {
		m_x = move(other.m_x);
		m_y = move(other.m_y);
	}
	
	return *this;
}

int Location::CreateParameter(int token, Curl::Parameter& param) const
{
	ostringstream s;
	s << "(x=" << m_x << "," << "y=" << m_y << ")";
	param.SetKey("location", false);
	param.SetValue(s.str(), false);
	
	return token;
}

} /* namespace Ruter */
