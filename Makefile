all:
	cd src; make;
	cd examples; make;

test:
	cd cmocka; make test;

clean:
	cd src; make clean;
	cd examples; make clean;
	cd cmocka; make clean;