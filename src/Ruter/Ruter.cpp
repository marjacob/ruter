#include <iostream> // for debug only
#include <list>
#include <memory>
#include <string>
#include "Curl/ParameterCollection.hpp"
#include "Ruter/Location.hpp"
#include "Ruter/Place.hpp"
#include "Ruter/Ruter.hpp"

using std::list;
using std::shared_ptr;
using std::string;

namespace Ruter {
	Ruter::Ruter() :
		m_curl("https://reisapi.ruter.no")
	{
		
	}
	
	Ruter::~Ruter()
	{
		
	}
	
	shared_ptr<Place>
	Ruter::GetStop(const string& id)
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
	
	shared_ptr<list<shared_ptr<Place>>>
	Ruter::GetPlaces(const string& search, Location *location)
	{
		Curl::ParameterCollection params;
		params.Add(location);
		m_curl.SetRequestParameters(&params);
		m_curl.SetRequestUri("Place/GetPlaces/" + search);
		
		shared_ptr<string> json(m_curl.Request());
		shared_ptr<list<shared_ptr<Place>>> places;
		
		if (!json) {
			return places;
		}
		
		std::cout << *json << "\n";
		
		return places;
	}
}
