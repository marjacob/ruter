#ifndef __H_4AFE2DAD99BF4511B8D26C1C57B852C0__
#define __H_4AFE2DAD99BF4511B8D26C1C57B852C0__

#include <string>
#include <vector>
#include "IParameter.h"

namespace Curl {

class WebRequest {
public:
	explicit WebRequest(const std::string& url);

	WebRequest(const WebRequest& other);
	WebRequest(WebRequest&& other);

	virtual ~WebRequest();

	WebRequest& operator=(const WebRequest& other);
	WebRequest& operator=(WebRequest&& other);
	
	std::string ToString() const;
	void AttachParameter(const IParameter *param);
	std::string GetMethod() const;
	void SetBaseUrl(const std::string& url);
	void SetMethod(const std::string& method);
	void SetResourceUri(const std::string& uri);
private:
	std::string CreateParameters() const;
	std::string *m_base_url;
	std::string *m_method;
	std::string *m_resource_uri;
	std::vector<const IParameter*> m_params;
};

} /* namespace Curl */

#endif
