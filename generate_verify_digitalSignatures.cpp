// Parameters are a file name and a length, used for signature length
// This function reads a file and generates a hash
std::string read_file(std::string filename = "file.txt") {
  std::ifstream infile (filename.c_str(), std::ios::binary);
  std::streampos begin,end;
  begin = infile.tellg();
  infile.seekg (0, std::ios::end);
  end = infile.tellg();
  std::streampos size = end-begin; //size of the file in bytes   
  infile.seekg (0, std::ios::beg);

  char* memblock = new char[size];
  infile.read (memblock, size); //read the entire file
  memblock[size] = '\0';
  infile.close();
  return static_cast<std::string>(memblock); // turns memblock into a string; This is the entire original file as a string representation  

}

/* 
  This function inputs a file that has the contents. 
  Either the file containing the secret key or public key will be passed to this function. 
  It also takes in the hash of a message. 
  Returns a string of the encrypted/decrypted message.
*/
std::string cryptomessage(std::string key_filename, BigUnsigned base) {
  std::ifstream input_key;
  input_key.open(key_filename);
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
std::string generate_signature(std::string filename) {
  filename = filename+".signed";
  std::ofstream outfile (filename.c_str(), std::ios::binary);
  std::string memblock = read_file(filename); // get the contents of the original file as a string
  char* content = new char[memblock.size()];
  strcpy(content, memblock.c_str());
  outfile.write (content, memblock.size()); // writes the contents of the original file to the signed file
  int sigLength = 1024; // length of the signature 
  char* signature = new char[sigLength]; // character array to hold the signature
  signature[sigLength] = '\0'; // add a terminator
  // Make call to cryptomessage to get the decrypted string of memblock
  std::string decrypt = cryptomessage("d_n.txt", BigUnsignedInABase(sha256(memblock), 32)); // pass the file holding the private key and n
  strcpy(signature, decrypt.c_str()); // copy the decrypted string into the character array
  outfile.write (signature, sigLength); // write the signature to the file
  outfile.close();
  return filename; // used to get name of signed file that is sent
}

// Takes two strings as arguments. string for the name of the file to check the signature
// Returns true if the document is authentic, and false otherwise.
bool verify_signature(std::string original, std::string signed_file) {
  std::string original_file = read_file(original); // gets the entire file as a string
  std::string sign = read_file(signed_file); // reads the entire file and signature 
  sign.erase(sign.begin(), sign.end()-original.length()); // erases the original from the signed_file to get the signed portion
  std::string signature_encrypt = cryptomessage("e_n.txt", stringToBigUnsigned(sign)); // String that holds the signature encrypted 
  // Compare strings and return if they are equal
  return (signature_encrypt.compare(sign) == true); // compares the signed_file to the encrypted version of it and returns if they are the same
}