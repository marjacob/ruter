#include <string>
#include <vector>
#include "Curl/ParameterCollection.hpp"
#include "Curl/IParameter.hpp"

using std::string;
using std::vector;

namespace Curl
{
	ParameterCollection::ParameterCollection() :
		m_params()
	{
		return;
	}
	
	ParameterCollection::~ParameterCollection()
	{
		return;
	}
	
	void ParameterCollection::Add(IParameter *param)
	{
		if (param) {
			m_params.push_back(param);
		}
	}
	
	vector<IParameter*>::size_type ParameterCollection::Size() const
	{
		return m_params.size();
	}
	
	IParameter* ParameterCollection::Get(unsigned int i) const
	{
		return i > Size() ? nullptr : m_params[i];
	}
	
	void ParameterCollection::Remove(unsigned int i)
	{
		m_params.erase(m_params.begin() + i);
	}
	
	string ParameterCollection::ToString() const
	{
		string parameters;
		Parameter param;
		IParameter *obj = nullptr;
		
		for (int i = 0, j = m_params.size(); i < j; i++) {
			obj = m_params[i];
			int token = 0;
			
			do {
				token = obj->CreateParameter(token, param);
				parameters += i ? "&" : "?";
				parameters += param.ToString();
			} while (token);
		}
		
		return parameters;
	}
}
