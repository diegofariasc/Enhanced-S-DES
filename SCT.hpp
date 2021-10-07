#include <iostream>
#include <math.h>
#include <string.h>

class SCT
{

    public:

        // Encryption method
        static std::string encrypt (std::string plaintext, bool verbose = false);

        // Decryption method
        static std::string decrypt (std::string plaintext);

        // Shift rows step method
        static std::string shiftRows (std::string plaintext, bool verbose=false);

    private:

        // Number of columns 
        static unsigned int const N_COLS = 3;
        static unsigned int const COL_ORDER[3];

        // Shift Row
        static void shiftRow(unsigned char matrix[][SCT::N_COLS], unsigned int row);

        // Show matrix
        static void showMatrix(unsigned char matrix[][SCT::N_COLS], unsigned int nRows);

}; // End SCT 