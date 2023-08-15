#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "randstate.h"
#include <gmp.h>
gmp_randstate_t state;
//makes the global state variable
void randstate_init(uint64_t seed) {
    //gmp_randseed_ui(state,seed);
    //given a seed then we intialize the random
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

void randstate_clear(void) {
    //end the state
    gmp_randclear(state);
}
