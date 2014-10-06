CC=
GXX=${CC}gcc
CFLAGS=-Wall -Wextra -pedantic -g
LDFLAGS=
OBJ=main.o matrix.o
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

.PHONY: clean
clean:
	rm -f ${OBJ}

.PHONY: realclean
realclean: clean
	rm -f ../${BIN}/${TARGET}
