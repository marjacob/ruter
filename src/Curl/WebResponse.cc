#include <memory>
#include <string>
#include <utility>
#include "Curl/WebResponse.h"

using std::move;
using std::shared_ptr;
using std::string;

namespace Curl {

WebResponse::WebResponse() :
	m_response_data(nullptr)
{
	return;
}

WebResponse::WebResponse(const WebResponse& other) :
	m_response_data(other.m_response_data)
{
	return;
}

WebResponse::WebResponse(WebResponse&& other) :
	m_response_data(move(other.m_response_data))
{
	return;
}

WebResponse::~WebResponse()
{
	return;
}

WebResponse& WebResponse::operator=(const WebResponse& other)
{
	if (this != &other) {
		m_response_data = other.m_response_data;
	}
	
	return *this;
}

WebResponse& WebResponse::operator=(WebResponse&& other)
{
	if (this != &other) {
		m_response_data = move(other.m_response_data);
	}
	
	return *this;
}

shared_ptr<string> WebResponse::GetResponse()
{
	return m_response_data;
}

} /* namespace Curl */
