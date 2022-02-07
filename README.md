# HW2 Simple Key-value Store

## Directories
- /server ->	server program related sources
- /client ->	client program related sources
- /common ->	common inclusions
- /util ->	common utilization
- /build ->	target build directory

## Building the Project
Code out your `/server/server.c` and `/client/client.c`, then
```shell
$ make
```
Test your `/build/server` and `build/client`.

## Implementations
### Please briefly describe your multi-threading design pattern
A main thread listening listening on port.
Create a new thread when there is a new client, detach the thread after created.
Command is fetch in thread(client pass the whole string to server).
### Please briefly describe your data structure implementation
A linked list to implement data storage.
Pointer head declare as global pointing to the head node of linked list
insert() to insert new kvpair
delete() to delete kvpair, return 1 if success, -1 if no such key in list
get() to search for kvpair, return 1 if found, -1 if not found

## References
* [POSIX thread man pages](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* [socket man pages](https://linux.die.net/man/7/socket)

