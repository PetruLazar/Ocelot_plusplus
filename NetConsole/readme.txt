This app redirects an incoming connection to a configurable server and tracks and logs the data sent between the two end of the connection.
Each packet is logged in the "tcp" directory, which has to exist for the logging to take place.

The name of each packet has the following format: <id><source>_to_<destination>.bin   (raw binary files)
  id - the id of the package. Each app execution, it starts at 0 and increases each time a packet is logged
  source/destination - where the packet is coming from and where the packet is to arrive (both can be 's' for server (the connection that the app redirects to) or 'c' for client (the connection that the app redirects))
examples: 0c_to_s.bin
          1s_to_c.bin

#serverconfig.txt

example: "127.0.0.1 25565 8888"

The first two values are the ip and the port the app redirects the incoming connection to.
The third value is the port to listen to for the incoming connection, which it will redirect to the ip and port represented as the first two values.
