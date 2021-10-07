#include <iostream>

class ES_DES
{
    public:

        // Encryption method
        static std::string encrypt(std::string plain, std::string key, bool verbose=false);

        // Decryption method
        static std::string decrypt(std::string cipher, std::string key, bool verbose=false);

    private:
        static unsigned int const SCTMR_ROUNDS = 2;

}; // End class
