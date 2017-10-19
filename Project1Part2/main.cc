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
void read_file(std::string filename = "file.txt") {
  std::ifstream infile(filename.c_str(), std::ios::binary | std::ios::ate);
  std::streampos size;
  size = infile.tellg();

  infile.seekg(0, std::ios::beg);
  char* memblock = new char[size];
  infile.read (memblock, size); //read the entire file
  memblock[size] = '\0';

  infile.close();
  delete [] memblock;
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
  Takes a string of the name of a file to be read. The file is read by calling the read_file function.
  The input file is modified by adding a signature, decrypted string of the input file's contents, to the end of the original file.
*/
std::size_t generate_signature(std::string filename) {
  //\DELETE std::string memblock = read_file(filename); // get the contents of the original file as a string
  std::size_t sigLength;
  std::ifstream infile(filename.c_str(), std::ios::binary | std::ios::ate);
  while(!infile.is_open())
    return 0;
  std::streampos size;
  size = infile.tellg();
  infile.seekg(0, std::ios::beg);
  char* memblock = new char[size];
  infile.read (memblock, size); //read the entire file

  infile.close();
  filename = filename+".signed";
  std::ofstream outfile(filename.c_str(), std::ios::binary);
  outfile.write(memblock, size); // writes the contents of the original file to the signed file
  // Turn memblock into a BigInteger
  BigInteger base = dataToBigInteger(memblock,
  size, BigInteger::positive);
  // Make call to cryptomessage to get the decrypted string of memblock
  std::string decrypt = cryptomessage("d_n.txt", base); // pass the file holding the private key and n
  //\DELETE int sigLength = 1024; // length of the signature
  char* signature = new char[decrypt.length() + 1]; // character array to hold the signature
  strcpy(signature, decrypt.c_str()); // copy the decrypted string into the character array
  outfile.write(signature, decrypt.length()); // write the signature to the file
  std::cout << strlen(signature) << "\n";
  outfile.close();
  sigLength = strlen(signature);
  delete [] memblock;
  //\DELETE delete [] content;
  delete [] signature;
  return sigLength;
}

// Takes a string representing the name of the file to be checked
// Returns true if the document is authentic, and false otherwise.
bool verify_signature(std::string check) {
  // sets check to the entire contents of the file represented as a string
  // check contains the entire file contents and signature, i.e this is expected to be the original content plus the signature
  //check = read_file(check);

  // create a new string named sign and initialize it to check
  // this string will be used to separate the content from the signature
  //std::string sign(check);
  ////unsigned char buffer[check.length()];
  //std::copy(check.c_str(), 1024, )
  //const char* check_array[check.length()];
  //strcpy(check_array, check.c_str());
  const char* check_array = check.c_str(); // create a character array containing the cstring representation of check
  char check2[sizeof(check_array)]; // a charater array the size of the content
  std::cout << check_array;
  //std::cout << "\n" << sizeof(check_array) << " " << sizeof(check2);
  //strncpy(check2, check.c_str(), sizeof(check2));
  strncpy(check2, check_array, sizeof(check2));
  //memmove(check_array+0, check_array+sizeof(check_array)-1024, 1024);
  char* sign_array[1024]; // a character array the size of the signature
  std::cout << check_array << "\n";
  //std::cout << check2 << "\n";
  //check.erase(check.begin(), sizeof(check.c_str())-1024); // erase the
  std::cout << check << "\n";
  //strncpy(check2, check.c_str(), sizeof(check2));

  //delete [] check2;

  //check_array[check.length()-1024] = '\0';
  //strcpy(check_array, check.c_str()); // copy only the length of the content into check_array
  //sign.erase(sign.begin()+0, sign.end()-1024); // erases the original from the check to get the signed portion


  //std::cout << sign << "\n";

  //std::string signature_encrypt = cryptomessage("e_n.txt", stringToBigInteger(sign)); // String that holds the signature encrypted
  // Compare strings and return if they are equal
  //return (signature_encrypt.compare(sign) == true); // compares the check to the encrypted version of it and returns if they are the same
  return true;
}

int main(int argc, char *argv[])
{
  switch (*argv[1]) {
    case 's':
      return static_cast<std::size_t>(generate_signature(argv[2]));
      //\ Maybe output length instead 
      //break;
    case 'v':
      if(verify_signature(argv[2]) == true)
        std::cout << "The document is authentic\n";
        else
          std::cout << "The document is modified\n";
    default:
      return 0;
  }

  return 0;
}
