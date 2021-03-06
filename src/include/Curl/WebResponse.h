#ifndef __H_945E23D07029401B9E9973FA87003631__
#define __H_945E23D07029401B9E9973FA87003631__

#include <memory>
#include <string>

namespace Curl {

class WebResponse {
public:
	WebResponse();

	WebResponse(const WebResponse& other);
	WebResponse(WebResponse&& other);

	virtual ~WebResponse();

	WebResponse& operator=(const WebResponse& other);
	WebResponse& operator=(WebResponse&& other);

	std::shared_ptr<std::string> GetResponse();
private:
	std::shared_ptr<std::string> m_response_data;
};

} /* namespace Curl */

#endif
