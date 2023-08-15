#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdbool.h>
#include <string.h>
#define OPTIONS "i:o:n:vh"
#include <unistd.h>
#include <sys/stat.h>
//the program takes encrypted file and the private keys given by the user and decrypts uing said keys
int main(int argc, char **argv) {
    //inital variables
    bool tru_val[6] = { false };
    FILE *fp = stdin;
    FILE *outfp = stdout;
    FILE *fppriv;
    //using getopt to get user inputs
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            tru_val[0] = true;
            fp = fopen(optarg, "r+");

            break;
        case 'o':
            tru_val[1] = true;
            outfp = fopen(optarg, "w+");
            break;

        case 'n':
            tru_val[2] = true;
            fppriv = fopen(optarg, "r+");

            break;

        case 'v': tru_val[5] = true; break;

        case 'h':
            printf("SYNOPSIS\n"); //help prompt
            printf("   Encrypts data using RSA encryption.\nEncrypted Data is decrypted by the "
                   "decrypt program.\n\n");
            printf("USAGE\n   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey -d privkey\n\n");
            printf("OPTIONS\n\n");
            printf("  -h             Display program help and usage.\n");
            printf("  -v             Display verbose program output.\n");
            printf("  -i infile      Input file of data to encrypt (default: stdin).\n");
            printf("  -o outfile     Output file for encrypted data (default: stdout).\n");
            printf("  -n pvfile      Public key file (default: rsa.priv).\n");

            return 0;
        }
    }
    //if the user did not give a file for keys
    if ((tru_val[2]) == false) {
        fppriv = fopen("rsa.priv", "r+");
    }

    mpz_t n;
    mpz_init(n);
    mpz_t d;
    mpz_init(d);

    //read in said private keys to n and d
    rsa_read_priv(n, d, fppriv);

    //call rsa decrypt passing the keys and the ecnrypted file and the desired output file
    rsa_decrypt_file(fp, outfp, n, d);
    //close and clear variables
    int n1, e1 = 0;
    //s2 = mpz_sizeinbase(s, 2);
    //p1 = mpz_sizeinbase(p, 2);
    //q1 = mpz_sizeinbase(q, 2);
    n1 = mpz_sizeinbase(n, 2);
    e1 = mpz_sizeinbase(d, 2);
    //d1 = mpz_sizeinbase(dhold, 2);
    if ((tru_val[5]) == true) {
        //printf("User = %s\n", usrn);
        //printf("s (%d bits) = ", s2);
        //gmp_printf("%Zd\n", s);
        //printf("p (%d bits) = ", p1);
        //gmp_printf("%Zd\n", p);
        //printf("q (%d bits) = ", q1);
        //gmp_printf("%Zd\n", q);
        printf("n (%d bits) = ", n1);
        gmp_printf("%Zd\n", n);
        printf("e (%d bits) = ", e1);
        gmp_printf("%Zd\n", d);
        //printf("d (%d bits) = ", d1);
        //gmp_printf("%Zd\n", dhold);
    }
    fclose(fp);
    fclose(outfp);
    fclose(fppriv);
    mpz_clears(n, d, NULL);

    return 0;
}
