#include "BigIntegerLibrary.hh"
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime>
#include <cmath>
#include <cassert>

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