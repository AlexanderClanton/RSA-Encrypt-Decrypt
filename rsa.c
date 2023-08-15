#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <gmp.h>
#include <stdint.h>
#include "math.h"
#include <time.h>
bool logz2(mpz_t n, uint64_t nbit); //this tests if its less than nbits
uint64_t logz22(mpz_t n); //just gimmie the log
bool logz2(mpz_t n, uint64_t nbit) { //elmer credit for this

    uint64_t count = 0;
    mpz_t nhold;
    mpz_init(nhold);
    mpz_set(nhold, n);
    //if its not grater than 0 we done
    while (((mpz_cmp_ui(nhold, 0)) > 0)) {
        mpz_fdiv_q_ui(nhold, nhold, 2);
        count++;
    }
    if (count - 1 == nbit) {
        mpz_clears(nhold, NULL);
        return true;
    }
    if (count >= nbit) {
        mpz_clears(nhold, NULL);
        return true;
    }
    mpz_clears(nhold, NULL);
    return false;
}
uint64_t logz22(mpz_t n) { //elmer credit
    //does log 2 return the value
    uint64_t count = 0;
    mpz_t nhold;
    mpz_init(nhold);
    mpz_set(nhold, n);

    while (((mpz_cmp_ui(nhold, 0)) > 0)) {
        mpz_fdiv_q_ui(nhold, nhold, 2);
        count++;
    }
    mpz_clears(nhold, NULL);
    return count;
}
//this makes the public key via given number of bits, p and q are total of nbits//iters is the number of times we check if its prime
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    uint64_t temp = 0;

    srand((time(0)));
    uint64_t lower = nbits / 4;
    uint64_t upper = ((3 * nbits) / 4);
    uint64_t bittot = 0;

    //the range for the bits
    bittot = (rand() % (upper - lower + 1)) + lower;
    temp = nbits - bittot;

    do {
        if ((bittot == 1 || 0)
            && (temp == 1 || temp == 0)) { //dont have 1 or 0 bit values for really small input
            bittot = (rand() % (upper - lower + 1)) + lower;
            temp = nbits - bittot;
        }
        make_prime(p, bittot, iters);

        make_prime(q, temp, iters);
        mpz_mul(n, p, q); //makes the n

    } while (!(logz2(n, nbits))); //must be up to spec log 2
    //ya ik inits whatever
    mpz_t temp2;
    mpz_init(temp2);
    mpz_t temp3;
    mpz_init(temp3);
    mpz_mul(n, p, q);
    mpz_t tot;
    mpz_init(tot);
    mpz_sub_ui(temp2, p, 1);
    mpz_sub_ui(temp3, q, 1);
    mpz_mul(tot, temp2, temp3);

    mpz_urandomb(e, state, nbits);
    mpz_t dez;
    mpz_init(dez);
    mpz_t ehold; //legacy item for when pow deleted the values, but idk wanna break it

    mpz_init(ehold);
    //this makes sure the bits are good
    do {
        mpz_urandomb(e, state, nbits);

        mpz_set(ehold, e);
        mpz_gcd(dez, e, tot);

    } while (!((mpz_cmp_ui(dez, 1)) == 0));

    mpz_set(e, ehold);
    mpz_clears(temp2, temp3, tot, dez, ehold, NULL);

    return;
}
//this takes the n,e,s mpz_t's and writes them to the file
//adds the newline as aksed and are stored as hexstrings
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx", n);
    fwrite("\n", sizeof(char), 1, pbfile);
    gmp_fprintf(pbfile, "%Zx", e);
    fwrite("\n", sizeof(char), 1, pbfile);
    gmp_fprintf(pbfile, "%Zx", s);
    fwrite("\n", sizeof(char), 1, pbfile);
    fwrite(username, sizeof(char), strlen(username), pbfile);
    fwrite("\n", sizeof(char), 1, pbfile);
    return;
}

