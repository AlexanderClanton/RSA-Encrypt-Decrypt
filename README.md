#Purpose

#The program takes command line inputs from the user to create a key (use keygen) where the input determines the size of the prime for RSA. The keygen then outputs the public and private information into rsa.pub and rsa.priv.

#The program takes command line input from the user to encrypt a file, give it the unencrypted file and the desired file for output. Where it encrypts using the key files (rsa.pub).

#The program takes command line input from the user to decrypt a file, give it the encrypted file and the desired file for output of the decrypted message. Where decryots uses the key file(rsa.priv).

#The program uses RSA and Miller-Rabin for the primes so their is a very very small chance it won't. Miller-Rabin is quite good

#make

#make all

#make clean

#make format

#Run

#use ./keygen -h for help

#use ./encode -h for help

#use ./decode -h for help

#./keygen -b the size of bits the prime number you want (Use -h for more information)

#./encode -i for the input file (-o for output filename)

#./decode -i filename (-o for output filename)

#program defaults to stdout/stdin if not given a file. If not given bits it defaults to 256, their are other default values. Use -h

#no reported bugs

#thx for reading
