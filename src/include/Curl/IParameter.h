#ifndef __H_B7033EEFFEE14E57A6B92484FC9F0D3B__
#define __H_B7033EEFFEE14E57A6B92484FC9F0D3B__

#include <string>
#include "Parameter.h"

namespace Curl {

class IParameter {
public:
	virtual ~IParameter() {};
	
	virtual int
	CreateParameter(int token, Parameter& param)
		const = 0;
};

} /* namespace Curl */

#endif
