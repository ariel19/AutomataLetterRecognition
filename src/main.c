#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "matrix.h"
#include "alr.h"

int main(int argc, char **argv) {
	msize_t splits_num = 5;
	msize_t symbol_class_num = 26;
	fsize_t feature_num = 10;
	
	automata_t atm;
	feat_t max; /* TODO: malloc & init */

	puts("Hello, automata!");
	
	srand(time(NULL));
	automata_init(&atm, &max, feature_num, splits_num, symbol_class_num);
	
	
		
	return EXIT_SUCCESS;
}
