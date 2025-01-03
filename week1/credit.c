#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

int numDigits(long cardNum);
void catchInvalidCard();
int parseDigit(long cardNum, int reverseIndex);
int parseIssuerID(long cardNum, int cardLength);
string validateIssuer(int issuerID, int cardLength);

// Prompt user for credit card number, validating and printing credit card issuer
int main(void)
{
    long cardNum = get_long("Number: ");
    int cardLength = numDigits(cardNum);

    // Validate overall card length constraints
    if (cardLength < 13 || cardLength == 14 || cardLength > 16)
    {
        catchInvalidCard();
    }

    // Summation of every other digit multiplied by 2 (starts with second-to-last digit)
    int sumOne = 0;
    for (int i = 1; i < cardLength; i += 2)
    {
        // Parse digit via reverse indexing, multiply by 2
        int product = parseDigit(cardNum, i) * 2;

        // Add each digit of product to cumulative sum
        for (int j = 0; j < numDigits(product); j++)
        {
            sumOne += parseDigit(product, j);
        }
    }

    // Summation of other digits
    int sumTwo = 0;
    for (int i = 0; i < cardLength; i += 2)
    {
        // Parse digit via reverse indexing and add to cumulative sum
        sumTwo += parseDigit(cardNum, i);
    }

    // Add both cumulative summations together
    int totalChecksum = sumOne + sumTwo;

    // Validate modulo 10 of checksum
    if (totalChecksum % 10 != 0)
    {
        catchInvalidCard();
    }

    // Parse issuerID and print respective issuer
    string issuer = validateIssuer(parseIssuerID(cardNum, cardLength), cardLength);
    printf("%s\n", issuer);
}

// Returns number of digits in long
int numDigits(long num)
{
    int count = 0;
    int base = 1;

    if (num == 0)
    {
        return 1;
    }

    while (num > 0)
    {
        count++;
        num /= 10;
    }

    return count;
}

// Catch invalid cards, alerting user and gracefully exiting
void catchInvalidCard()
{
    printf("INVALID\n");
    exit(0);
}

// Parse specified digit using reverse indexing
int parseDigit(long cardNum, int reverseIndex)
{
    // Raise base to correct power of 10
    long base = 1;
    for (int i = 0; i < reverseIndex + 1; i++)
    {
        base *= 10;
    }

    // Parse digit by modulating at correct base and dividing relatively
    return (int) ((cardNum % base) / (base / 10));
}

// Parses issuerID from card number
int parseIssuerID(long cardNum, int cardLength)
{
    int tensPlace = parseDigit(cardNum, cardLength - 1) * 10;
    int onesPlace = parseDigit(cardNum, cardLength - 2);

    return tensPlace + onesPlace;
}

// Validate issuerID against length of card, returning issuer name, if valid
string validateIssuer(int issuerID, int cardLength)
{
    if (issuerID == 34 || issuerID == 37)
    {
        if (cardLength == 15)
        {
            return "AMEX";
        }
    }
    else if (issuerID == 51 || issuerID == 52 || issuerID == 53 || issuerID == 54 || issuerID == 55)
    {
        if (cardLength == 16)
        {
            return "MASTERCARD";
        }
    }
    // Use floor division (automatically via integer division) to find all IDs starting with 4
    else if ((issuerID / 10) == 4)
    {
        if (cardLength == 13 || cardLength == 16)
        {
            return "VISA";
        }
    }

    catchInvalidCard();

    return NULL;
}
