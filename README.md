# Apachelite v0.1
Apachelite is a *very* basic HTTP server written in C. I wrote it to
practice writing in C and to learn about the ins and outs of HTTP.

##Version
0.2

## Features
This server is (and will possibly always remain) in
construction. Here's a quick list of what it currently supports.

+ Unix
+ Fully parsing any request
+ Serving GET requests (200, 400, 404, 500, 501 responses possible)
+ html / image / plain text content
+ Server root path (hardcoded in global constant)
+ One process per connection
+ Connection closes after response

## (Possible) Upcoming Features
+ POST / HEAD / PUT / ... (you get the idea)
+ PHP
+ Keep-alive connections

## Building and Running
Building is pretty standard. `cd` into the directory.

    make

This will create an executable named apachelite. Run it using

    ./apachelite -p PORT

