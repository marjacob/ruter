#include <iostream> // for debug only
#include <memory>
#include <string>
#include "Ruter/Location.hpp"
#include "Ruter/Place.hpp"
#include "Ruter/Ruter.hpp"

using std::shared_ptr;
using std::string;

namespace Ruter
{
	Ruter::Ruter() :
		m_curl("https://reisapi.ruter.no")
	{
		
	}
	
	Ruter::~Ruter()
	{
		
	}
	
	shared_ptr<Place> Ruter::GetStop(const string& id)
	{
		m_curl.SetRequestUri("Place/GetStop/" + id);
		m_curl.SetRequestParameters(nullptr);
		
		shared_ptr<string> json(m_curl.Request());
		shared_ptr<Place> place(nullptr);
		
		if (!json) {
			return place;
		}
		
		std::cout << *json << "\n";
		
		return place;
	}
	
	/*shared_ptr<list<shared_ptr<Place>>> Ruter::GetPlaces(const string& id, const Location& location)
	{
		
	}*/
}
