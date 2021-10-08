#include <iostream>
#include <math.h>
#include <string.h>

class SCT
{

    public:

        // Encryption method
        static std::string encrypt (std::string plaintext, bool verbose = false);

        // Decryption method
        static std::string decrypt(std::string plaintext, bool verbose=false);

        // Shift rows left step method
        static std::string shiftRowsLeft(std::string plaintext, bool verbose=false);

        // Shift rows right step method
        static std::string shiftRowsRight(std::string plaintext, bool verbose=false);

    private:

        // Number of columns 
        static unsigned int const N_COLS = 3;
        static unsigned int const COL_ORDER[3];

        // Shift Row left
        static void shiftRowLeft(unsigned char matrix[][SCT::N_COLS], unsigned int row);

        // Shift Row rifht
        static void shiftRowRight(unsigned char matrix[][SCT::N_COLS], unsigned int row);

        // Method for shifting rows
        static std::string shiftRows(std::string plaintext, bool left, bool verbose);

        // Show matrix
        static void showMatrix(unsigned char matrix[][SCT::N_COLS], unsigned int nRows);

}; // End SCT 