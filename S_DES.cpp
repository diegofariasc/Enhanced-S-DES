
#include "S_DES.hpp"

// Initialization of tables 
unsigned char const S_DES::IP[8]    = {2,6,3,1,4,8,5,7};
unsigned char const S_DES::FP[8]    = {4,1,3,5,7,2,8,6};
unsigned char const S_DES::EP[8]    = {4,1,2,3,2,3,4,1};
unsigned char const S_DES::P10[10]  = {3,5,2,7,4,10,1,9,8,6};
unsigned char const S_DES::P8[8]    = {6,3,7,4,8,5,10,9};
unsigned char const S_DES::S0[4][4] = {{1,0,3,2},{3,2,1,0},{0,2,1,3},{3,1,3,2}};
unsigned char const S_DES::S1[4][4] = {{0,1,2,3},{2,0,1,3},{3,0,1,0},{2,1,0,3}};
unsigned char const S_DES::P4[4]    = {2,4,3,1};

unsigned char S_DES::execute(char inputChar, std::string key, bool verbose, bool encrypt)
{
    // Declare bitsets
    std::tuple<std::bitset<8>, std::bitset<8>> subkeys;
    std::tuple<std::bitset<4>, std::bitset<4>> outputR1, outputR2;
    std::bitset<8> input, ip, fp, k1, k2, sw, roundsOut, output;
    std::bitset<10> keyBits;

    // Convert to bitset and push plain bits
    input = std::bitset<8>(inputChar);

    // Apply initial permutation
    ip = S_DES::permute<8,8>(input, S_DES::IP);

    // Display initial steps if required
    if (verbose)
    {
        std::cout << "Parameters:\n";
        std::cout << "Input:\t" << input << "\n";       // Received bits
        std::cout << "Key:\t" << key << "\n";           // Received key bits
        std::cout << "IP:\t" << ip << "\n\n";           // Initial permutation result
    } // End if
    
    // Rounds

    // Convert bits string to bitset
    keyBits = std::bitset<10>(key);

    // Call for subkey generation
    subkeys = S_DES::generateSubkeys(keyBits, verbose);
    k1 = std::get<0>(subkeys);
    k2 = std::get<1>(subkeys);

    // Execute round 1
    if (encrypt)
        outputR1 = S_DES::doRound(ip, k1, verbose);
    else
        outputR1 = S_DES::doRound(ip, k2, verbose);

    // Join applying switch operation
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            sw.set(i, std::get<0>(outputR1)[i]);
        else
            sw.set(i, std::get<1>(outputR1)[i - 4]);

    } // End for   

    // Print switch result if required
    if (verbose)
        std::cout << "SW:\t" << sw << "\n";

    // Execute round 2
    if (encrypt)
        outputR2 = S_DES::doRound(sw, k2, verbose);
    else
        outputR2 = S_DES::doRound(sw, k1, verbose);

    // Join round 2 results
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            roundsOut.set(i, std::get<1>(outputR2)[i]);
        else
            roundsOut.set(i, std::get<0>(outputR2)[i - 4]);

    } // End for   

    // Apply final permutation
    output = S_DES::permute<8,8>(roundsOut,S_DES::FP);

    if (verbose)
    {
        std::cout << "\nOutput:\n";
        std::cout << "Rounds:\t" << roundsOut << "\n";  // Received plaintext bits
        std::cout << "IP-1:\t" << output << "\n\n"; // Final permutation result
    } // End if

    return static_cast<unsigned char>( output.to_ulong() );

} // End execute

unsigned char S_DES::decrypt(char cipher, std::string key, bool verbose)
{
    return S_DES::execute(cipher,key,verbose,false);

} // End decrypt

unsigned char S_DES::encrypt(char plain, std::string key, bool verbose)
{
    return S_DES::execute(plain,key,verbose,true);

} // End encrypt


template <size_t IN, size_t OUT>
std::bitset<OUT> S_DES::permute(std::bitset<IN> bits, const unsigned char* table)
{
    // Declare
    std::bitset<OUT> result;

    // Set bits according to permutation table
    for (int i = 0; i < OUT; i++)
    {
        result.set(OUT - i - 1, bits[IN - table[i]]);
    } // End for

    return result;

} // End permute

template <size_t N>
std::bitset<N> S_DES::rotateLeft(std::bitset<N> input)
{

    // Declare
    std::bitset<N> shifted;

    // Shift one place at left
    for (int i = 0; i < N - 1; i++)
    {
        shifted.set(i+1, input[i]);
    } // End for

    // Recover dropped bit at the rightmost
    shifted.set(0, input[N-1]);

    return shifted;

} // End rotateLeft

