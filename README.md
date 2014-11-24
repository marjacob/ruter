**Development will be resumed once Ruter publishes its new API.**
This is *alpha* code and bugs should be expected. However, everything compiles and some functionality is usable (searching and departure information). A lot of code will be removed, changed or added when the new API is released. The travel planner functionality has not yet been implemented.

# ruter

*Ruter AS* is a common management company for public transport in Oslo and Akershus in Norway. They provide real time travel information and travel planners on their [website](https://ruter.no/) and through an [API](http://api.ruter.no/ReisRest/help). This program is my attempt to make that functionality available from the terminal. I am in no way affiliated with *Ruter AS*.

UTF8 characters are supported. I do my very best to conform to the [Linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle) at all times.

## Building

Build the code by issuing `make` or `make debug`. The executable `ruter` can be found in the `bin` directory. Removing all generated files is just a matter of issuing `make clean` as usual.

## Usage

### Searching
Searching for a place can be done with the `--search` (or `-s`) option.

	ruter --search nationaltheatret

### Departures
Use the `--from` (or `-f`) option to show the departures from a place.

	ruter --from forsk

The number of printed results can be controlled with the `--limit` (ir `-l`) option (zero indicates no limit), and the desired kind of transportation can be indicated with the `--kind` (or `-k`) option.

	ruter -f national -k metro -l 8

## Dependencies

*   [json-parser](https://github.com/udp/json-parser/) (included)
*   [libcurl](http://curl.haxx.se/libcurl/)
