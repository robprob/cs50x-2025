#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Initialize letter scorings
const string points1 = "AEILNORSTU";
const string points2 = "DG";
const string points3 = "BCMP";
const string points4 = "FHVWY";
const string points5 = "K";
const string points8 = "JX";
const string points10 = "QZ";

int calculateScore(string s);
int charPoints(char c);
bool findMatch(char c, string s);

// Obtain input words from each player, calculating score to output result/winner
int main(void)
{
    // Obtain input words from each player
    string inputP1 = get_string("Player 1: ");
    string inputP2 = get_string("Player 2: ");

    // Calculate word scores
    int scoreP1 = calculateScore(inputP1);
    int scoreP2 = calculateScore(inputP2);

    // Compare scores to deteremine a result/winner
    if (scoreP1 > scoreP2)
    {
        printf("Player 1 wins!\n");
    }
    else if (scoreP2 > scoreP1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// Calculate Scrabble score of given word
int calculateScore(string s)
{
    int score = 0;

    // Iterate through string
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        char c = s[i];

        // Skip char if not alphabetical
        if (isalpha(c) == 0)
        {
            continue;
        }

        // Standardize to upper case
        c = toupper(c);

        // Add respective point value to total score
        score += charPoints(c);
    }

    return score;
}

// Determine respective point value of given char
int charPoints(char c)
{
    int points = 0;

    if (findMatch(c, points1))
    {
        return 1;
    }
    else if (findMatch(c, points2))
    {
        return 2;
    }
    else if (findMatch(c, points3))
    {
        return 3;
    }
    else if (findMatch(c, points4))
    {
        return 4;
    }
    else if (findMatch(c, points5))
    {
        return 5;
    }
    else if (findMatch(c, points8))
    {
        return 8;
    }
    else if (findMatch(c, points10))
    {
        return 10;
    }
    else
    {
        return 0;
    }
}

// Iterate each string of point values to look for char match
bool findMatch(char c, string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == c)
        {
            return true;
        }
    }
    return false;
}
