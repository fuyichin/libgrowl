FROM alpine:3.14
LABEL maintainer="use alpine linux to create docker image for automake"
WORKDIR /home
RUN <<EOF
apk add gcc musl-dev binutils autoconf automake  pkgconfig git 
apk add cmocka-dev make
cd /home
git clone https://github.com/fuyichin/libgrowl
cd libgrowl
aclocal; autoconf
automake --add-missing
./configure --disable-libnotify --enable-serverhack --disable-dependency-tracking
EOF

CMD ["/bin/ls"]
