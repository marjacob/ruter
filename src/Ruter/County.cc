#include <string>
#include <utility>
#include "Curl/Parameter.h"
#include "Ruter/County.h"

using std::move;
using std::string;

namespace Ruter {

County::County(string name) :
	m_name(name)
{
	return;
}

County::County(const County& other) :
	m_name(other.m_name)
{
	return;
}

County::County(County&& other) :
	m_name(move(other.m_name))
{
	return;
}

County::~County()
{
	return;
}

County& County::operator=(const County& other)
{
	if (this != &other) {
		m_name = other.m_name;
	}
	
	return *this;
}

County& County::operator=(County&& other)
{
	if (this != &other) {
		m_name = move(other.m_name);
	}
	
	return *this;
}

void County::SetName(string name)
{
	m_name = name;
}

string County::GetName()
{
	return m_name;
}

int County::CreateParameter(int token, Curl::Parameter& param) const
{
	param.SetKey("counties", false);
	param.SetValue(m_name, true);
	
	return token;
}

} /* namespace Ruter */
