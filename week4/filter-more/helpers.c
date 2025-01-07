// Added helpers functionality

#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iter pixel rows
    for (int i = 0; i < height; i++)
    {
        // Iter pixel columns in that row
        for (int j = 0; j < width; j++)
        {
            // Total color intensity values
            int totalIntensity = 0;
            totalIntensity += image[i][j].rgbtRed;
            totalIntensity += image[i][j].rgbtGreen;
            totalIntensity += image[i][j].rgbtBlue;

            // Calculate intensity as a single constant average
            int avgIntensity = (int) round((double) totalIntensity / 3);

            // Set each color intensity as that average (C passes arrays by reference)
            image[i][j].rgbtRed = avgIntensity;
            image[i][j].rgbtGreen = avgIntensity;
            image[i][j].rgbtBlue = avgIntensity;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iter pixel rows
    for (int i = 0; i < height; i++)
    {
        // Iter pixel columns in first half of row
        for (int j = 0; j < width / 2; j++)
        {
            // Store temporary pixel for swapping
            RGBTRIPLE tempPixel = image[i][width - j - 1];

            // Swap along middle of image
            image[i][width - j - 1] = image[i][j];
            image[i][j] = tempPixel;
        }
    }
}

// Update total counts of surrounding color intensity and pixel count
void updateSurrounding(int row, int col,
                       int *totalRed, int *totalGreen, int *totalBlue,
                       int *pixelCount,
                       int height, int width, RGBTRIPLE image[height][width])
{
    *totalRed += image[row][col].rgbtRed;
    *totalGreen += image[row][col].rgbtGreen;
    *totalBlue += image[row][col].rgbtBlue;
    *pixelCount += 1;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for new RGBTRIPLE array
    RGBTRIPLE newImage[height][width];

    // Iter pixel rows
    for (int i = 0; i < height; i++)
    {
        // Iter pixel columns in that row
        {
            for (int j = 0; j < width; j++)
            {
                // Calculate average intensity of surrounding pixels
                int totalRed = 0;
                int totalGreen = 0;
                int totalBlue = 0;

                int pixelCount = 0;

                // Evaluate central pixel, itself
                updateSurrounding(i, j, &totalRed, &totalGreen, &totalBlue, &pixelCount, height,
                                  width, image);

                // Up
                if ((i - 1) >= 0)
                {
                    updateSurrounding(i - 1, j, &totalRed, &totalGreen, &totalBlue, &pixelCount,
                                      height, width, image);

                    // Up-left
                    if ((j - 1) >= 0)
                    {
                        updateSurrounding(i - 1, j - 1, &totalRed, &totalGreen, &totalBlue,
                                          &pixelCount, height, width, image);
                    }

                    // Up-right
                    if ((j + 1) < width)
                    {
                        updateSurrounding(i - 1, j + 1, &totalRed, &totalGreen, &totalBlue,
                                          &pixelCount, height, width, image);
                    }
                }

                // Left
                if ((j - 1) >= 0)
                {
                    updateSurrounding(i, j - 1, &totalRed, &totalGreen, &totalBlue, &pixelCount,
                                      height, width, image);
                }

                // Right
                if ((j + 1) < width)
                {
                    updateSurrounding(i, j + 1, &totalRed, &totalGreen, &totalBlue, &pixelCount,
                                      height, width, image);
                }

                // Down
                if ((i + 1) < height)
                {
                    updateSurrounding(i + 1, j, &totalRed, &totalGreen, &totalBlue, &pixelCount,
                                      height, width, image);

                    // Down-left
                    if ((j - 1) >= 0)
                    {
                        updateSurrounding(i + 1, j - 1, &totalRed, &totalGreen, &totalBlue,
                                          &pixelCount, height, width, image);
                    }

                    // Down-right
                    if ((j + 1) < width)
                    {
                        updateSurrounding(i + 1, j + 1, &totalRed, &totalGreen, &totalBlue,
                                          &pixelCount, height, width, image);
                    }
                }

                // Calculate average intensity of each RGB value to add new pixel to newImage array
                newImage[i][j].rgbtRed = (int) round((double) totalRed / pixelCount);
                newImage[i][j].rgbtGreen = (int) round((double) totalGreen / pixelCount);
                newImage[i][j].rgbtBlue = (int) round((double) totalBlue / pixelCount);
            }
        }
    }

    // Update image by copying from the new image created
    for (int j = 0; j < height; j++)
    {
        for (int k = 0; k < width; k++)
        {
            image[j][k] = newImage[j][k];
        }
    }
}

// Update intermediate Sobel values for edge detection
void updateSobel(int row, int col,
                 int kernelFactor,
                 int *redG, int *greenG, int *blueG,
                 int height, int width, RGBTRIPLE image[height][width])
{
    // Multiply RGB value by specified kernel factor, adding to cumulative G value for that color
    *redG += image[row][col].rgbtRed * kernelFactor;
    *greenG += image[row][col].rgbtGreen * kernelFactor;
    *blueG += image[row][col].rgbtBlue * kernelFactor;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for new RGBTRIPLE array
    RGBTRIPLE newImage[height][width];

    // Iter pixel rows
    for (int i = 0; i < height; i++)
    {
        // Iter pixel columns in that row
        {
            for (int j = 0; j < width; j++)
            {

                // Initialize Sobel intermediate G values for each color
                int redGx = 0;
                int redGy = 0;
                int greenGx = 0;
                int greenGy = 0;
                int blueGx = 0;
                int blueGy = 0;

                // Update G values based on surrounding pixels

                // Up
                if ((i - 1) >= 0)
                {
                    // Gx
                    updateSobel(i - 1, j, 0, &redGx, &greenGx, &blueGx, height, width, image);
                    // Gy
                    updateSobel(i - 1, j, -2, &redGy, &greenGy, &blueGy, height, width, image);

                    // Up-left
                    if ((j - 1) >= 0)
                    {
                        // Gx
                        updateSobel(i - 1, j - 1, -1, &redGx, &greenGx, &blueGx, height, width,
                                    image);
                        // Gy
                        updateSobel(i - 1, j - 1, -1, &redGy, &greenGy, &blueGy, height, width,
                                    image);
                    }

                    // Up-right
                    if ((j + 1) < width)
                    {
                        // Gx
                        updateSobel(i - 1, j + 1, 1, &redGx, &greenGx, &blueGx, height, width,
                                    image);
                        // Gy
                        updateSobel(i - 1, j + 1, -1, &redGy, &greenGy, &blueGy, height, width,
                                    image);
                    }
                }

                // Left
                if ((j - 1) >= 0)
                {
                    // Gx
                    updateSobel(i, j - 1, -2, &redGx, &greenGx, &blueGx, height, width, image);
                    // Gy
                    updateSobel(i, j - 1, 0, &redGy, &greenGy, &blueGy, height, width, image);
                }

                // Right
                if ((j + 1) < width)
                {
                    // Gx
                    updateSobel(i, j + 1, 2, &redGx, &greenGx, &blueGx, height, width, image);
                    // Gy
                    updateSobel(i, j + 1, 0, &redGy, &greenGy, &blueGy, height, width, image);
                }

                // Down
                if ((i + 1) < height)
                {
                    // Gx
                    updateSobel(i + 1, j, 0, &redGx, &greenGx, &blueGx, height, width, image);
                    // Gy
                    updateSobel(i + 1, j, 2, &redGy, &greenGy, &blueGy, height, width, image);

                    // Down-left
                    if ((j - 1) >= 0)
                    {
                        // Gx
                        updateSobel(i + 1, j - 1, -1, &redGx, &greenGx, &blueGx, height, width,
                                    image);
                        // Gy
                        updateSobel(i + 1, j - 1, 1, &redGy, &greenGy, &blueGy, height, width,
                                    image);
                    }

                    // Down-right
                    if ((j + 1) < width)
                    {
                        // Gx
                        updateSobel(i + 1, j + 1, 1, &redGx, &greenGx, &blueGx, height, width,
                                    image);
                        // Gy
                        updateSobel(i + 1, j + 1, 1, &redGy, &greenGy, &blueGy, height, width,
                                    image);
                    }
                }

                // For each RGB value, "combine" Gx and Gy, calculating (sqrt(Gx^2 + Gy^2)), capping
                // at 255
                int finalRedG =
                    (int) fmin(round(sqrt(pow(redGx, 2.0) + pow(redGy, 2.0))), 255);
                int finalGreenG =
                    (int) fmin(round(sqrt(pow(greenGx, 2.0) + pow(greenGy, 2.0))), 255);
                int finalBlueG =
                    (int) fmin(round(sqrt(pow(blueGx, 2.0) + pow(blueGy, 2.0))), 255);

                // Updated new image with the edge-detection RGB values
                newImage[i][j].rgbtRed = finalRedG;
                newImage[i][j].rgbtGreen = finalGreenG;
                newImage[i][j].rgbtBlue = finalBlueG;
            }
        }
    }

    // Update image by copying from the new image created
    for (int j = 0; j < height; j++)
    {
        for (int k = 0; k < width; k++)
        {
            image[j][k] = newImage[j][k];
        }
    }
}
