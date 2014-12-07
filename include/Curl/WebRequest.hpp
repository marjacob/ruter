#ifndef __H_4AFE2DAD99BF4511B8D26C1C57B852C0__
#define __H_4AFE2DAD99BF4511B8D26C1C57B852C0__

#include <memory>
#include <string>
#include <vector>
#include "Curl/IParameter.hpp"

namespace Curl {

class WebRequest {
public:
	WebRequest(const std::string& url);
	~WebRequest();
	
	std::string ToString() const;
	void AttachParameter(const IParameter *param);
	void SetUri(const std::string& uri);
private:
	std::string FormatParameters() const;
	std::unique_ptr<std::string> m_baseUrl;
	std::unique_ptr<std::string> m_resourceUri;
	std::vector<const IParameter*> m_params;
};

} /* namespace Curl */

#endif
