// Original program provided by CS50

// Modifies the volume of an audio file by a user-specified factor

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    // Define 8-bit array to store header
    uint8_t header[HEADER_SIZE];

    // Read header from input file into header array
    fread(header, HEADER_SIZE, 1, input);

    // Write header from header array to output file
    fwrite(header, HEADER_SIZE, 1, output);

    // TODO: Read samples from input file and write updated data to output file

    // Define 16-bit array to store audio sample
    int16_t sample;

    // Loop through reading remainder of file in 16-bit (2-byte) increments, reading data to sample
    while (fread(&sample, 2, 1, input) != 0)
    {
        // Adjust to new volume factor
        sample *= factor;

        // Append to output file
        fwrite(&sample, 2, 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
