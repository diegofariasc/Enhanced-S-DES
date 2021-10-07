#include "S_DES.hpp"

int main(void)
{

    S_DES::encrypt("10010111","1010000010",true);
    S_DES::decrypt("00111000","1010000010",true);

} // End main