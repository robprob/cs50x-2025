#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Define Coleman-Liau constants
const float c1 = 0.0588;
const float c2 = 0.296;
const float c3 = 15.8;

int colemanLiau(string s);
void printResults(int index);

// Implements Coleman-Liau index to estimate reading level of text supplied by user
int main(void)
{
    // Prompt user for text
    string userText = get_string("Text: ");

    // Print results given calculated Coleman-Liau index
    printResults(colemanLiau(userText));
}

// Calculate Coleman-Liau index
// index = 0.0588 * L - 0.296 * S - 15.8
int colemanLiau(string s)
{
    // Parse quantifiable structure of provided text

    int letterCount = 0;
    // Instantiate word count at 1, as first word does not begin with space
    int wordCount = 1;
    int sentenceCount = 0;

    // Iterate through text
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        // Locate alphabetical chars, incrementing letter count
        if (isalpha(s[i]) != 0)
        {
            letterCount++;
        }
        // Locate breaks between words, incrementing word count
        else if (s[i] == ' ')
        {
            wordCount++;
        }
        // Locate end of sentence punctuation, incrementing sentence count
        else if (s[i] == '.' || s[i] == '?' || s[i] == '!')
        {
            sentenceCount++;
        }
    }

    // Calculate algorithm inputs

    // Average letters per 100 words
    float inputL = (100.0 * letterCount) / wordCount;

    // Average sentences per 100 words
    float inputS = (100.0 * sentenceCount) / wordCount;

    // Calculate and return index, rounded to nearest integer
    return (int) round(c1 * inputL - c2 * inputS - c3);
}

// Given Coleman-Liau index, prints corresponding grade level
void printResults(int index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}
