#include "S_DES.hpp"
#include "SCT.hpp"
#include "ES_DES.hpp"

std::string ES_DES::encrypt(std::string plain, std::string key, bool verbose)
{
    // Declare
    std::string sctRounds, rowShifted, encrypted;

    // Show step if required
    if (verbose)
        std::cout << "Input:\t" << plain << "\n";

    // Execute SCTMR
    sctRounds = plain;
    for (int i = 0; i < SCTMR_ROUNDS; i++)
    {
        sctRounds = SCT::encrypt(sctRounds,verbose); 

        // Show step if required
        if (verbose)                                            
            std::cout << "Output:\t" << sctRounds << "\n\n";
    } // End for

    // Row shifting
    rowShifted = SCT::shiftRows(sctRounds,verbose);         
    if (verbose)                                            // Show step if required
        std::cout << "Output:\t" << rowShifted << "\n";

    // Encrypt with S-DES
    for (int i = 0; i < plain.length(); i++)
    {
        encrypted.push_back(S_DES::encrypt(rowShifted.at(i), key, verbose));
    } // End for
    
    return encrypted;

} // End encrypt