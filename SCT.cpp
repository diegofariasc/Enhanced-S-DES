#include "SCT.hpp"

// Initialization of tables 
unsigned int const SCT::COL_ORDER[3] = {2,3,1};

std::string SCT::encrypt(std::string plaintext, bool verbose)
{

    // Declare
    unsigned int nRows, row, col; 
    std::string output;

    // Initialize
    nRows = ceil((float) plaintext.length() / (float) N_COLS);      // Compute number of rows
    unsigned char matrix[nRows][N_COLS];                            // Generate array
    memset(matrix, 0, nRows * N_COLS * sizeof(unsigned char));      // Initialize in zero

    // Fill-in matrix           
    row = 0;
    col = 0;
    for (int i = 0; i < plaintext.length(); i++)
    {
        matrix[row][col++] = plaintext.at(i);

        // If end of column is reached move onto the following
        if (col >= N_COLS)
        {
            row++;
            col = 0;
        } // End if

    } // End for

    // Show matrix if required
    if (verbose)
    {
        std::cout << "SCTMR Round:\n";
        SCT::showMatrix(matrix, nRows);
    } // End if

    // Iterate over matrix to retrieve output
    for (int i = 0; i < N_COLS; i++)
    {
        for (int j = 0 ; j < nRows; j++)
        {
            // Declare
            unsigned char character;

            // Initialize
            character = matrix[j][COL_ORDER[i] - 1];

            // Avoid pushing nulls
            if (character != 0)
                output.push_back(character);

        } // End for

    } // End for

    return output;

} // End encrypt



std::string SCT::decrypt(std::string ciphertext, bool verbose)
{

    // Declare
    unsigned int nRows, row, col, missing; 
    std::string output;

    // Initialize
    nRows = ceil((float) ciphertext.length() / (float) N_COLS);     // Compute number of rows
    unsigned char matrix[nRows][N_COLS];                            // Generate array
    memset(matrix, 0, nRows * N_COLS * sizeof(unsigned char));      // Initialize in zero
    missing = (nRows * N_COLS) - ciphertext.length();               // Calculate missing values to fill in matrix

    // Fill-in matrix           
    row = 0;
    col = 0;
    for (int i = 0; i < ciphertext.length(); i++)
    {
        matrix[row++][COL_ORDER[col] - 1] = ciphertext.at(i);

        // If end of column is reached move onto the following
        // Note: consider using (COL_ORDER[col] - 1 >= N_COLS - missing ? 1 : 0)
        // that some columns may be incomplete due to len of plaintext % N_COLS != 0
        if (row + (COL_ORDER[col] - 1 >= N_COLS - missing ? 1 : 0) >= nRows )
        {
            col++;
            row = 0;
        } // End if

    } // End for

    // Show matrix if required
    if (verbose)
    {
        std::cout << "SCTMR Decrypt round:\n";
        SCT::showMatrix(matrix, nRows);
    } // End if

    
    // Iterate over matrix to retrieve output
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0 ; j < N_COLS; j++)
        {
            // Declare
            unsigned char character;

            // Initialize
            character = matrix[i][j];

            // Avoid pushing nulls
            if (character != 0)
                output.push_back(character);

        } // End for

    } // End for

    return output;

} // End encrypt





void SCT::shiftRowLeft(unsigned char matrix[][SCT::N_COLS], unsigned int row)
{
    // Declare
    unsigned char firstChar;

    // Initialize
    firstChar = matrix[row][0];

    // Iterate over shifting row taking element at 
    // column i+1 into i
    for (int i = 0; i < N_COLS - 1; i++)
    {
        matrix[row][i] = matrix[row][i+1];
    } // End for

    // Recover left-dropped element at the end 
    matrix[row][N_COLS - 1] = firstChar;

} // End shiftRowLeft

void SCT::shiftRowRight(unsigned char matrix[][SCT::N_COLS], unsigned int row)
{
    // Declare
    unsigned char lastChar;

    // Initialize
    lastChar = matrix[row][N_COLS - 1];

    // Iterate over shifting row taking element at 
    // column i+1 into i
    for (int i = N_COLS - 1; i > 0; i--)
    {
        matrix[row][i] = matrix[row][i-1];
    } // End for

    // Recover left-dropped element at the end 
    matrix[row][0] = lastChar;

} // End shiftRowLeft

void SCT::showMatrix(unsigned char matrix[][SCT::N_COLS], unsigned int nRows)
{
    // Show matrix
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            if (matrix[i][j] != 0)
                std::cout << matrix[i][j] << " ";
            else
                std::cout << "  ";
        } // End for
        std::cout << "\n";
    } // End for

} // End showMatrix

std::string SCT::shiftRows(std::string plaintext, bool left, bool verbose)
{
    // Declare
    unsigned int nRows, row, col; 
    std::string output;

    // Initialize
    nRows = ceil((float) plaintext.length() / (float) N_COLS);      // Compute number of rows
    unsigned char matrix[nRows][N_COLS];                            // Generate array
    memset(matrix, 0, nRows * N_COLS * sizeof(unsigned char));      // Initialize in zero

    // Fill-in matrix
    row = 0;
    col = 0;
    for (int i = 0; i < plaintext.length(); i++)
    {
        matrix[row][col++] = plaintext.at(i);

        // If end of column is reached move onto the following
        if (col >= N_COLS)
        {
            row++;
            col = 0;
        } // End if

    } // End for

    // For decryption, show matrix if required
    if (verbose)
    {
        std::cout << "Row shifting: \n";
        if (!left)
            SCT::showMatrix(matrix, nRows);
            std::cout << "\n";
    } // End if

    // Iterate over rows
    for (int i = 1; i < nRows; i++)
    {
        // Circular-shift rows N times where N = row number
        for (int j = 0; j < i; j++)
        {
            if (left)
                SCT::shiftRowLeft(matrix, i);
            else
                SCT::shiftRowRight(matrix, i);
        } // End for
    } // End for

    // Show matrix after rotations if required
    if (verbose)
    {
        SCT::showMatrix(matrix, nRows);
    } // End if


    // Iterate over matrix to retrieve output
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < N_COLS; j++)
        {
            // Declare
            unsigned char character;

            // Initialize
            character = matrix[i][j];

            // Avoid pushing nulls
            if (character != 0)
                output.push_back(character);

        } // End for

    } // End for

    return output;

} // End shiftRows

std::string SCT::shiftRowsLeft(std::string plaintext, bool verbose)
{
    return SCT::shiftRows(plaintext, true, verbose);
} // End shiftRowsLeft


std::string SCT::shiftRowsRight(std::string plaintext, bool verbose)
{
    return SCT::shiftRows(plaintext, false, verbose);
} // End shiftRowsRight