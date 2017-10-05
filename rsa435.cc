// Standard libraries
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <sys/stat.h>
#include "sha256.h"
// `BigIntegerLibrary.hh' includes all of the library headers
// This library is from https://mattmccutchen.net/bigint/index.html 
// The library is not included in the folder for this repository
#include "BigIntegerLibrary.hh"
#include "RSA_functions.cpp"

int main(int argc, char *argv[]) {

  std::fstream datafile; // file used to save p and q 
  /* The library throws `const char *' error messages when things go
   * wrong.  It's a good idea to catch them using a `try' block like this
   * one.  Your C++ compiler might need a command-line option to compile
   * code that uses exceptions. */
  try {
      // This block creates the values needed for RSA
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

  // Generate and verify digital signatures 
  if(argc > 1) {
    // for loop used to pass each string to the function 
    for(int i = 1; i < argc; ++i) {
      // pass argument to main and 0 because this is the original file 
      // and does not have a signature 
      std::string signed_file = generate_signature(argv[i]); // signed_file is the name of the file returned by passing an input file to the function
      
      if((verify_signature(argv[i], signed_file) == true)) // passes the original file and the signed file
        std::cout << "The document is authentic\n";
      else
        std::cout << "The document is modified\n";
      //binary_file(argv[i]); // input the string for each argument passed to main; these are expected to be file names 
      
    }
  }

  return 0;
}
