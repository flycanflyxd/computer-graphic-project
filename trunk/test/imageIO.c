#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
	unsigned char R, G, B;  /* Red, Green, Blue */
} Pixel;

typedef struct {
	Pixel *pPixel;
	int xRes, yRes;
} ColorImage;

void initColorImage(int xSize, int ySize, ColorImage *image);
void freeColorImage(ColorImage *image);
void clearColorImage(Pixel background, ColorImage *image);
Pixel* readPixel(int x, int y, ColorImage *image);
void writePixel(int x, int y, Pixel p, ColorImage *image);
void readPPM(char *filename, ColorImage *image);
void outputPPM(char *filename, ColorImage *image);


void initColorImage(int xSize, int ySize, ColorImage *image)
{
	Pixel p = {0,0,0};
	image->xRes = xSize;
	image->yRes = ySize;
	image->pPixel = (Pixel*) malloc(sizeof(Pixel)*xSize*ySize);
	clearColorImage(p, image);
}

void freeColorImage(ColorImage *image)
{
	free(image->pPixel);
}

void clearColorImage(Pixel background, ColorImage *image)
{
	int i;

	if (! image->pPixel) return;
	for (i=0; i<image->xRes*image->yRes; i++) image->pPixel[i] = background;
}

Pixel* readPixel(int x, int y, ColorImage *image)
{
	assert(image->pPixel); /* die if image not initialized */
	return &image->pPixel[x + y * image->yRes];
}

void writePixel(int x, int y, Pixel p, ColorImage *image)
{
	assert(image->pPixel); /* die if image not initialized */
	image->pPixel[x + y*image->yRes] = p;
}

void readPPM(char *filename, ColorImage *image)
{
    FILE *inFile = fopen(filename, "rb");
    char buffer[1024];
    int xRes, yRes;

	assert(inFile); /* die if file can't be opened */

    fgets(buffer, 1024, inFile);
    if (0 != strncmp(buffer, "P6", 2)) {
       printf("Sorry, only P6 format is currently supported for PPM files.\n");
       exit(1);
    }

    fgets(buffer, 1024, inFile);
    while ('#' == buffer[0]) {  // skip the comment lines
        fgets(buffer, 1024, inFile);
    }

	sscanf(buffer, "%d %d", &xRes, &yRes);
	printf("xRes=%d, yRes = %d\n", xRes, yRes);
	initColorImage(xRes, yRes, image);

    fgets(buffer, 1024, inFile); // skip the remaining header lines

	fread(image->pPixel, 1, 3*image->xRes*image->yRes, inFile );
	fclose(inFile);
}

void outputPPM(char *filename, ColorImage *image)
{
    FILE *outFile = fopen(filename, "wb");

	assert(outFile); /* die if file can't be opened */

	fprintf(outFile, "P6 %d %d 255", image->xRes, image->yRes);
	fwrite(image->pPixel, 1, 3*image->xRes*image->yRes, outFile );

	fclose(outFile);
}

/* A test program that generates varying shades of reds. */
int test_main(int argc, char* argv[])
{
	ColorImage image;
	int x, y;
	Pixel p={0,0,0};

	initColorImage(256, 256, &image);
	for (y=0; y<256; y++) {
		for (x=0; x<256; x++) {
			p.R = y;
			writePixel(x, y, p, &image);
		}
	}

	outputPPM("reds.ppm", &image);
}