std::tuple<std::bitset<8>, std::bitset<8>> S_DES::generateSubkeys(std::bitset<10> input, bool verbose)
{
    // Declare
    std::bitset<10> permutedP10, joinedLS1, joinedLS2;
    std::bitset<5> left, right, leftLS1, rightLS1, leftLS2, rightLS2;
    std::bitset<8> k1, k2;

    // Apply permutation P-10
    permutedP10 = S_DES::permute<10,10>(input, S_DES::P10);

    // Split into left and right for rotations
    for (int i = 0; i < 10; i++)
    {
        if (i < 5)
            right.set(i, permutedP10[i]);
        else
            left.set(i - 5, permutedP10[i]);

    } // End for   

    // First left-circular shifts (LS-1)
    leftLS1 = S_DES::rotateLeft(left);
    rightLS1 = S_DES::rotateLeft(right);

    // Join left and right LS-1
    for (int i = 0; i < 10; i++)
    {
        if (i < 5)
            joinedLS1.set(i, rightLS1[i]);
        else
            joinedLS1.set(i, leftLS1[i - 5]);

    } // End for   

    // Apply first P-8 permutation for key 1
    k1 = S_DES::permute<10,8>(joinedLS1,S_DES::P8);

    // Second left-circular shifts (LS-2)
    leftLS2 = S_DES::rotateLeft(S_DES::rotateLeft(leftLS1));
    rightLS2 = S_DES::rotateLeft(S_DES::rotateLeft(rightLS1));

    // Join left and right LS-2
    for (int i = 0; i < 10; i++)
    {
        if (i < 5)
            joinedLS2.set(i, rightLS2[i]);
        else
            joinedLS2.set(i, leftLS2[i - 5]);

    } // End for   

    // Apply first P-8 permutation for key 2
    k2 = S_DES::permute<10,8>(joinedLS2,S_DES::P8);

    // Print procedure if required
    if (verbose)
    {
        std::cout << "Subkey generation:\n";
        std::cout << "Init:\t" << input << "\n";                                // Initial bits
        std::cout << "P-10:\t" << permutedP10 << "\n";                          // Permutation P-10
        std::cout << "LS-1:\t" << leftLS1 << rightLS1 << "\n";                  // Rotated bits
        std::cout << "P-8:\t" << k1 << " (K1) \n";                              // Permutation P-8
        std::cout << "LS-2:\t" << leftLS2 << rightLS2 << "\n";                  // Rotated bits
        std::cout << "P-8:\t" << k2 << " (K2) \n";                              // Permutation P-8
    } // End if

    return {k1, k2};

} // End generateSubkey

std::tuple<std::bitset<4>, std::bitset<4>> S_DES::doRound(std::bitset<8> input, std::bitset<8> subkey, bool verbose)
{
    // Declare
    std::bitset<4> left, right, joinedRight, rightP4, xorP4left;
    std::bitset<8> expanded, xorSubkey;
    std::bitset<2> innerSubLeft, innerSubRight, outerSubLeft, outerSubRight, outS0, outS1;
    unsigned char rowS0, colS0, rowS1, colS1;

    // Split into left and right 
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
            right.set(i, input[i]);
        else
            left.set(i - 4, input[i]);

    } // End for   

    // Expand and permute
    expanded = S_DES::permute<4,8>(right, S_DES::EP);

    // XOR between subkey and expanded right
    xorSubkey = expanded ^ subkey;

    // Split the right part into left and right sub-parts
    // and into inner and outer bits
    outerSubRight[0] = xorSubkey[0];
    innerSubRight[0] = xorSubkey[1];
    innerSubRight[1] = xorSubkey[2];
    outerSubRight[1] = xorSubkey[3];

    outerSubLeft [0] = xorSubkey[4];
    innerSubLeft [0] = xorSubkey[5];
    innerSubLeft [1] = xorSubkey[6];
    outerSubLeft [1] = xorSubkey[7];

    // Get S-Box output
    rowS0 = static_cast<unsigned char>( outerSubLeft.to_ulong() );
    colS0 = static_cast<unsigned char>( innerSubLeft.to_ulong() );
    rowS1 = static_cast<unsigned char>( outerSubRight.to_ulong() );
    colS1 = static_cast<unsigned char>( innerSubRight.to_ulong() );
    outS0 = std::bitset<2>(S_DES::S0[rowS0][colS0]);
    outS1 = std::bitset<2>(S_DES::S1[rowS1][colS1]);

    // Join outputs of S-Boxes
    for (int i = 0; i < 4; i++)
    {
        if (i < 2)
            joinedRight.set(i, outS1[i]);
        else
            joinedRight.set(i, outS0[i - 2]);

    } // End for   

    // Apply P-4 permutation
    rightP4 = S_DES::permute<4,4>(joinedRight, S_DES::P4);

    // Apply XOR with left
    xorP4left = rightP4 ^ left;

    if (verbose)
    {
        std::cout << "\nRound: \n";
        std::cout << "Input:\t" << input << "\n";                                                               // Input bits
        std::cout << "EP (R):\t" << expanded << " <- " << subkey << " (SK) \n";                                 // Expansion and subkey
        std::cout << "XOR:\t" << xorSubkey << "\n";                                                             // XOR with subkey
        std::cout << "SB-0:\tRow: " << outerSubLeft << " Col: " << innerSubLeft << " -> " << outS0 << "\n";     // Inner and outer S0
        std::cout << "SB-1:\tRow: " << outerSubRight << " Col: " << innerSubRight << " -> " << outS1 << "\n";   // Inner and outer S1
        std::cout << "P-4:\t" << rightP4 << " <- " << left << " (L) \n";                                        // Apply P-4 permutation
        std::cout << "XOR:\t" << xorP4left << " <- " << right << " (R) \n";                                     // XOR with left
    } // End if

    return {xorP4left, right};

} // End doRound