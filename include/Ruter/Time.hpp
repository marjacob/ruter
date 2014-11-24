#ifndef __H_B279FCF5DC04466AB131A74B87BB1E22__
#define __H_B279FCF5DC04466AB131A74B87BB1E22__

#include <ctime>
#include "Curl/Parameter.hpp"
#include "Curl/IParameter.hpp"

namespace Ruter {
	class Time : public Curl::IParameter {
	public:
		explicit Time(bool isDeparture = true);
		explicit Time(std::time_t time, bool isDeparture = true);
		explicit Time(std::tm& time, bool isDeparture = true);
		explicit Time(const Time& other);
		explicit Time(Time&& other);
		
		virtual ~Time() override;
		
		Time& operator=(const Time& other);
		Time& operator=(Time&& other);
		
		virtual int
		CreateParameter(int token, Curl::Parameter& param)
			const override;
	protected:
		std::string Format() const;
	private:
		bool m_isDeparture;
		std::time_t m_time;
	};
}

#endif
