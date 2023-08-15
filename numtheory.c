#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "numtheory.h"
#include <stdbool.h>
#include "randstate.h"
#include <time.h>

//internal TEST of NUM got memories ain't gonna delete
//gmp_randstate_t state;
/*
int  main(){
		
	//unsigned long seedz;
	//gmp_randseed_ui(state,63521);
	//gmp_randinit_mt(state);
	//gmp_randseed_ui(state,63283);
	//gmp_printf("valuez : %Zd\n",state);
	randstate_init(32632);
	mpz_t b;
    mpz_init(b);
    mpz_set_ui(b,5);
    mpz_t e;
    mpz_init(e);
    //uint64_t along;
    //along=mpz_get_ui(a);
    mpz_set_ui(e,20);
    mpz_t mod;
    mpz_init(mod);
	mpz_set_ui(mod,40);
	
	mpz_t hold;
	mpz_init(hold);
	mpz_t primez;
	mpz_init(primez);
	
	//make_prime(primez, 16,40);
	
	//printf("ahhh\n");
	//pow_mod(hold,b,e,mod);
	//mpz_set_ui(hold,1);
	//gmp_printf("value: %Zd\n", primez);
	//gcd(hold,e,mod);
	//gmp_printf("valz: %Zd\n", hold);
	mpz_set_ui(e,1832911111);
	mpz_set_ui(mod,133445545454);
	mod_inverse(hold,e,mod);
	gmp_printf("val2: %Zd\n", hold);
	//make_prime(primez,32,40);
	//gmp_printf("value: %Zd\n", primez);
	printf("is prime tests\n");
	mpz_t testo;
	mpz_init(testo);
	//mpz_set_ui(testo,184467440737095515);
	bool test=false;

	test=is_prime(testo,40);	
	printf("%d\n",test);
	return 0;

}
*/
//this does power exponetatian. aka take a^d mod n very efficiently
//it feeds the answer back through the struct pointer mpz_t o
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t v;
    mpz_init(v);
    mpz_set_ui(v, 1);
    mpz_t p;
    mpz_init(p);
    //doing my intial values ik
    mpz_t dhold;
    mpz_init(dhold);
    mpz_set(dhold, d);
    mpz_set(p, a);
    mpz_t val;
    mpz_init(val);
    mpz_set_ui(val, 0);
    //if its not odd and not 0 then we multiple v*P and v*n
    //then square p and p*n
    //then flor divison the d
    //set out to v
    while ((mpz_cmp(dhold, val)) > 0) {
        if ((mpz_odd_p(dhold)) != 0) {

            mpz_mul(v, v, p);

            mpz_mod(v, v, n);
        }

        mpz_mul(p, p, p);

        mpz_mod(p, p, n);
        mpz_fdiv_q_ui(dhold, dhold, 2);
    }

    mpz_set(o, v);

    mpz_clears(v, p, val, dhold, NULL);
    return;
}
//this does the miller rabin test it is done by the number of iters

