#ifndef __H_5A79A1CBAF5948F6E5A8E166271DED9B__
#define __H_5A79A1CBAF5948F6E5A8E166271DED9B__

#include "Curl/Parameter.hpp"
#include "Curl/IParameter.hpp"

namespace Ruter {

class Location : public Curl::IParameter {
public:
	explicit Location(double x = 0.0, double y = 0.0);
	explicit Location(const Location& other);
	explicit Location(Location&& other);
	
	virtual ~Location() override;
	
	Location& operator=(const Location& other);
	Location& operator=(Location&& other);
	
	virtual int
	CreateParameter(int token, Curl::Parameter& param)
		const override;
private:
	double m_x;
	double m_y;
};

} /* namespace Ruter */

#endif
