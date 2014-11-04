CC=
GXX=${CC}gcc
CFLAGS=-Wall -Wextra -pedantic -g# -O3
LDFLAGS=
OBJ=main.o matrix.o syswrap.o alr.o swarm.o
SRC=src
BIN=bin

TARGET=alr

${TARGET} : ${OBJ}
	${GXX} ${LDFLAGS} -o ${TARGET} ${OBJ}
	@mv ${TARGET} ../${BIN}

main.o: main.c
	${GXX} ${CFLAGS} -c $<

matrix.o: matrix.c
	${GXX} ${CFLAGS} -c $<

syswrap.o: syswrap.c
	${GXX} ${CFLAGS} -c $<

alr.o: alr.c
	${GXX} ${CFLAGS} -c $<

swarm.o: swarm.c
	${GXX} ${CFLAGS} -c $<

.PHONY: clean
clean:
	rm -f ${OBJ}

.PHONY: realclean
realclean: clean
	rm -f ../${BIN}/${TARGET}
