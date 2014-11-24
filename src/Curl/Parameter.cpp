#include <curl/curl.h>
#include <string>
#include "Curl/Parameter.hpp"

using std::string;

namespace Curl
{
	string Parameter::GetKey()
	{
		return m_key;
	}
	
	string Parameter::GetValue()
	{
		return m_value;
	}
	
	void Parameter::SetKey(string key, bool escape)
	{
		m_escapeKey = escape;
		m_key = key;
	}
	
	void Parameter::SetValue(string value, bool escape)
	{
		m_escapeValue = escape;
		m_value = value;
	}
	
	string Parameter::ToString()
	{
		string key = m_escapeKey ? Escape(m_key) : m_key;
		string value = m_escapeValue ? Escape(m_value) : m_value;
		
		return key + "=" + value;
	}
	
	string Parameter::Escape(string value)
	{
		char *escaped = curl_easy_escape(
			NULL, value.c_str(), value.length());
		
		string esc = string(escaped);
		curl_free(escaped);
		
		return esc;
	}
}
