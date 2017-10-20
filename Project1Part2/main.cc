#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/stat.h>
#include "sha256.h"
#include "BigIntegerLibrary.hh"

/// Parameters are a file name and a length, used for signature length
/// This function reads the contents of an entire file
std::string read_file(std::string filename = "file.txt") {
  std::ifstream infile(filename.c_str(), std::ios::binary | std::ios::ate);
  std::streampos size;
  size = infile.tellg();

  infile.seekg(0, std::ios::beg);
  char* memblock = new char[static_cast<size_t>(size) + 1];
  infile.read (memblock, size); //read the entire file
  memblock[size] = '\0';
  std::string str(memblock);
  infile.close();
  delete [] memblock;
  return str;
}

/*
  This function inputs a file that has the contents.
  Either the file containing the secret key or public key will be passed to this function.
  It also takes in the sha256 hash of a message.
  Returns a string of the encrypted/decrypted message.
*/
std::string cryptomessage(std::string key_filename, BigInteger base) {
  std::ifstream input_key(key_filename.c_str());
  std::string key, n; // strings to hold the values extracted from the file
  for(int i = 0; i < 2; ++i) {
    if(i == 0)
      std::getline(input_key, key); // the first line in the file is e or d
    else
     std::getline(input_key, n); // the second line in the file is n
  }
  input_key.close();

  /// Encrypt/decrypt using the hash using the key
  return bigUnsignedToString(modexp(base, stringToBigUnsigned(key), stringToBigUnsigned(n)));
}

/*
  The string passed is a filename that is read into a character array.
  The array is passed into the cryptomessage function.
  The signature is generated and written to a new file that is the name of the original file with .signature extension
*/
void generate_signature(std::string filename) {
  //\DELETE std::string memblock = read_file(filename); // get the contents of the original file as a string
  //\std::size_t sigLength;
  std::ifstream infile(filename.c_str(), std::ios::binary | std::ios::ate);
  while(!infile.is_open())
    return;
  std::streampos size;
  size = infile.tellg();
  infile.seekg(0, std::ios::beg);
  char* memblock = new char[size];
  infile.read (memblock, size); //read the entire file
  memblock[size] = '\0';
  std::string base(memblock);
  base = sha256(base);
  infile.close();
  std::cout << base;
  filename = filename+".signed";
  std::ofstream outfile(filename.c_str(), std::ios::binary);
  //\Skip for now Writes the contents of the original file to the signed file
  outfile.write(memblock, size);
  // Make call to cryptomessage to get the decrypted string of memblock
  std::string decrypt = cryptomessage("d_n.txt", stringToBigInteger(base)); // pass the file holding the private key and n
  char* signature = new char[decrypt.length()]; // character array to hold the signature
  strcpy(signature, decrypt.c_str()); // copy the decrypted string into the character array
  outfile.write(signature, decrypt.length()); // write the signature to the file
  outfile.close();
  //std::ofstream out

  //\sigLength = strlen(signature);
  delete [] memblock;
  delete [] signature;
  //\return sigLength;
}

/*
 Takes two strings, one representing the name of the file to be checked and the second of the signature of the contents.
  Returns true if the document is authentic, and false otherwise.
  If false, encrypting signature will not be the same as the check file to be read.
*/
bool verify_signature(std::string check, std::string signature) {
  // sets check to the entire contents of the file represented as a string
  check = read_file(check); // set check to the file read as a string
  //\std::cout << check << "\n";
  // Create a new string named sign by passing signature
  std::ifstream infile2(signature.c_str());
  std::getline(infile2, signature, '\0');
  infile2.close();
  //\signature = read_file(signature); // set signature to the file read as a string
  //\std::cout << signature << "\n" << "\n";
//  BigInteger base = dataToBigInteger(memblock,
  //size, BigInteger::positive);
  std::string signature_encrypt = cryptomessage("e_n.txt", stringToBigInteger(signature)); // String that holds the signature encrypted
  std::cout << signature_encrypt << "\n";
//  std::string content_encrypt = cryptomessage("d_n.txt", )
  // Compare strings and return if they are equal
  return (signature_encrypt.compare(signature) == true); // compares the check to the encrypted version of it and returns if they are the same
}

int main(int argc, char *argv[])
{
  try {
    switch (*argv[1]) {
      case 's':
        //\return static_cast<std::size_t>(generate_signature(argv[2]));
        generate_signature(argv[2]);
        break;
      case 'v':
        if(verify_signature((argv[2]), argv[3]) == true)
          std::cout << "The document is authentic\n";
          else
            std::cout << "The document is modified\n";
      default:
        return 0;
    }


  } catch(char const* err) {
    std::cout << "The library threw an exception:\n"
      << err << std::endl;
  }

}
