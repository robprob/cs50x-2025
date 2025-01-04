#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define KEYLENGTH 26

int validateCommandInput(int argc, string argv[]);
string encrypt(string plaintext, string key);

// Takes cipher key that is used to perform substitution encryption on plaintext obtained from user
int main(int argc, string argv[])
{
    // Validate command-line arguments
    if (validateCommandInput(argc, argv) == 1)
    {
        return 1;
    }

    // Define key from command-line input
    string key = argv[1];

    // Obtain user plaintext
    string plaintext = get_string("plaintext: ");

    // Encrypt plaintext using given key, printing ciphertext output
    printf("ciphertext: %s\n", encrypt(plaintext, key));
}

// Validate command-line arguments
int validateCommandInput(int argc, string argv[])
{
    // Validate number of args
    if (argc == 1)
    {
        printf("Error: Please provide a cipher key.\n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("Error: Too many arguments.\n");
        return 1;
    }

    // Validate provided cipher key
    string key = argv[1];

    // Validate length
    if (strlen(key) != KEYLENGTH)
    {
        printf("Error: cipher key must contain 26 characters.\n");
        return 1;
    }

    // Validate alphabetical completeness
    for (int alphaChar = 'A'; alphaChar <= 'Z'; alphaChar++)
    {
        bool matchFound = false;

        // Iterate cipher key looking for matching
        for (int i = 0; i < KEYLENGTH; i++)
        {
            if (toupper(key[i]) == alphaChar)
            {
                matchFound = true;
            }
        }

        if (!(matchFound))
        {
            printf("Error: Key must contain every letter. Key is missing '%c'.\n", alphaChar);
            return 1;
        }
    }

    return 0;
}

// Use given cipher key to encrypt plaintext into ciphertext
string encrypt(string plaintext, string key)
{
    string ciphertext = plaintext;

    // Iterate over plaintext string
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char plainChar = plaintext[i];

        // Skip non-alphabetical chars
        if (isalpha(plainChar) == 0)
        {
            continue;
        }

        // Evaluate case of char before standardizing
        int isUpper = isupper(plainChar);
        plainChar = toupper(plainChar);

        // Locate substitution value by referencing the alphabetically ordered key (i.e. "distance" from 'A')
        ciphertext[i] = toupper(key[plainChar - 'A']);

        // Correct case, if necessary
        if (isUpper == 0)
        {
            ciphertext[i] = tolower(plaintext[i]);
        }
    }

    return ciphertext;
}
