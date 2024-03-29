package=libgrowl
version=0.1.0

all:
	cd src; make;
	cd examples; make;

test:
	cd cmocka; make test;

deb: control src/libgrowl.h src/libgrowl.a
	install -d /tmp/$(package)/DEBIAN
	install -d /tmp/$(package)/usr/lib/pkgconfig
	install -d /tmp/$(package)/usr/include
	install src/libgrowl.h /tmp/$(package)/usr/include/.
	install src/libgrowl.a /tmp/$(package)/usr/lib/.
	install control /tmp/$(package)/DEBIAN/.
	install libgrowl.pc /tmp/$(package)/usr/lib/pkgconfig/.
	# cd /tmp/$(package) && find usr -type f -exec md5sum {} \; > /tmp/$(package)/DEBIAN/md5sums
	dpkg -b /tmp/$(package) $(package)-$(version).deb

clean:
	cd src; make clean;
	cd examples; make clean;
	cd cmocka; make clean;
	rm -f $(package)-*.deb
