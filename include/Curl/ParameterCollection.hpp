#ifndef __H_4AFE2DAD99BF4511B8D26C1C57B852C0__
#define __H_4AFE2DAD99BF4511B8D26C1C57B852C0__

#include <string>
#include <vector>
#include "Curl/IParameter.hpp"

namespace Curl {

class ParameterCollection {
public:
	ParameterCollection();
	~ParameterCollection();
	
	IParameter* Get(unsigned int i) const;
	std::string ToString() const;
	std::vector<IParameter*>::size_type Size() const;
	void Add(IParameter *param);
	void Remove(unsigned int i);
private:
	std::vector<IParameter*> m_params;
};

} /* namespace Curl */

#endif
