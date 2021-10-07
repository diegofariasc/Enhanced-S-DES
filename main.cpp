#include<bitset>
#include "ES_DES.hpp"

#define BIN 0
#define HEX 1

void display(std::string text, unsigned int format);

int main(void)
{
    // Driver code
    // Declare
    std::string text, key, cipher;

    // Initialize
    text = "DIDYOUSEE";
    key = "0010010111";
    cipher = ES_DES::encrypt(text,key,true);

    // Display ciphertext
    std::cout << "Final output:\n";
    display(cipher, BIN);
    display(cipher, HEX);
    

} // End main

void display(std::string text, unsigned int format)
{
    // Indicate the display format
    if (format == BIN)
        std::cout << "Bin:\t";
    else if (format == HEX)
        std::cout << "Hex:\t";

    // Iterate over string
    for (int i = 0; i < text.length(); i++)
    {
        std::bitset<8> byte;
        std::bitset<4> nibbleA, nibbleB;
        
        // Convert char into bitset of 8
        byte = std::bitset<8>(text.at(i));

        // Split byte into nibbles
        for (int j = 0; j < 8; j++)
        {
            if (j < 4)
                nibbleB.set(j, byte[j]);
            else
                nibbleA.set(j - 4, byte[j]);
        } // End for
        
        // Check printing format and display
        if (format == BIN)
            std::cout << nibbleA << " " << nibbleB << " ";
        else
        {
            std::cout << std::uppercase << std::hex << nibbleA.to_ulong();
            std::cout << std::uppercase << std::hex << nibbleB.to_ulong();
        } // End else
            

    } // End for
    std::cout << "\n";

} // End display