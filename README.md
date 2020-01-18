
# irc
This project is about creating an IRC client and server (Internet relay Chat) that allows
to send and receives messages between many different people, from within chat groups
called “channel”.
![project pic](https://res.cloudinary.com/smarwise/image/upload/v1579336912/Mine/irc.png)
I had the freedom to choose the protocol (I was not obligated to respect the RFC
defining IRC, I could create my own chat protocol). I had to however, no matter my
choice absolutely had a consistant relationship between server and client. They must
communicate properly.
The communication between server and client must be in TCP/IP (v4).
It implements the following features:
* /nick <nickname>: Implement the notion of nickname (the login by default, maximum 9 characters, like in the RFC).
* /join <#chan>, /leave [#channel] , etc.: Implement the notion of channel.
* /who: Who is connected on the channel?
* /msg <nick> <message>: Sends a message to a specific user on the server.
* /connect <server> [port]: Connects to the server from the client.
* Switch the code in non-blocking mode: the code provided is blocking in some cases.
If a client does not respond, the server can block itself after a short delay. Be careful
only one Select is authorized in your code.
