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
	Ruter::Ruter *ruter = nullptr;
	
	try {
		ruter = new Ruter::Ruter();
	} catch (const Curl::CurlException& e) {
		cerr << argv[0] << ": " << e.what() << endl;
		return EXIT_FAILURE;
	}
	
	auto place = ruter->GetStop("3010930");
	auto places = ruter->GetPlaces("forsk");
	
	return EXIT_SUCCESS;
}
