CFLAGS=-std=gnu99 -Wall -Wextra -O2
POOL=europe.pool.ntp.org
TARGET=sntp

.PHONY: all run clean

all: ${TARGET}

run: ${TARGET}
	./${TARGET} 0.${POOL}
	./${TARGET} 1.${POOL}
	./${TARGET} 2.${POOL}
	./${TARGET} 3.${POOL}

clean:
	rm -fv *.o ${TARGET}

