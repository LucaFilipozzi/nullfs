CFLAGS=-Wall
CFLAGS+=$(shell pkg-config fuse --cflags)
LDFLAGS+=$(shell pkg-config fuse --libs)
DEB_HOST_MULTIARCH=$(shell dpkg-architecture -qDEB_HOST_MULTIARCH)

.PHONY: default
default: build

.PHONY: build
build: nullfs

.PHONY: check
check: nullfs.c
	splint -weak -expect 2 -fullinitblock +posixlib -I/usr/include/$(DEB_HOST_MULTIARCH) $<

.PHONY: clean
clean:
	rm -f nullfs