bool is_prime(mpz_t n, uint64_t iters) {

    mpz_t s1;
    mpz_init(s1);
    mpz_set_ui(s1, 0);
    mpz_t r;
    mpz_init(r);
    mpz_set(r, n);
    mpz_t temp;
    mpz_init(temp);
    mpz_t temp3;
    mpz_init(temp3);
    mpz_t divhold;
    mpz_init(divhold);
    mpz_t temp4;
    mpz_init(temp4);
    mpz_sub_ui(temp, n, 1);
    mpz_set(divhold, temp);
    mpz_set(temp3, temp);
    //hard coce check for 0-3 issue with primes. otherwise its dynamic
    if ((mpz_cmp_ui(n, 1)) == 0) {
        return false;
    }
    if ((mpz_cmp_ui(n, 0)) == 0) {
        return false;
    }
    if ((mpz_cmp_ui(n, 2)) == 0) {
        return true;
    }
    if ((mpz_cmp_ui(n, 3)) == 0) {
        return true;
    }
    if (((mpz_odd_p(n)) == 0)) {
        return false;
    }

    //do the and r and s
    //check if the modulus is still 2
    //if its still 2, then we continue dividing by 2 and adding to s
    while ((mpz_mod_ui(temp, temp, 2)) == 0) {
        mpz_add_ui(s1, s1, 1);

        mpz_tdiv_q_ui(divhold, divhold, 2);

        mpz_set(temp, divhold);
        mpz_set(temp4, divhold);

        mpz_set(r, divhold);
    }
    //gmp_printf("value r: %Zd\n",r);
    //gmp_printf("value s: %Zd\n",s1);

    //now do the algothrithm
    mpz_t j;
    mpz_t y;
    mpz_t a;
    mpz_t stemp;
    mpz_init(stemp);
    mpz_t temp2;
    mpz_init(j);
    mpz_init(y);
    mpz_init(a);
    mpz_init(temp2);
    //create the variables for mpz_t
    mpz_sub_ui(stemp, s1, 1); //stemp is s-1
    mpz_sub_ui(temp2, n, 1); //temp2 is n-1
    mpz_t two;
    mpz_init(two);
    mpz_set_ui(two, 2); //holds value of 2
    //do until the given iters
    for (uint64_t x = 1; x < iters; x++) {
        do {
            mpz_urandomm(a, state, temp2);
            // random numbers must be 2 to n-2
        } while (((mpz_cmp_ui(a, 0) == 0) || (mpz_cmp_ui(a, 1) == 0) || (mpz_cmp_ui(a, 2) == 0)));

        pow_mod(y, a, r, n); //call power mod of a^r mond n
        //if its not 1 and not n-1
        if ((!(mpz_cmp_ui(y, 1) == 0)) && (!(mpz_cmp(y, temp2) == 0))) {
            mpz_set_ui(j, 1); //set j to 1
            //if not s-1 and not n-1
            while (((mpz_cmp(j, stemp)) <= 0) && (!((mpz_cmp(y, temp2) == 0)))) {
                //y gets y y^y2 mod n
                pow_mod(y, y, two, n);
                if ((mpz_cmp_ui(y, 1)) == 0) {
                    mpz_clears(
                        j, y, a, stemp, two, temp2, s1, r, temp, temp3, divhold, temp4, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            //if not n-2
            if (!(mpz_cmp(y, temp2) == 0)) {
                mpz_clears(j, y, a, stemp, two, temp2, s1, r, temp, temp3, divhold, temp4, NULL);
                return false;
            }
        }
    }
    mpz_clears(j, y, a, stemp, two, temp2, s1, r, temp, temp3, divhold, temp4, NULL);
    return true; //true if its cleears the n-1 and 1
}

//creates primes from the bits and p and q sizing. value is passed back via the mpz_t p
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t prime;
    mpz_init(prime);

    bool found_prime = false;

    mpz_urandomb(prime, state, bits + 1); //make a random number in bit size

    //if were not a prime, we prob aren't
    while (found_prime == false) {
        //check if were 2 otherwise return
        if ((mpz_cmp_ui(prime, 2)) == 0) {
            mpz_set(p, prime);
            return;
        }
        //
        //gmp_printf("\ndef not %Zd\n",prime);
        //if were odd then check if were a prime call is_prime
        if ((mpz_odd_p(prime) != 0)) {

            found_prime = is_prime(prime, iters);
        }

        //if it was not a prime then we call again and redo the while loop
        if (found_prime == false) {

            mpz_urandomb(prime, state, bits + 1);
        }
    }
    //size_t gets=0;
    //gets=mpz_sizeinbase(prime,2);
    //printf("\nbits num %zu\n",gets);

    mpz_set(p, prime);
    mpz_clears(prime, NULL);
    return;
}

//check the greated common factors given a and b. the value is passed back through the mpz_t d
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t t;
    mpz_init(t);

    //if the b is not 0 then we set the t to b, b to ab and a to t
    while (!(mpz_cmp_ui(b, 0) == 0)) {
        mpz_set(t, b);
        mpz_mod(b, a, b);
        mpz_set(a, t);
    }
    //set d to a. we dont wanna destory or overwrite the parameters
    mpz_set(d, a);

    mpz_clears(t, NULL);
    return;
}

//this does the inverse of pow_mod
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    //miles course staff discord. thanks
    mpz_t r, r_prime, t, t_prime, q1, temp, temp2;
    mpz_inits(r, r_prime, t, t_prime, q1, temp, temp2, NULL);
    mpz_set(r, n);
    mpz_set(r_prime, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(t_prime, 1);
    //set the values and holds above for parrel assignment
    while (!(mpz_cmp_ui(r_prime, 0) == 0)) {
        mpz_fdiv_q(q1, r, r_prime);
        mpz_set(temp2, r);
        mpz_set(r, r_prime);
        mpz_mul(temp, q1, r_prime);
        mpz_sub(r_prime, temp2, temp);
        mpz_set(temp2, t);
        mpz_set(t, t_prime);
        mpz_mul(temp, q1, t_prime);
        mpz_sub(t_prime, temp2, temp);
        //does r and r prime gets r prime and r-q*r prime
        //does t and t prime gets t prime and t-q* t prime
    }
    //if the r is greater than 1
    if (((mpz_cmp_ui(r, 1)) > 0)) {
        mpz_set_ui(i, 0);
        mpz_clears(r, r_prime, t, t_prime, q1, temp, temp2, NULL);
        return;
    }
    //if the t than 0
    if (((mpz_cmp_ui(t, 0))) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(i, t);
    mpz_clears(r, r_prime, t, t_prime, q1, temp, temp2, NULL);
    return;
    //mpz_clears(r,r_prime,t,t_prime,q1,temp,temp2,NULL);
}
