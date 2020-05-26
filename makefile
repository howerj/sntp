CFLAGS=-std=gnu99 -Wall -Wextra -O2
VERSION=1.0.0
POOL=europe.pool.ntp.org
TARGET=sntp
AR=ar
ARFLAGS=rcs
DESTDIR=install

.PHONY: all run test dist install clean

all: ${TARGET}

test:

run: ${TARGET}
	./${TARGET} 0.${POOL}
	./${TARGET} 1.${POOL}
	./${TARGET} 2.${POOL}
	./${TARGET} 3.${POOL}

lib${TARGET}.a: sntp.o
	${AR} ${ARFLAGS} $@ $^

${TARGET}: main.o lib${TARGET}.a

${TARGET}.1: readme.md
	pandoc -s -f markdown -t man $< -o $@

install: ${TARGET} ${TARGET}.h lib${TARGET}.a ${TARGET}.1
	install -p -D ${TARGET} ${DESTDIR}/bin/${TARGET}
	install -p -m 644 -D lib${TARGET}.a ${DESTDIR}/lib/lib${TARGET}.a
	install -p -m 644 -D ${TARGET}.h ${DESTDIR}/include/${TARGET}.h
	-install -p -m 644 -D ${TARGET}.1 ${DESTDIR}/man/${TARGET}.1
	mkdir -p ${DESTDIR}/src
	cp -a .git ${DESTDIR}/src
	cd ${DESTDIR}/src && git reset --hard HEAD

dist: install
	tar zcf ${TARGET}-${VERSION}.tgz ${DESTDIR}


clean:
	rm -rf *.o *.1 *.d *.tgz ${TARGET} install

