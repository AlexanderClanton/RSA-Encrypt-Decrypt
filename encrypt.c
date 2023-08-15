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
//user specifies the file they want to encrypt  and the output of the encryoted data, user gives the public key file
int main(int argc, char **argv) {
    //takes command line
    //inital variables
    bool tru_val[6] = { false };
    FILE *fp = stdin;
    FILE *outfp = stdout;
    FILE *fppub = stdout;
    //get user input via getopt
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
            fppub = fopen(optarg, "r+");

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
            printf("  -n pbfile      Public key file (default: rsa.pub).\n");

            return 0;
        }
    }
    //if the user did not give a file for the keys go to default
    if ((tru_val[2]) == false) {
        fppub = fopen("rsa.pub", "r+");
    }

    mpz_t n;
    mpz_init(n);
    mpz_t e;
    mpz_init(e);
    mpz_t s;
    mpz_init(s);
    char usrn[200] = { 0 };
    //read the keys stored in the public file
    rsa_read_pub(n, e, s, usrn, fppub);
    //get rids of newline on the username
    usrn[strcspn(usrn, "\n")] = 0;
    //test=strlen(usrn);
    //printf("LENGTH: %d\n", test);
    mpz_t usr;
    mpz_init(usr);
    mpz_set_str(usr, usrn, 62);
    //if the username is correct otherwise exit
    if ((rsa_verify(usr, s, e, n)) == false) {
        printf("bad sig");
        return 0;
    }
    //call encrypt file and it takes the input and using the key to encrypt
    //close and clear keys

    rsa_encrypt_file(fp, outfp, n, e);
    int s2, n1, e1 = 0;
    s2 = mpz_sizeinbase(s, 2);
    //p1 = mpz_sizeinbase(p, 2);
    //q1 = mpz_sizeinbase(q, 2);
    n1 = mpz_sizeinbase(n, 2);
    e1 = mpz_sizeinbase(e, 2);
    //d1 = mpz_sizeinbase(dhold, 2);
    if ((tru_val[5]) == true) {
        printf("User = %s\n", usrn);
        printf("s (%d bits) = ", s2);
        gmp_printf("%Zd\n", s);
        //printf("p (%d bits) = ", p1);
        //gmp_printf("%Zd\n", p);
        //printf("q (%d bits) = ", q1);
        //gmp_printf("%Zd\n", q);
        printf("n (%d bits) = ", n1);
        gmp_printf("%Zd\n", n);
        printf("e (%d bits) = ", e1);
        gmp_printf("%Zd\n", e);
        //printf("d (%d bits) = ", d1);
        //gmp_printf("%Zd\n", dhold);
    }
    fclose(fp);
    fclose(outfp);
    fclose(fppub);
    mpz_clears(n, e, s, usr, NULL);

    return 0;
}
