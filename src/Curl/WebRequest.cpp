#include <memory>
#include <string>
#include <vector>
#include "Curl/WebRequest.hpp"
#include "Curl/IParameter.hpp"

using std::string;
using std::unique_ptr;
using std::vector;

namespace Curl {

WebRequest::WebRequest(const string& url) :
	m_baseUrl(new string(url)),
	m_params(),
	m_resourceUri()
{
	return;
}

WebRequest::~WebRequest()
{
	return;
}

void WebRequest::AttachParameter(const IParameter *param)
{
	if (param) {
		m_params.push_back(param);
	}
}

void WebRequest::SetUri(const string& uri)
{
	m_resourceUri = unique_ptr<string>(new string(uri));
}

string WebRequest::FormatParameters() const
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

string WebRequest::ToString() const
{
	string url = *m_baseUrl;

	if (m_resourceUri) {
		url += "/" + *m_resourceUri;
	}
	
	if (m_params.size() > 0) {
		url += "/" + FormatParameters();
	}

	return url;
}

} /* namespace Curl */
