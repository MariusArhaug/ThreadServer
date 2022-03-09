# ThreadServer

A multi threaded web server, serving HTTP 1.1 on *n* threads using web sockets. Developed using C and only standard libraries. 


## Description

This web server was created as a part of the exercises in the course `TDT4186 Operating Systems` at NTNU in Trondheim, Norway.'

It supports both JSON and HTML response headers and is able to server multiple incomming requests at once due to its multithreaded capabilities. 



## Getting Started

### Dependencies

- This project was made with POSIX in mind and therefore is only supported for POSIX systems.

### Installing

```
$ git clone git@github.com:MariusArhaug/MultithreadedWebServer.git
```
```
cd MultithreadedWebServer
```

### Executing program

Make sure you have `GCC` and `Make` installed on your system and run the following commands

```
$ make
```

> compiles the program into an output file in /bin

```
$ make run
```

> compiles as before but also runs the binary.
