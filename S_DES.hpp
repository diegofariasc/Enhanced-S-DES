#include <bitset>
#include <iostream>
#include <tuple>

class S_DES
{
    public:
        // Encryption method
        static unsigned char encrypt (std::string plain, std::string key, bool verbose=false);

        // Decryption method
        static unsigned char decrypt(std::string cipher, std::string key, bool verbose=false);

    private: 

        // 10-bit key permutation table
        static unsigned char const P10[10];

        // 10-bit to 8 bit key permutation table
        static unsigned char const P8[8];

        // Initial permutation table
        static unsigned char const IP[8];

        // Final permutation table
        static unsigned char const FP[8];
        
        // Extension/Permutation table
        static unsigned char const EP[8];

        // S-Box 0
        static unsigned char const S0[4][4];

        // S-Box 1
        static unsigned char const S1[4][4];

        // Permutation of right side after S-Boxes (P4)
        static unsigned char const P4[4];

        // Permute a bitset using a permutation table
        template <size_t IN, size_t OUT>
        static std::bitset<OUT> permute(std::bitset<IN> bits, const unsigned char* table);

        // Generate subkeys method
        static std::tuple<std::bitset<8>, std::bitset<8>> generateSubkeys(std::bitset<10> input, bool verbose);

        // Execute a round
        static std::tuple<std::bitset<4>, std::bitset<4>> doRound(std::bitset<8> input, std::bitset<8> subkey, bool verbose);

        // Left shift a collection of bits
        template <size_t N>
        static std::bitset<N> rotateLeft(std::bitset<N> input);

        // Right shift a collection of bits
        template <size_t N>
        static std::bitset<N> rotateRight(std::bitset<N> input);

}; // End S_DES

