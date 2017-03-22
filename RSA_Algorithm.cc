// RSA Assignment for ECE4122/6122 Spring 2017
// Implementation by Sean Csukas

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

using namespace std;

#define NUM_PRIME_REPS 100

// Implement the RSA_Algorithm methods here

// Constructor
RSA_Algorithm::RSA_Algorithm()
  : rng(gmp_randinit_default)
{
  // get a random seed for the random number generator
  int dr = open("/dev/random", O_RDONLY);
  if (dr < 0)
    {
      cout << "Can't open /dev/random, exiting" << endl;
      exit(0);
    }
  unsigned long drValue;
  read(dr, (char*)&drValue, sizeof(drValue));
  //cout << "drValue " << drValue << endl;
  rng.seed(drValue);
// No need to init n, d, or e.
}

// Generate key
void RSA_Algorithm::GenerateRandomKeyPair(size_t sz) {

  // Initialize p and q
  mpz_class p, q;

  int isPrime = 0;

  // Generate prime number p
  while (!isPrime) {
    p = rng.get_z_bits(sz);
    isPrime = mpz_probab_prime_p(p.get_mpz_t(),NUM_PRIME_REPS);
  }

  isPrime = 0;

  // Generate prime number q
  while (!isPrime) {
    q = rng.get_z_bits(sz);
    isPrime = mpz_probab_prime_p(q.get_mpz_t(),NUM_PRIME_REPS);
  }

  // Compute n
  n = p * q;

  // Compute phi(n)
  mpz_class phin = (p - 1) * (q - 1);

  // Initialize gcd
  mpz_class gcd;

  // Generate random key d
  while (!(1 < d and d < phin and gcd == 1)) {
    d = rng.get_z_bits(sz * 2);
    mpz_gcd(gcd.get_mpz_t(),phin.get_mpz_t(),d.get_mpz_t());
  }


  // Compute key e
  mpz_invert(e.get_mpz_t(),d.get_mpz_t(),phin.get_mpz_t());

}


// Encrypt plaintext message M with key  pair n/e
// By convention, we will make the encryption key e the public key
// and the decryption key d the private key.
mpz_class RSA_Algorithm::Encrypt(mpz_class M) {
  mpz_class C;

  mpz_powm(C.get_mpz_t(),M.get_mpz_t(),e.get_mpz_t(),n.get_mpz_t());

  return C;
}

// Decrypt ciphertext message C with key pair n/d
mpz_class RSA_Algorithm::Decrypt(mpz_class C) {
  mpz_class M;

  
  mpz_powm(M.get_mpz_t(),C.get_mpz_t(),d.get_mpz_t(),n.get_mpz_t());

  return M;
}

void RSA_Algorithm::PrintND()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(mpz_class M)
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(mpz_class C)
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}




