// Standard libraries
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <cassert>
// `BigIntegerLibrary.hh' includes all of the library headers
// This library is from https://mattmccutchen.net/bigint/index.html 
// The library is not included in the folder for this repository
#include "BigIntegerLibrary.hh"

/*
  Fermat Test Algorithm
  This test returns true if the input value is a prime, 
  otherwise it returns false.

*/
bool fermat_primality(unsigned int position, BigUnsigned n) {
  std::srand(std::time(0));
  int a = std::rand() % 15 + 2; // randomly generate an integer greater than 1 change to start with srand
  assert(BigUnsigned(a) < n); // assert a is less than n

  while(position < 3) { // will execute this loop 3 times 
    if(modexp(a, n-1, n) == (BigUnsigned(1))) 
      fermat_primality(++position, n);
    else {
      return false;
    }
  }
  // If n passes through the loop, return true
  return true;
}
/*
  Function that generates a random number.
  If the generated value returns false when passed into the fermat_primality test, the function is called again until the generated value is prime. 
*/
BigUnsigned generate_p() {
  BigUnsigned p = BigUnsigned(1);

  for(int i = 0; i < 80; ++i) {
    std::srand(std::time(0));
    p = p*71 + std::rand();
  }

  return ((fermat_primality(0, p) == false) ? BigUnsigned(generate_p()) : p);
}
BigUnsigned generate_q() {
  BigUnsigned q = BigUnsigned(1);

  for(int i = 0; i < 77; ++i) {
    std::srand(std::time(0));
    q = q*83 + std::rand();
  }

    //assert((a*2 + 1) % 2 == 1); // assert a is odd}
  return ((fermat_primality(0, q) == false) ? BigUnsigned(generate_q()) : q);

}

// This function generates a value for e
BigUnsigned generate_e(BigUnsigned phi, BigUnsigned e) {
  if(gcd(phi, e) == 1)
    return e;
  else {
    return generate_e(phi, e + 2);
  }
}

int main() {

  // Variables 
  std::fstream datafile; // file used to save p and q 
  /* The library throws `const char *' error messages when things go
   * wrong.  It's a good idea to catch them using a `try' block like this
   * one.  Your C++ compiler might need a command-line option to compile
   * code that uses exceptions. */
  try {
      
      BigUnsigned p = generate_p(); // Call generate function to get prime number p
      BigUnsigned q = generate_q(); // Call generate function to get prime number q
      assert(p != q); // verify that p and q are not the same value 
      BigUnsigned n = p * q;
      BigUnsigned phi = (p - 1) * (q - 1);
      BigUnsigned e = generate_e(phi, 3); // call function to get value relatively prime to phi
      BigUnsigned d = modinv(e, phi); // set the value of d as the multiplicative inverse of e modulo phi

      // Asserts that the bit length of p and q is greater than or equal to 512 bits for each number respectively 
      assert((p.bitLength() >= 512) && (q.bitLength() >= 512));

      // Code to save p and q to p_q.txt
      datafile.open("p_q.txt", std::ios::out); // Open file in output mode
      datafile << p << "\n"; // p is output to file on its own line
      datafile << q << "\n"; // q is output to file on its own line
      datafile.close();
     // std::cout << p << "\n\n" << q << "\n\n"; // remove this

      // Use the Extended Euclidean Algorithm to generate two pairs of keys
      datafile.open("e_n.txt", std::ios::out);
      datafile << e << "\n";
      datafile << n << "\n";
      datafile.close();

      datafile.open("d_n.txt", std::ios::out);
      datafile << d << "\n";
      datafile << n << "\n";
      datafile.close();
      
      
  } catch(char const* err) {
    std::cout << "The library threw an exception:\n"
      << err << std::endl;
  }

  return 0;
}
