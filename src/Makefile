CC=
GXX=${CC}gcc
CFLAGS=-Wall -Wextra -pedantic -g#-O3
FUZZY=-DFUZZY_TYPE
LDFLAGS=-lm
OBJ=main.o matrix.o syswrap.o alr.o swarm.o
SRC=src
BIN=bin

TARGET=alr

${TARGET} : ${OBJ}
	${GXX} -o ${TARGET} ${OBJ} ${LDFLAGS}
	@mv ${TARGET} ../${BIN}

main.o: main.c
	${GXX} ${CFLAGS} ${FUZZY} -c $<

matrix.o: matrix.c
	${GXX} ${CFLAGS} ${FUZZY} -c $<

syswrap.o: syswrap.c
	${GXX} ${CFLAGS} ${FUZZY} -c $<

alr.o: alr.c
	${GXX} ${CFLAGS} ${FUZZY} -c $<

swarm.o: swarm.c
	${GXX} ${CFLAGS} ${FUZZY} -c $<

.PHONY: clean
clean:
	rm -f ${OBJ}

.PHONY: realclean
realclean: clean
	rm -f ../${BIN}/${TARGET}
