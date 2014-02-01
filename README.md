**Development will be resumed once Ruter publishes its new API.**
This is *alpha* code and should not be expected to function very well. A lot of code will be removed, changed or added when the new API is released. The travel planner functionality has not yet been implemented.

# ruter

This is a client for the REST/JSON based Ruter API written in C. I do my very best to conform to the [Linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle) at all times.

## Building

Build the code by issuing `make`. The executable `ruter` can be found in the `bin` directory.

## Usage

### Searching
Searching for a place can be done with the `-f` option.

	ruter -f nationaltheatret

### Departures
Use the `-s` option to show the departures from a place. A list of possible matches will be printed. Select the correct place by entering `y` followed by pressing return.

	ruter -s nationaltheatret

## Libraries

*   [json-parser](https://github.com/udp/json-parser/)
*   [libcurl](http://curl.haxx.se/libcurl/)
