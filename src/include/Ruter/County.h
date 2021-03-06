#ifndef __H_3E0E67866B4F4477ABDB3BE7FAB6A67D__
#define __H_3E0E67866B4F4477ABDB3BE7FAB6A67D__

#include <string>
#include "Curl/Parameter.h"
#include "Curl/IParameter.h"

namespace Ruter {

class County : public Curl::IParameter {
public:
	explicit County(std::string name = "");
	
	County(const County& other);
	County(County&& other);
	
	virtual ~County() override;
	
	County& operator=(const County& other);
	County& operator=(County&& other);
	
	void SetName(std::string name);
	std::string GetName();
	
	virtual int CreateParameter(int token, Curl::Parameter& param)
		const override;
private:
	std::string m_name;
};

} /* namespace Ruter */

#endif
