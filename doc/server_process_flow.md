growlnotify
=====
There are 2 calls:
1. register app name (close socket)
2. send notification (close socket)

serverhack (GrowlServerHack)
=====

server with fork
-----
```
  /* with fork() */
  socket()
  listen()
  while(1) {
    new_socket= accept();
    pid= fork()l

    if (pid==0) { // child
      read socket
      process message
    }
    else { // parent 
      close(new_socket);
    }
  } // end of while, wait for new socket again

```

server without fork
-----
```
  socket()
  listen()
  while(1) {
  ...
    new_socket= accept(); // wait for socket connection

    while(valread= read(new_socket, buffer, 1024-1)>0) { 
    	// since growlnotify only send 1 message/session, can skip this while loop

    } // end of read
    close(new_socket);  
  } // end of socket
  close(server_fd); // This line is unreachable in the current loop structure

```