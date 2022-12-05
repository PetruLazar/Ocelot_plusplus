# Ocelot++
Ocelot++ is a Minecraft Java 1.18.2 Windows-only server made in `C++17`.  
For more details about the Minecraft Java protocol, check the [wiki.vg](https://wiki.vg/Protocol) site.

## State
As at the time of reading this message, the project is currently `discontinued`.  
The server is usable in Creative, but not really playable in Survival, for more informations about the current problems and plans, check out [NOTES](NOTES.md).  

## Libraries
- [SFML 2.5.1](https://www.sfml-dev.org/) - Networking.
- [ZLIB](https://zlib.net/) - Compression.
- [Thread](https://github.com/aphenriques/thread) - Thread pooling.
- [GoogleTest](https://github.com/google/googletest) - Testing.

## Project structure
MC stuff - The server project.  
NetConsole - An app that redirects an incoming connection to a configurable server which tracks and logs the data sent between the two.  
UnitTests - Project's tests.
Utils/Sandbox
