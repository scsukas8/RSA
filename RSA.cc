// ECE4122/6122 RSA Encryption/Decryption assignment
// Spring Semester 2017
// Implementation by Sean Csukas

#include <iostream>
#include "RSA_Algorithm.h"

using namespace std;

#define min_bits 32
#define max_bits 1024
#define num_pairs 100
#define num_msgs 100

int main()
{
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;
  
  //Initialize Messages
  mpz_class M, C;

  // Iterate through encryption sizes
  for (size_t sz = min_bits; sz <= max_bits; sz *= 2) {
    // For each size, generate key pairs
    for (int pairs = 0; pairs < num_pairs; ++pairs) {
      // Generate n, d, and e
      RSA.GenerateRandomKeyPair(sz);

      //Output key pairs
      RSA.PrintNDE();

      // For each key pair, encrypt and decrypt msgs
      for (int msgs = 0; msgs < num_msgs; ++msgs) {
        // Generate 0 < M < n
        M = RSA.rng.get_z_range(RSA.n);

        // Output initial message
        RSA.PrintM(M);

        // Encrypt message
        C = RSA.Encrypt(M);

        // Output Cypher
        RSA.PrintC(C);

        // Decrypt Cypher
        M = RSA.Decrypt(C);
      }
    }
  }
}


  
