/// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <math.h>
#include <cassert>
#include <fstream>

// `BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

/*
  Fermat Test Algorithm
  This test returns true if the input value is a prime, 
  otherwise it returns false

*/
bool primality(BigUnsigned n) {
  int a = 2; // randomly generate a long greater than 1 and less than n.
  assert(BigUnsigned(a) < n);
  if(modexp(a, n-1, n) == (BigUnsigned(1) % n))
    return true;
  else
    return false;
}
/*
  Function that generates a random number.
  If the generated value returns false when passed into the primality test, the function is called again until the generated value is prime. 
*/
BigUnsigned generate() {
  BigUnsigned a = BigUnsigned(1);
  for(int i = 0; i < 80; i++) {
    a = a*71 + rand();
  }
 return ((primality(a) == false) ? BigUnsigned(generate()) : a);

}

int main() {
	/* The library throws `const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a `try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {
      
      // Call generate function to get prime numbers p and q 
      BigUnsigned p = generate(); 
      BigUnsigned q = generate();

      // Asserts that the generated values are both prime
      assert((primality(p) && primality(q)) == true);

      // Asserts that the bit length of p and q is greater than or equal to 512 bits for each number respectively 
      assert((p.bitLength() >= 512) && (q.bitLength() >= 512));

      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}


  // Pass n into primality to see if it is a prime number
  //if(primality(n))
    //std::cout << "n is prime\n";
  //else
    //std::cout << "bad n\n";


	return 0;
}