#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdbool.h>
#include <string.h>
#define OPTIONS "bcn:d:svh"
#include <unistd.h>
#include <sys/stat.h>
//creates RSA keys for encryption via given info from the user about size of prime numbers they desire
//prints the keys to default files rsa.pub rsa.priv
int main(int argc, char **argv) {
    //default values
    uint64_t iters = 50;
    uint64_t bits = 256;
    //setting up variables
    bool tru_val[6] = { false };
    FILE *fp = stdout;
    FILE *outfp = stdout;
    uint64_t seed = time(NULL);
    int opt = 0;
    //get user input get opt
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            tru_val[0] = true;
            bits = atoi(argv[optind]);

            break;
        case 'c':
            tru_val[1] = true;
            iters = atoi(argv[optind]);
            break;

        case 'n':
            tru_val[2] = true;
            fp = fopen(optarg, "w+");

            break;

        case 'd':
            tru_val[3] = true;
            outfp = fopen(optarg, "w+");

            break;

        case 's':
            tru_val[4] = true;
            seed = atoi(argv[optind]);
            break;
        case 'v': tru_val[5] = true; break;

        case 'h':
            printf("SYNOPSIS\n"); //help prompt
            printf("   Generates an RSA public/private key pair.\n\n");
            printf("USAGE\n   ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n\n");
            printf("OPTIONS\n\n");
            printf("  -h             Display program help and usage.\n");
            printf("  -v             Display verbose program output.\n");
            printf("  -b bits        Minimum bits needed for public key n.\n");
            printf("  -c confidence  Miller-Rabin iterations for testing primes (default: 50).\n");
            printf("  -n pbfile      Public key file (default: rsa.pub).\n");
            printf("  -d pvfile      Private key file (default: rsa.priv).\n");
            printf("  -s seed        Random seed for testing.\n");

            return 0;
        }
    }

    //sets file permissions
    int filepub = 0;
    int filepriv = 0;

    if (tru_val[2] == false) {
        fp = fopen("rsa.pub", "w+");
    }
    if (tru_val[3] == false) {
        outfp = fopen("rsa.priv", "w+");
    }

    filepub = fileno(fp);
    filepriv = fileno(outfp);

    //if it fails
    if ((fchmod(filepub, 0600) == -1)) {
        printf("error");
        return 0;
    }
    if ((fchmod(filepriv, 0600) == -1)) {
        printf("error2");
        return 0;
    }

    //mpz_t primez;
    //mpz_init(primez);

    mpz_t p;
    mpz_init(p);
    mpz_t q;
    mpz_init(q);
    mpz_t n;
    mpz_init(n);
    mpz_t e;
    mpz_init(e);
    mpz_t d;
    mpz_init(d);
    char *namez = NULL;
    //name holder pointer
    randstate_init(seed);
    //initalize random seed and create the pub and private keys via given bits and iters
    rsa_make_pub(p, q, n, e, bits, iters);

    rsa_make_priv(d, e, p, q);

    mpz_t dhold;
    mpz_init(dhold);
    mpz_set(dhold, d);
    mpz_t dhold2;
    mpz_init(dhold2);
    mpz_set(dhold2, d);
    //gmp_printf("org D is %Zd\n", d);
    //www.oreilly.com/library/view/c-in-a/0596006977/re105.html
    namez = getenv("USER");
    mpz_t usr;
    mpz_init(usr);
    mpz_t s;
    mpz_init(s);
    //mpz_t m;
    //mpz_init(m);
    int length = 0;
    length = strlen(namez);
    //writes the username
    char name2[200] = { 0 };
    for (int z = 0; z < length; z++) {
        name2[z] = namez[z];
    }
    //username into a mpz_t
    mpz_set_str(usr, name2, 62);

    rsa_sign(s, usr, dhold, n);
    //sign the name

    //write the public and private keys
    rsa_write_pub(n, e, s, namez, fp);

    rsa_write_priv(n, dhold2, outfp);
    //verbose information
    int s2, p1, q1, n1, e1, d1 = 0;
    s2 = mpz_sizeinbase(s, 2);
    p1 = mpz_sizeinbase(p, 2);
    q1 = mpz_sizeinbase(q, 2);
    n1 = mpz_sizeinbase(n, 2);
    e1 = mpz_sizeinbase(e, 2);
    d1 = mpz_sizeinbase(dhold, 2);
    if ((tru_val[5]) == true) {
        printf("User = %s\n", namez);
        printf("s (%d bits) = ", s2);
        gmp_printf("%Zd\n", s);
        printf("p (%d bits) = ", p1);
        gmp_printf("%Zd\n", p);
        printf("q (%d bits) = ", q1);
        gmp_printf("%Zd\n", q);
        printf("n (%d bits) = ", n1);
        gmp_printf("%Zd\n", n);
        printf("e (%d bits) = ", e1);
        gmp_printf("%Zd\n", e);
        printf("d (%d bits) = ", d1);
        gmp_printf("%Zd\n", dhold);
    }
    //fclose and clear variables
    fclose(fp);
    fclose(outfp);
    randstate_clear();
    mpz_clears(p, q, n, e, d, usr, dhold, dhold2, s, NULL);

    return 0;
}
