**Development will be resumed once Ruter publishes its new API.**
This is *alpha* code and should not be expected to function very well. A lot of code will be removed, changed or added when the new API is released. The travel planner functionality has not yet been implemented.

# ruter

*Ruter AS* is a common management company for public transport in Oslo and Akershus in Norway. They provide real time travel information and travel planners on their [website](https://ruter.no/) and through an [API](http://api.ruter.no/reisrest/help/). This program is an attempt to make that functionality available from the terminal. I do my very best to conform to the [Linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle) at all times.

## Building

Build the code by issuing `make`. The executable `ruter` can be found in the `bin` directory.

## Usage

### Searching
Searching for a place can be done with the `-f` option.

	ruter -f nationaltheatret

### Departures
Use the `-s` option to show the departures from a place. A list of possible matches will be printed. Select the correct place by entering `y` followed by pressing return.

	ruter -s nationaltheatret

## Dependencies

*   [json-parser](https://github.com/udp/json-parser/)
*   [libcurl](http://curl.haxx.se/libcurl/)
