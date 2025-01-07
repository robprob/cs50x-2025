#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int SIGNATURE_SIZE = 4;
const int BLOCK_SIZE = 512;

// Recover JPEG files from a raw forensic image file
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Specify the name of a single forensic image from which to recover JPEGs.\n");
        return 1;
    }

    char *fileName = argv[1];

    // Attempt to open file in read mode
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("Unable to locate and open file.\n");
        return 1;
    }

    // Buffer array to hold block of data
    uint8_t block[BLOCK_SIZE];

    // Array for holding name of current image file
    char imageName[8];

    // Current image count (0-indexed, meaning 000.jpg is first file)
    int imageNum = 0;

    // Placeholder for writing new file out
    FILE *newImage = NULL;

    // Continuously read file, looking for matching JPEG signature
    while (fread(block, sizeof(block), 1, file) == 1)
    {
        // Validate 4-byte signature
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && ((block[3] & 0xf0) == 0xe0))
        {
            // Close previous file, if necessary
            if (newImage != NULL)
            {
                fclose(newImage);
            }

            // Instantiate file to write JPEG data to
            sprintf(imageName, "%03i.jpg", imageNum);
            imageNum++;

            // Open created file to write
            newImage = fopen(imageName, "w");

            // Write block to output file
            fwrite(block, sizeof(block), 1, newImage);
        }
        // Otherwise continue writing out to valid JPEG
        else if (newImage != NULL)
        {
            fwrite(block, sizeof(block), 1, newImage);
        }
    }

    // Prevent memory leakage by closing any open image files before exiting
    if (newImage != NULL)
    {
        fclose(newImage);
    }

    // Close raw forensic file
    fclose(file);

    return 0;
}
