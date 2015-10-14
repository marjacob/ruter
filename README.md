**This is alpha code** and bugs should be expected. However, everything compiles and some functionality is usable (searching and departure information). A lot of code will be removed, changed or added when the new API is released. The travel planner functionality has not yet been implemented.

# ruter

*Ruter AS* is a common management company for public transport in Oslo and Akershus in Norway. They provide real time travel information and travel planners on their [website](https://ruter.no/) and through an [API](http://api.ruter.no/ReisRest/help). This program is my attempt to make that functionality available from the terminal. I am in no way affiliated with *Ruter AS*.

## Building

Build the code with `cmake` and `make`. From within the top level source code directory, issue the following commands.

	mkdir build
	cd build
	cmake ..
	make

## Usage

Currenly in an unusable state.

## Dependencies

*   [libcurl](http://curl.haxx.se/libcurl/)
