/*

date : 2010/09/04

*/

#include <stdio.h>

#include "timer.h"
#include "bmp.h"
#include "extract.h"

int main(int argc, char *argv[])
{
	if(argc > 1) {
		Image<RGB> image, image2;
		Image<HSV> imageHSV;
		Image<unsigned char> bin;

		Timer timer;
		
		printf("start : load bitmap\n");
		
		timer.Start();
		LoadBitmap(argv[1], &image);
		printf("time : %f\n", timer.Stop());
		
		printf("start : Convert HSV\n");
		timer.Start();
		IP::Convert(&image, &imageHSV);
		printf("time : %f\n", timer.Stop());
		
		HSV max = { 36, 252, 255 };
		HSV min = { 2, 139, 197 };

		printf("start : Extract HSV\n");
		timer.Stop();
		IP::ExtractHSV(&imageHSV, &bin, max, min);
		printf("time : %f\n", timer.Stop());

		printf("start : save bitmap\n");
		timer.Start();
		SaveBitmap("result002.bmp", &bin);
		printf("time : %f\n", timer.Stop());
	}
	return 0;
}
