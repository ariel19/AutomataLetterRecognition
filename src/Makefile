CC=
GXX=${CC}gcc
CFLAGS=-Wall -Wextra -pedantic -O3
FUZZY=#-DFUZZY_TYPE
REJECTED=-DIS_REJECTED
DEFS=${FUZZY} ${REJECTED}
LDFLAGS=-lm
OBJ=main.o matrix.o syswrap.o alr.o swarm.o
SRC=src
BIN=bin

TARGET=alr

${TARGET} : ${OBJ}
	${GXX} -o ${TARGET} ${OBJ} ${LDFLAGS}
	@mv ${TARGET} ../${BIN}

main.o: main.c
	${GXX} ${CFLAGS} ${DEFS} -c $<

matrix.o: matrix.c
	${GXX} ${CFLAGS} ${DEFS} -c $<

syswrap.o: syswrap.c
	${GXX} ${CFLAGS} ${DEFS} -c $<

alr.o: alr.c
	${GXX} ${CFLAGS} ${DEFS} -c $<

swarm.o: swarm.c
	${GXX} ${CFLAGS} ${DEFS} -c $<

.PHONY: clean
clean:
	rm -f ${OBJ}

.PHONY: realclean
realclean: clean
	rm -f ../${BIN}/${TARGET}
