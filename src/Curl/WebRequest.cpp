#include <string>
#include <utility>
#include <vector>
#include "Curl/WebRequest.hpp"
#include "Curl/IParameter.hpp"

using std::move;
using std::string;
using std::vector;

namespace Curl {

WebRequest::WebRequest(const string& url) :
	m_base_url(new string(url)),
	m_params(),
	m_resource_uri(nullptr)
{
	return;
}

WebRequest::WebRequest(const WebRequest& other) :
	m_base_url(other.m_base_url),
	m_params(other.m_params),
	m_resource_uri(other.m_resource_uri)
{
	return;
}

WebRequest::WebRequest(WebRequest&& other) :
	m_base_url(move(other.m_base_url)),
	m_params(move(other.m_params)),
	m_resource_uri(move(other.m_resource_uri))
{
	return;
}

WebRequest::~WebRequest()
{
	delete m_base_url;
	delete m_resource_uri;
}

WebRequest& WebRequest::operator=(const WebRequest& other)
{
	if (this != &other) {
		m_base_url = other.m_base_url;
		m_params = other.m_params;
		m_resource_uri = other.m_resource_uri;
	}
	
	return *this;
}

WebRequest& WebRequest::operator=(WebRequest&& other)
{
	if (this != &other) {
		m_base_url = move(other.m_base_url);
		m_params = move(other.m_params);
		m_resource_uri = move(other.m_resource_uri);
	}
	
	return *this;
}

void WebRequest::AttachParameter(const IParameter *param)
{
	if (param) {
		m_params.push_back(param);
	}
}

void WebRequest::SetBaseUrl(const string& url)
{
	if (m_base_url) {
		delete m_base_url;
	}

	m_base_url = new string(url);
}

void WebRequest::SetResourceUri(const string& uri)
{
	if (m_resource_uri) {
		delete m_resource_uri;
	}

	m_resource_uri = new string(uri);
}

string WebRequest::ToString() const
{
	string url = *m_base_url;

	if (m_resource_uri) {
		url += "/" + *m_resource_uri;
	}
	
	if (m_params.size() > 0) {
		url += "/" + CreateParameters();
	}

	return url;
}

string WebRequest::CreateParameters() const
{
	string params;
	Parameter param;
	const IParameter *obj = nullptr;
	
	for (int i = 0, j = m_params.size(); i < j; ++i) {
		obj = m_params[i];
		int token = 0;
		
		do {
			token = obj->CreateParameter(token, param);
			params += i ? "&" : "?";
			params += param.ToString();
		} while (token);
	}
	
	return params;
}

} /* namespace Curl */
