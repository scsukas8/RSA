// Grad student portion of the RSA assignment
// Spring 2017 ECE6122
// Implementation by Sean Csukas

#include <iostream>
#include <stdlib.h> 

#include "RSA_Algorithm.h"

using namespace std;


mpz_class factor(mpz_class n) {
    /* 
    This function is an implementation of Richard Brent's 
    variant of Pollard's rho factoring algorithm.
    */

    // The algorithm's check for division by two is ignored
    // because the factors are expected to be of large magnitude.
        
    //Build rng for initial values
    gmp_randclass rng (gmp_randinit_default);
    
    mpz_class x,y,ys,c,m;
    y = rng.get_z_range(n-1); 
    c = rng.get_z_range(n-1);
    m = rng.get_z_range(n-1);    

    mpz_class k,g,r,q,i,min;
    g = 1;
    r = 1;
    q = 1;

    while (g == 1) {
        x = y;
        for (i = 0; i < r; i++) {
            mpz_powm_ui(y.get_mpz_t(),y.get_mpz_t(),2,n.get_mpz_t());
            y = y + c;
            mpz_powm_ui(y.get_mpz_t(),y.get_mpz_t(),1,n.get_mpz_t());
        }
        k = 0;

        while (k < r && g == 1) {
            ys = y;
            min = r-k;
            if (m < min)
                min = m;

            for (i = 0; i < min; i++) {
                mpz_powm_ui(y.get_mpz_t(),y.get_mpz_t(),2,n.get_mpz_t());
                y = y + c;
                mpz_powm_ui(y.get_mpz_t(),y.get_mpz_t(),1,n.get_mpz_t());
                q = q*(abs(x-y));
                mpz_powm_ui(q.get_mpz_t(),q.get_mpz_t(),1,n.get_mpz_t());
            }
            mpz_gcd(g.get_mpz_t(),q.get_mpz_t(),n.get_mpz_t());
            k = k + m;
        }
        r = r * 2;
    }   


    return g;

    
}

mpz_class factorRSA(mpz_class n,mpz_class e) {
    // Initilize the decryption key d
    mpz_class d;

    // Initialize prime factors p and q
    mpz_class p,q;

    // Factor n to get p and q
    p = factor(n);
    q = n / p;

    // Compute phi(n)
    mpz_class phin = (p - 1) * (q - 1);

    // Compute key d
    mpz_invert(d.get_mpz_t(),e.get_mpz_t(),phin.get_mpz_t());

    return d;
}

int main(int argc, char** argv)
{ // Arguments are as follows:
  //argv[1] = n;
  //argv[2] = e;  // n and e are the public key
  //argv[3] = first 6 characters of encrypted message
  //argv[4] = next 6 characters .. up to argv[12] are the lsat 6 characters
  // The number of arguments will always be exacty 12, and each argument past the
  // public key contain 6 ascii characters of the encrypted message.
  // Each of the 32bit values in the argv[] array are right justified in the
  // low order 48 bits of each unsigned long.  The upper 16 bits are always
  // zero, which insures the each unsigned long is < n (64 bits) and therefore
  // the RSA encryption will work.

  // Below is an example of the BreakRSA and command line arguments:

// ./BreakRSA  2966772883822367927 2642027824495698257  817537070500556663 1328829247235192134 
// 1451942276855579785 2150743175814047358 72488230455769594 1989174916172335943 962538406513796755 
// 1069665942590443121 72678741742252898 1379869649761557209

//   The corect output from the above is:
//   HelloTest  riley CekwkABRIZFlqmWTanyXLogFgBUENvzwHpEHRCZIKRZ
//
//   The broken (computed) private key for the above is 4105243553



  // Our one and only RSA_Algorithm object
  RSA_Algorithm rsa;
  
  // First "break" the keys by factoring n and computing d
  // Set the keys in the rsa object afer calculating d
  rsa.n = mpz_class(argv[1]);
  rsa.e = mpz_class(argv[2]);

  // Set rsa.d to the calculated private key d
  rsa.d = factorRSA(rsa.n,rsa.e);

  for (int i = 3; i < 13; ++i) {
     
      // Decrypt each set of 6 characters
      mpz_class c(argv[i]);
      mpz_class m = rsa.Decrypt(c);

      //  use the get_ui() method in mpz_class to get the lower 48 bits of the m
      unsigned long ul = m.get_ui();

      // Now print the 6 ascii values in variable ul.  
      // As stated above the 6 characters
      // are in the low order 48 bits of ui.
      for (int j = 0; j < 6; ++j)
        {
          unsigned char ch = ul >> (48 - j * 8 - 8);
          cout << ch;
        }
    }
  cout << endl;
}

