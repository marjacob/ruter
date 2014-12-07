#include <ctime>
#include <string>
#include <utility>
#include "Curl/Parameter.hpp"
#include "Ruter/Time.hpp"

using std::move;
using std::string;
using std::time_t;
using std::tm;

namespace Ruter {

Time::Time(bool isDeparture) :
	m_isDeparture(isDeparture)
{
	m_time = std::time(NULL);
	return;
}

Time::Time(time_t time, bool isDeparture) :
	m_time(time),
	m_isDeparture(isDeparture)
{
	return;
}

Time::Time(tm& time, bool isDeparture) :
	m_isDeparture(isDeparture)
{
	m_time = std::mktime(&time);
	return;
}

Time::Time(const Time& other) :
	m_isDeparture(other.m_isDeparture),
	m_time(other.m_time)
{
	return;
}

Time::Time(Time&& other) :
	m_isDeparture(move(other.m_isDeparture)),
	m_time(move(other.m_time))
{
	return;
}

Time::~Time()
{
	return;
}

const Time Time::operator--(int)
{
	Time time(*this);
	--(*this);
	return time;
}

const Time Time::operator++(int)
{
	Time time(*this);
	++(*this);
	return time;
}

Time& Time::operator--()
{
	m_time -= 60;
	return *this;
}

Time& Time::operator++()
{
	m_time += 60;
	return *this;
}

Time& Time::operator=(Time&& other)
{
	if (this != &other) {
		m_isDeparture = move(other.m_isDeparture);
		m_time = move(other.m_time);
	}
	
	return *this;
}

Time& Time::operator=(const Time& other)
{
	if (this != &other) {
		m_isDeparture = other.m_isDeparture;
		m_time = other.m_time;
	}
	
	return *this;
}

int Time::CreateParameter(int token, Curl::Parameter& param) const
{
	if (!token) {
		param.SetKey("time", false);
		param.SetValue(Format(), false);
		return ++token;
	} else {
		string isAfter = m_isDeparture ? "true" : "false";
		param.SetKey("isAfter", false);
		param.SetValue(isAfter, false);
	}
	
	return --token;
}

string Time::Format() const
{
	tm time = *std::localtime(&m_time);
	char buf[15] = { '\0' }; /* ddmmYYYYhhmmss = 14 chars + \0 */
	std::strftime(buf, sizeof(buf), "%d%m%Y%H%M%S", &time);
	return string(buf, sizeof(buf));
}	

} /* namespace Ruter */
