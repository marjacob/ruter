#include <ctime>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>
#include "Curl.hpp"
#include "Ruter.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

int main(int argc, char *argv[])
{
	Curl::ParameterCollection params;
	Ruter::County kolbotn("Færevåg");
	Ruter::Location location(66000.0, 66000.0);
	Ruter::Time departure;
	params.Add(&kolbotn);
	params.Add(&location);
	params.Add(&departure);
	
	cout << params.ToString() << endl;
	
	Ruter::Ruter *ruter = NULL;
	
	try {
		ruter = new Ruter::Ruter();
	} catch (const Curl::CurlException& e) {
		cerr << argv[0] << ": " << e.what() << endl;
		return EXIT_FAILURE;
	}
	
	shared_ptr<Ruter::Place> place = ruter->GetStop("3010930");
	
	return EXIT_SUCCESS;
}