//this reads the data from the public file with the username n and public exponet
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    //unix username is 256 characters
    char hold[256] = { 0 };
    //read and skip
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);

    fread(hold, sizeof(char), 256, pbfile);
    //makes sure we got no /n attached
    strcpy(username, hold);
    username[strcspn(username, "\n")] = 0;

    return;
}
//this creates the private key given the d. we find the totient and multiply and then
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {

    mpz_t hold;
    mpz_init(hold);
    mpz_t hold2;
    mpz_init(hold2);
    mpz_t hold3;
    mpz_init(hold3);
    mpz_sub_ui(hold, p, 1);
    mpz_sub_ui(hold2, q, 1);

    mpz_mul(hold3, hold, hold2);
    mod_inverse(d, e, hold3);
    //mpz_invert(d,e,hold3);
    //gmp_printf("D IN3side %Zd\n",d);
    mpz_set(d, d);
    //mod_inverse(d,e,hold3);
    mpz_clears(hold, hold2, hold3, NULL);
    return;
}
//this writes the key we made and n/d to the rsa private file. don't lose these!
//written has hexstrings and /n

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx", n);
    fwrite("\n", sizeof(char), 1, pvfile);
    //gmp_printf("D write %Zd\n",d);
    gmp_fprintf(pvfile, "%Zx", d);
    fwrite("\n", sizeof(char), 1, pvfile);

    return;
}
//read use gmp_fscanf and every newline read again

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);

    return;
}
//use our pow_mod and public exponet
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {

    pow_mod(c, m, e, n);
    //mpz_powm_sec(c,m,e,n);
    return;
}

//encrypts the file, taking our input data and desired encrypted file, our public exponet and n
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t k;
    mpz_init(k);
    mpz_t temp;
    mpz_init(temp);

    //calcuate the block size
    uint64_t loged = 0;
    loged = logz22(n);
    loged -= 1;
    mpz_set_ui(temp, loged);
    mpz_fdiv_q_ui(k, temp, 8);
    uint8_t *arr;
    //this is the block size now in an 64bit int
    uint64_t jb = mpz_get_ui(k);
    arr = (uint8_t *) malloc((jb) * sizeof(uint8_t));
    //make a buffer array on the size of the block
    //uint64_t jb=mpz_get_ui(k);
    arr[0] = 0xFF;
    uint64_t j = 0;
    fseek(infile, 0, SEEK_SET);

    mpz_t m;
    mpz_init(m);
    mpz_t c;
    mpz_init(c);

    jb -= 1;
    //read the data at arr+1 and j-1 total
    //then it converts that to a mpz_t
    //then encrypt it via rsa_encrypt in a block
    //then write that to the outfile

    while ((j = fread(arr + 1, sizeof(uint8_t), jb, infile)) > 0) {
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, arr);
        //gmp_printf("\nThe org value is %Zd\n", m);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    free(arr);
    mpz_clears(k, temp, m, c, NULL);
    return;
}
//takes the c^d mod n to decrpt. d is private
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    //mpz_powm_sec(c,m,d,n);
    return;
}
//this function takes an encrypted file and private d and decrypts it
//outputs a decrypted file with the orginial message
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t k;
    mpz_init(k);
    mpz_t temp;
    mpz_init(temp);
    uint64_t loged = 0;
    loged = logz22(n);
    loged -= 1;
    mpz_set_ui(temp, loged);
    mpz_fdiv_q_ui(k, temp, 8);

    uint64_t jb = mpz_get_ui(k);
    uint8_t *arr;
    arr = (uint8_t *) malloc((jb) * sizeof(uint8_t));
    mpz_t m;
    mpz_init(m);
    mpz_t c;
    mpz_init(c);

    arr[0] = 0xFF;
    uint64_t *j = NULL;
    j = (uint64_t *) malloc(sizeof(uint64_t));

    jb -= 1;

    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        //gmp_printf("\ncypher text is: %Zd\n", c);
        rsa_decrypt(m, c, d, n);
        //gmp_printf("cypher text is: %Zd", m);
        mpz_export(arr, j, 1, sizeof(uint8_t), 1, 0, m);

        fwrite(arr + 1, sizeof(char), (*j) - 1, outfile);
    }
    free(j);
    free(arr);
    mpz_clears(k, temp, m, c, NULL);
    return;
}
//signs it via power exponetian, take the m message to the private d to mod n
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    //mpz_powm(s,m,d,n);
    //gmp_printf("S: %Zd",s);
    pow_mod(s, m, d, n);
    //gmp_printf("S: %Zd", s);
    return;
}

//verify by returning true false if the sig matches aka good keys
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);

    pow_mod(t, s, e, n);

    if (((mpz_cmp(t, m))) == 0) {
        mpz_clears(t, NULL);
        return true;
    }
    mpz_clears(t, NULL);
    return false;
}
