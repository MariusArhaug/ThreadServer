# ThreadServer

A multi threaded web server, serving HTTP 1.1 on _n_ threads using web sockets. Developed using C and only standard libraries.

## Description

This web server was created as a part of the exercises in the course `TDT4186 Operating Systems` at NTNU in Trondheim, Norway.

It supports both JSON and HTML response headers and is able to server multiple incomming requests at once due to its multithreaded capabilities.

## Getting Started

### Dependencies

- This project was made with POSIX in mind and therefore is only supported for POSIX systems.

### Installing

```
$ git clone git@github.com:MariusArhaug/ThreadServer.git
```

```
$ cd ThreadServer
```

#### Environment variables

To set up environment variables, you need to create a `.env` file in the root directory. Its contents should be like this:

Note: **Key name cannot change, only value**

```bash
PORT=<port>
WEB_PATH=<path to where the webserver should reside>
N_THREADS=<number of threads>
N_BUFFS=<number of buffers>

```

> Note that there should be no space between the equal sign (neither before nor after).

### Executing program

#### Execute yourself without .env variables

Make sure you have `GCC` and `Make` installed on your system and run the following commands

```
$ make
```

> compiles the program into an output file in /bin

```
$ ./bin/mtwwd [www-path] [port] [#threads] [#bufferslots]
```

> Run from binary folder with desired arguments, will not run unless all four have been selected

##### Or run from makefile with .env variables

```
$ make run
```

> compiles as before but also runs the binary with the environment variables defined earlier, if these are not defined this rule will not work.

<br />
<br />

# Tasks

**a) Single thread server**.

The first task was to implement a single threaded web server that served requests for html documents in one of the servers sub directory.

This can achieved as such

```
$ ./mtwwwd 8080 home/www-path/ 0 0
```

> By setting [#threads] and [#bufferslots] to 0 you're turning it to a single threaded server.

<br/>

```
$ curl localhost:8080/doc/index.html
```

> Port is set to 8080 here but you can freely change it as described earlier

> **Note**: whenever an invalid request is sent, the server responds with a JSON format, use a web browser or Insomnia/Postman to view the response in its JSON glory.

Where the server will respond with the file, in a http packet format.

<br/>
<br/>

**b) Counting semaphores**.

The second task was to create a semaphore interface that allows us to easily coordinate and interface with POSIX threads, this is acheived by using `pthread_mutex_lock(3)` and `pthread_cont_wait(3)` in the `src/types/sem.c` file.

<br/>
<br/>

**c) Ring buffer**
A ring buffer was created to handle incomming file descriptors `"connfd"` from the `accept(3)` function in the main producer thread in the servers main loop.

The buffer then using the semaphores created in the previous task, to signal other waiting processes that something either has been added or removed from it.

<br />
<br />

**d) Multithreaded server**

Putting tasks a) - c) together we can create a thread pool that creates ` [#threads]` number of threads aswell as a buffer with `[#bufferslots]` available to be filled up. The main thread then acts as a `producer thread` while the other #threads are known as `consumer threads`

We pass each thread a handle_thread function that actively tries to get data from the ringbuffer. Whenever the buffer is empty though, it forces the current consuming thread to wait until the buffer has items to give.

To run the multithreaded server just use `#threads` and `#bufferslots` greater than 0

```
$ ./mtwwd 8080 /home/www-path/ 4 4
```

> Initalizes server with port `8080` from `/home/www-path/` directory with `4 consumer threads` and `4 slots in the ring buffer`

<br />
<br />

**e) Extra credit: identify significant security problems**
