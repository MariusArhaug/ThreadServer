# ThreadServer

A multi threaded web server, serving HTTP 1.1 on _n_ threads using web sockets. Developed using C and only standard libraries.

## Description

This web server was created as a part of the exercises in the course `TDT4186 Operating Systems` at NTNU in Trondheim, Norway.

It supports both JSON and HTML response headers and is able to server multiple incomming requests at once due to its multithreaded capabilities. It supports both IPv4 and IPv6

## Getting Started

### Dependencies

- This project was made with POSIX in mind and therefore is only supported for POSIX systems.

<br />

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
N_BUFFS=<number of bufferslots>

```

> Note that there should be no space between the equal sign (neither before nor after).

> A provided example can be found in the `.env.dist` file in the root directory

<br />

### Executing program

#### Execute yourself without .env variables

Make sure you have `GCC` and `Make` installed on your system and run the following commands

```shell
$ make
```

> compiles the program into an output file in /bin

```
$ ./bin/mtwwd [www-path] [port] [#threads] [#bufferslots]
```

> Run from binary folder with desired arguments, will not run unless all four have been selected

##### Or run from makefile with .env variables

```shell
$ make run
```

> compiles as before but also runs the binary with the environment variables defined earlier, if these are not defined this rule will not work.

<br />

## Query server

To query the server's endpoint you can use for example use `curl`:

```shell
$ curl http://localhost:8080/
```

> localhost will is a routeback to IPv4's loopback address 127.0.0.1 and IPv6's loopback address ::1

You can also explixitly query the server with either IPv4 or IPv6 ip addresses.

```shell
$ curl http://127.0.0.1:8080/
```

> IPv4

```shell
$ curl http://\[::1\]:8080/
```

> IPv6, curl needs to escape the brackets to understand them "\[" and "\]"

<br />
<br />

# Tasks

**a) Single thread server**.

The first task was to implement a single threaded web server that served requests for html documents in one of the servers sub directory.

This can achieved as such

```shell
$ ./mtwwwd 8080 home/www-path/ 0 0
```

> By setting [#threads] and [#bufferslots] to 0 you're turning it to a single threaded server.

<br/>

```shell
$ curl localhost:8080/doc/index.html
```

> Port is set to 8080 here but you can freely change it as described earlier

> **Note**: whenever an invalid request is sent, the server responds with a JSON format, use a web browser or Insomnia/Postman to view the response in its JSON glory.

Where the server will respond with the file, in a http packet format.

<br/>

**b) Counting semaphores**.

The second task was to create a semaphore interface that allows us to easily coordinate and interface with POSIX threads, this is acheived by using `pthread_mutex_lock(3)` and `pthread_cont_wait(3)` in the `src/types/sem.c` file.

<br/>

**c) Ring buffer**
A ring buffer was created to handle incomming file descriptors `"connfd"` from the `accept(3)` function in the main producer thread in the servers main loop.

The buffer then using the semaphores created in the previous task, to signal other waiting processes that something either has been added or removed from it.

<br />

**d) Multithreaded server**

Putting tasks a) - c) together we can create a thread pool that creates ` [#threads]` number of threads aswell as a buffer with `[#bufferslots]` available to be filled up. The main thread then acts as a `producer thread` while the other #threads are known as `consumer threads`

We pass each thread a handle_thread function that actively tries to get data from the ringbuffer. Whenever the buffer is empty though, it forces the current consuming thread to wait until the buffer has items to give.

To run the multithreaded server just use `#threads` and `#bufferslots` greater than 0

```shell
$ ./mtwwd 8080 /home/www-path/ 4 4
```

> Initalizes server with port `8080` from `/home/www-path/` directory with `4 consumer threads` and `4 slots in the ring buffer`

<br />
<br />

**e) Extra credit: identify significant security problems**

### Path traversal vulnearbility

A major flaw with the web servers current implementation is that it allows a client to request any file in the file system of the server, this is known as a **path traversal vulnearbility**.

An example of this is the following curl request

```shell
$ curl --path-as-is localhost:8080/doc/../src/main.c
```

> We use the flag --path-as-is because curl would automatically transform paths such as "../" to "/"
> Tools such as a webbrowser or Insomnia/Postman would also transform paths from "../" to "/". To avoid this you need to configure these tools differently..

This would before any security measurements send the main.c file to the request. potentially revealing secerets we do not want the client to know about the server. Even worse, the client could find every file in the system the server is running on!

There are two ways to prevent this currently implemented in the `src/server/router` directory

You can prevent this using regEx to match incomming path requests against the pattern `"\.\.\/"`

<br />

### Opening directories

Another problem is that the client could also ask for the whole directory if he does the following:

```shell
$ curl localhost:8080/
```

This would then return the contents of all the files under the `www-path` directory.

To prevent this we can use the `stat(3)` function in the `util/file.c` file to check if path is requesting a file or a directory.

<br />

### Opening unwanted files from other sub directories

A final problem is that a client can still open unwanted files under the `www-path` directory such as:

```shell
$ curl localhost:8080/src/main.c
```

This would then return the contents of the `main.c` file, again something we might not want our server to do.

To prevent this the `router.c` files handles valid routes that a client might want to reach out for. Currently only the `/doc/` endpoint is requested so that only files within that directory should be allowed to be sent back to the client.

## Testing

A simple curl script was made in the `scripts` directory to simulate multiple requests. It seems like it sends too many requests at one time causing the system to receive a bad file descriptor. Currently the server will shutdown if it encounters such a bad file descriptor.
