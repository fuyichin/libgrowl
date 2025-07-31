Docker Cheatsheet
=====

Introduction
=====
Docker is a container, lightweight, and isolated from other system. So is secure to run a service(eg. Apache) on it. 

Install
=====
(skip)

Hello world
=====
```
$ docker run hello-world
⇒ Hello from Docker!
```
May need super user, hello-world is an image. 

Alpine and busybox image
=====
Busybox 5Mb, 
Alpine 5-10Mb

Image and container
=====
Image can be downloaded from docker site, modified for own used.
Everytime user run an image, it created a container.

Dockerfile (create image)
=====
$ edit Dockerfile
```
FROM alpine
LABEL maintainer="use alpine linux to create own docker image"
WORKDIR /home
COPY dummy /home/.
CMD ["/bin/ls" ,"/home/dummy"]
```

```
$ docker build -t myimage .
 => ERROR [2/2] COPY dummy /
```
Missing dummy file, just create one and try again.

WORKDIR, the default the directory you run the image

COPY dummy /home, copy file dummy to myimage

CMD, the first command when you run myimage.

You can only run one command in CMD?? If you want to run more commands, run it as script file. 

```
$ docker run myimage
/home/dummy
```
Image list
=====
```
$ docker image ls
REPOSITORY          TAG       IMAGE ID       CREATED          SIZE
myimage             latest    b1951b8f031f   24 seconds ago   8.31MB
```

Dockerfile: RUN as in terminal
=====
```
$ docker run -it myimage /bin/sh
/home # ls
dummy
/home # touch readme && ls
dummy   readme
```
Note: -it interative tty

If you exit and run myimage again, readme file is not there anymore. Why? Because when you run myimage, it created a container, when you exit and run the image again, it created another container from the image. 

If you want to keep the readme file, you should do it in the Dockerfile and save the image.

Dockerfile (example 2)
=====
```
FROM alpine:3.14
LABEL maintainer="use alpine linux to create docker image for automake"
WORKDIR /home
RUN <<EOF
apk add make
apk add autoconf
EOF
```

Delete container
=====
```
$ docker container ps -a
CONTAINER ID   IMAGE          COMMAND     STATUS
fc6610cd3e78   30132ccb16ab   "/bin/sh"  Exited (0) 3 minutes ago

$ docker container rm <id>
```
```
 ## delete all the container
$ docker container prune
```

Delete image (rmi)
=====
```
$ docker image ls
REPOSITORY TAG       IMAGE ID
myimage    latest    39fa18ee40ce

$ docker rmi myimage:lastest
 ## or
$ docker rmi <image id>
```
```
$ docker rmi $(docker image ls -q)
```