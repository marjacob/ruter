#ifndef __H_7944EC7A9B1141548FA5499A8C1B524E__
#define __H_7944EC7A9B1141548FA5499A8C1B524E__

#include <string>

namespace Curl {
	class Parameter {
	public:
		std::string GetKey();
		std::string GetValue();
		std::string ToString();
		void SetKey(std::string key, bool escape);
		void SetValue(std::string value, bool escape);
	protected:
		std::string Escape(std::string value);
	private:
		bool m_escapeKey;
		bool m_escapeValue;
		std::string m_key;
		std::string m_value;
	};
}

#endif
