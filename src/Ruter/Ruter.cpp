#include <iostream> // for debug only
#include <list>
#include <memory>
#include <string>
#include "Curl/WebRequest.hpp"
#include "Ruter/Location.hpp"
#include "Ruter/Place.hpp"
#include "Ruter/Ruter.hpp"

using std::list;
using std::shared_ptr;
using std::string;
using std::unique_ptr;

namespace Ruter {

Ruter::Ruter() : m_api("https://reisapi.ruter.no"), m_client()
{
	return;
}

Ruter::~Ruter()
{
	
}

unique_ptr<Place> Ruter::GetStop(const string& id)
{
	Curl::WebRequest request(m_api);
	request.SetUri("Place/GetStop/" + id);
	
	unique_ptr<string> json = m_client.Request(request);
	unique_ptr<Place> place(nullptr);
	
	if (!json) {
		return place;
	}
	
	std::cout << *json << "\n";
	
	return place;
}

list<shared_ptr<Place>> Ruter::GetPlaces(
	const string& search,
	const Location *location)
{
	Curl::WebRequest request(m_api);
	request.AttachParameter(location);
	request.SetUri("Place/GetPlaces/" + search);
	
	unique_ptr<string> json = m_client.Request(request);
	list<shared_ptr<Place>> places;
	
	if (!json) {
		return places;
	}
	
	std::cout << *json << "\n";
	
	return places;
}

} /* namespace Ruter */
