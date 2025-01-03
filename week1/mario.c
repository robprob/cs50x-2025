#include <cs50.h>
#include <stdio.h>

void printLine(int numBlanks, int numBlocks);
void printBlanks(int numBlanks);
void printBlocks(int numBlocks);

int main(void)
{
    int height = 0;

    // Obtain input until valid number entered
    while (height < 1 || height > 8)
    {
        height = get_int("How tall? ");
    }

    for (int i = 1; i <= height; i++)
    {
        int numBlanks = height - i;
        int numBlocks = i;
        printLine(numBlanks, numBlocks);
    }
}

void printLine(int numBlanks, int numBlocks)
{
    printBlanks(numBlanks);
    printBlocks(numBlocks);
    printBlanks(2);
    printBlocks(numBlocks);
    printf("\n");
}

void printBlanks(int numBlanks)
{
    for (int i = 0; i < numBlanks; i++)
    {
        printf(" ");
    }
}

void printBlocks(int numBlocks)
{
    for (int i = 0; i < numBlocks; i++)
    {
        printf("#");
    }
}
