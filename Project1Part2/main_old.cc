#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include "sha256.h"
#include "BigIntegerLibrary.hh"
#include <cassert>

// Parameters are a file name and a length, used for signature length
// This function reads a file and generates a hash
std::string read_file(std::string filename = "file.txt") {
  std::ifstream infile(filename.c_str(), std::ios::binary);
  std::streampos begin,end;
  begin = infile.tellg();
  infile.seekg (0, infile.end);
  end = infile.tellg();
  std::streampos length = end-begin; //size of the file in bytes
  infile.seekg (0, infile.beg);

  char* memblock = new char[length];
  infile.read (memblock, length); //read the entire file
  memblock[length] = '\0';
  infile.close();
  delete [] memblock;
  return static_cast<std::string>(memblock); // turns memblock into a string; This is the entire original file as a string representation

}

/*
  This function inputs a file that has the contents.
  Either the file containing the secret key or public key will be passed to this function.
  It also takes in the hash of a message.
  Returns a string of the encrypted/decrypted message.
*/
std::string cryptomessage(std::string key_filename, BigUnsigned base) {
  std::ifstream input_key(key_filename.c_str());
  std::string key, n; // strings to hold the values extracted from the file
  for(int i = 0; i < 2; ++i) {
    if(i == 0)
      std::getline(input_key, key); // the first line in the file is e or d
    else
     std::getline(input_key, n); // the second line in the file is n
  }
  input_key.close();

  /****** Encrypt/decrypt using the hash using the key ******/
  return bigUnsignedToString(modexp(BigInteger(base), stringToBigUnsigned(key), stringToBigUnsigned(n)));
}

/*
  Takes a string of the name of a file to be read. The file is read by calling the read_file function.
  The input file is modified by adding a signature, decrypted string of the input file's contents, to the end of the original file.
  Returns the name of the modified input file.
*/
void generate_signature(std::string filename) {
  //filename = filename+".signed";
  std::string memblock = read_file(filename); // get the contents of the original file as a string
  /*std::ifstream infile(filename.c_str(), std::ios::binary);
  std::streampos begin,end;
  begin = infile.tellg();
  infile.seekg (0, infile.end);
  end = infile.tellg();
  std::streampos length = end-begin; //size of the file in bytes
  infile.seekg (0, infile.beg);

  char* memblock = new char[length];
  infile.read (memblock, length); //read the entire file
  memblock[length] = '\0';
  infile.close();
  */
  filename = filename+".signed";
  std::ofstream outfile (filename.c_str(), std::ios::binary);
  char* content = new char[length];
  strcpy(content, memblock);
  outfile.write (content, length); // writes the contents of the original file to the signed file
  int sigLength = 1024; // length of the signature
  char* signature = new char[sigLength]; // character array to hold the signature
  signature[sigLength] = '\0'; // add a terminator
  // Make call to cryptomessage to get the decrypted string of memblock
  std::string decrypt = cryptomessage("d_n.txt", BigUnsignedInABase(sha256(memblock), 16)); // pass the file holding the private key and n
  strcpy(signature, decrypt.c_str()); // copy the decrypted string into the character array
  outfile.write (signature, sigLength); // write the signature to the file
  outfile.close();
  delete [] memblock;
  delete [] content;
  delete [] signature;

  //return filename; // used to get name of signed file that is sent
}

// Takes two strings as arguments. string for the name of the file to check the signature
// Returns true if the document is authentic, and false otherwise.
bool verify_signature(std::string signed_file) {
  //std::string original_file = read_file(original); // gets the entire file as a string
  std::string sign = signed_file+".signed"; //read_file(signed_file); // reads the entire file and signature
  std::ifstream infile(signed_file.c_str(), std::ios::binary);
  std::streampos begin,end;
  begin = infile.tellg();
  infile.seekg (0, infile.end);
  end = infile.tellg();
  std::streampos length = end-begin; //size of the file in bytes
  infile.seekg (0, infile.beg);

  char* memblock = new char[length];
  infile.read (memblock, length); //read the entire file
  memblock[length] = '\0';
  infile.close();
  delete [] memblock;

  sign.erase(sign.begin(), sign.end()-1024); // erases the original from the signed_file to get the signed portion
  std::string signature_encrypt = cryptomessage("e_n.txt", stringToBigUnsigned(sign)); // String that holds the signature encrypted
  // Compare strings and return if they are equal
  return (signature_encrypt.compare(sign) == true); // compares the signed_file to the encrypted version of it and returns if they are the same
}

int main(int argc, char *argv[])
{
  switch (*argv[1]) {
    case 's':
      generate_signature(argv[2]);
      break;
    case 'v':
      if(verify_signature(argv[2]) == true)
        std::cout << "The document is authentic\n";
        else
          std::cout << "The document is modified\n";
    default:
      return 0;
  }
/*
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

    }
  }
  */
  return 0;
}
