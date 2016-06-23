#ifndef utils_hpp
#define utils_hpp

#include "md5.h"
#include <string>
#include <cmath>

namespace project1
{

// THESE are global variables to change dimensions of table and arbitrary challenge and secret
static const unsigned int numRows= pow(2,18);      //pow(2,18);
static const unsigned int chainLength = pow(2,10); //pow(2,10)
static const unsigned int challenge = 0x3a67cb9;
static const unsigned int KEYSPACE = chainLength * numRows;
static const int ValidationAttempts = 5;
static const std::string outputFileName = "Project1-18x10noCommaTRACED.csv";

    template <class T>
    inline std::string to_string(T t, std::ios_base & (*f)(std::ios_base&))
    {
        std::ostringstream oss;
        oss << f << t;
        return oss.str();
    }
    
    inline uint64_t mergeKeys(unsigned int upperBits, unsigned int lowerBits) {
        uint64_t md5Input = 0;
        md5Input = (uint64_t)upperBits ;
        md5Input= md5Input << 28;
        md5Input = md5Input^(uint64_t)lowerBits;
        return md5Input;
    }
    
    // Upperbits => Seed (Fob secret <=> ciphertext of previous
    inline unsigned int f(unsigned int upperBits, unsigned int lowerBits,
                          unsigned int chainOffset=0){
        uint64_t appendedKey = mergeKeys(upperBits,lowerBits);
        std::string hexStringIn = to_string<uint64_t>(appendedKey, std::hex); // convert the keyspace length int key to hex string
        //representation using hex alphabet hex(0.. F) i.e. dec(0..15)
        std::string hexStringOut = md5(hexStringIn);  // perform MD5 to get a string
        
        hexStringOut = hexStringOut.substr(hexStringOut.length()-7, hexStringOut.length()); // Get the last 28 bits of information- this is our starting random 20 bit key represented as Text with the alphabet hex(0..f)
        unsigned int cipherText = (unsigned int) stoul(hexStringOut,NULL,16);
        cipherText = (cipherText + chainOffset) % 268435456;  //(2**28)
        return (unsigned int)cipherText;
    }

};

#endif /* utils_hpp */
