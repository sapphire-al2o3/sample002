#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>
#include "image.h"
#include "file.h"

template <typename T>
int LoadBitmap(const char *fileName, Image<T> *dst);

template <typename T>
int SaveBitmap(const char *fileName, const Image<T> *src);

#pragma pack(1)

struct BitmapFileHeader { 
	unsigned short bfType;			// "BM"
	unsigned int   bfSize;			// ファイルサイズ
	unsigned short bfReserved1;		// 0
	unsigned short bfReserved2;		// 0
	unsigned int   bfOffBits;		// 画像データまでのオフセット
};

struct BitmapInfoHeader {
	unsigned int   biSize;			// 構造体のサイズ
	long           biWidth;			// 幅
	long           biHeight;		// 高さ
	unsigned short biPlanes;		// 1
	unsigned short biBitCount;		// ビット数
	unsigned int   biCompression;	// 圧縮方式
	unsigned int   biSizeImage;		// 画像データのサイズ
	long           biXPelsPerMeter;	// 水平解像度
	long           biYPelsPerMeter;	// 垂直解像度
	unsigned int   biClrUsed;		// 色数
	unsigned int   biClrImportant;	// 重要な色数
};

#pragma pack()

inline void ReadLine8(RGB *dst, FileStream *fs, int length, BGRA palette[])
{
	for(int i = 0; i < length; i++) {
		unsigned char index;
		fs->Read(&index, 1);
		dst->r = palette[index].r;
		dst->g = palette[index].g;
		dst->b = palette[index].b;
		dst++;
	}
}

inline void ReadLine8(RGBA *dst, FileStream *fs, int length, BGRA pallete[])
{
	while(length--) {
		unsigned char index;
		fs->Read(&index, 1);
		dst->r = pallete[index].r;
		dst->g = pallete[index].g;
		dst->b = pallete[index].b;
		dst->a = pallete[index].a;
		dst++;
	}
}

inline void ReadLine8(BGR *dst, FileStream *fs, int length, BGRA palette[])
{
	for(int i = 0; i < length; i++) {
		unsigned char index;
		fs->Read(&index, 1);
		dst->r = palette[index].r;
		dst->g = palette[index].g;
		dst->b = palette[index].b;
		dst++;
	}
}

inline void ReadLine8(BGRA *dst, FileStream *fs, int length, BGRA palette[])
{
	for(int i = 0; i < length; i++) {
		unsigned char index;
		fs->Read(&index, 1);
		*dst = palette[index];
		dst++;
	}
}

inline void ReadLine24(RGBA *dst, FileStream *fs, int length)
{
	while(length--) {
		fs->Read(&dst->b, 1);
		fs->Read(&dst->g, 1);
		fs->Read(&dst->r, 1);
		dst->a = 255;
		dst++;
	}
}

inline void ReadLine24(BGRA *dst, FileStream *fs, int length)
{
	while(length--) {
		fs->Read(dst, 3);
		dst->a = 255;
		dst++;
	}
}

inline void ReadLine24(RGB *dst, FileStream *fs, int length)
{
	RGB *end = dst + length;
	while(dst != end) {
		fs->Read(&dst->b, 1);
		fs->Read(&dst->g, 1);
		fs->Read(&dst->r, 1);
		dst++;
	}
}

inline void ReadLine24(BGR *dst, FileStream *fs, int length)
{
	fs->Read(dst, length * sizeof(BGR));
}

inline void ReadLine24(unsigned char *dst, FileStream *fs, int length)
{
	for(int i = 0; i < length; i++) {
		fs->Read(dst++, 1);
		fs->Skip(2);
	}
}

inline void ReadLine32(RGBA *dst, FileStream *fs, int length)
{
	for(int i = 0; i < length; i++) {
		fs->Read(&dst->b, 1);
		fs->Read(&dst->g, 1);
		fs->Read(&dst->r, 1);
		fs->Read(&dst->a, 1);
		dst++;
	}
}

inline void ReadLine32(BGRA *dst, FileStream *fs, int length)
{
	fs->Read(dst, length * sizeof(BGRA));
}

inline void ReadLine32(RGB *dst, FileStream *fs, int length)
{
	for(int i = 0; i < length; i++) {
		fs->Read(&dst->b, 1);
		fs->Read(&dst->g, 1);
		fs->Read(&dst->r, 1);
		fs->Skip(1);
		dst++;
	}
}

inline void ReadLine32(BGR *dst, FileStream *fs, int length)
{
	for(int i = 0; i < length; i++) {
		fs->Read(dst, sizeof(BGR));
		fs->Skip(1);
		dst++;
	}
}


inline void WriteLine24(const BGR *src, FILE *fp, int length)
{
	fwrite(src, sizeof(BGR), length, fp);
}

inline void WriteLine24(const RGB *src, FILE *fp, int length)
{
	for(int i = 0; i < length; i++) {
		unsigned char buf[3] = {
			src->b,
			src->g,
			src->r
		};
		fwrite(buf, 3, 1, fp);
		src++;
	}
}

inline void WriteLine24(const BGRA *src, FILE *fp, int length)
{
	for(int i = 0; i < length; i++) {
		fwrite(src++, 3, 1, fp);
	}
}

inline void WriteLine24(const RGBA *src, FILE *fp, int length)
{
	for(int i = 0; i < length; i++) {
		unsigned char buf[3] = {
			src->b,
			src->g,
			src->r
		};
		fwrite(buf, 3, 1, fp);
		src++;
	}
}

inline void WriteLine24(const unsigned char *src, FILE *fp, int length)
{
	for(int i = 0; i < length; i++) {
		fwrite(src, 1, 1, fp);
		fwrite(src, 1, 1, fp);
		fwrite(src, 1, 1, fp);
		src++;
	}
}

void LoadPalette(FileStream *fs, BGRA palette[], int n)
{
	fs->Read(palette, sizeof(BGRA) * n);
}

/*
 * load bitmap file
 * fileName : input file name
 * dst      : output image
 * return   : result code
 */
template <typename T>
int LoadBitmap(const char *fileName, Image<T> *dst)
{
	int i, linemod;
	BitmapFileHeader head;
	BitmapInfoHeader info;
	FileStream fs;
	
	if(!fs.Open(fileName)) {
		printf("can not open %s!\n", fileName);
		return 0;
	}
	
	// read header
	fs.Read(&head, sizeof(BitmapFileHeader));
	fs.Read(&info, sizeof(BitmapInfoHeader));
	
	
	// undefine type
	if(head.bfType != ('B' | 'M' << 8)) {
		printf("no bitmap format [%s]!\n", fileName);
		fs.Close();
		return 0;
	}
	
	dst->Create(info.biWidth, info.biHeight);
	
	// load palette
	int paletteCount;
	BGRA palette[256];
	if(info.biBitCount & (8 | 4 | 1)) {
		// biClrUsedに0が入っている可能性があるのでbfOffBitsからパレット数を判断する
		paletteCount = (head.bfOffBits - 0x36) >> 2;
		LoadPalette(&fs, palette, paletteCount);
	}
	
	if(info.biBitCount & ~(24)) {
		printf("biBitCount %d.\n", info.biBitCount);
		printf("%s Load failed!\n", fileName);
		fs.Close();
		return 0;
	}
	
	linemod = ((info.biWidth * info.biBitCount) >> 3) & 3;
	T **lines = &dst->lines[info.biHeight - 1];
	for(i = 0; i < info.biHeight; i++) {
		switch(info.biBitCount) {
		case 8:
			ReadLine8(*lines, &fs, info.biWidth, palette);
			break;
		case 24:
			ReadLine24(*lines, &fs, info.biWidth);
			break;
		case 32:
			ReadLine32(*lines, &fs, info.biWidth);
			break;
		}
		lines--;
		if(linemod != 0) fs.Skip(4 - linemod);
	}
	
	fs.Close();
	//printf("load finish\n");
	return 1;
}

/*
 * Save Bitmap file by 24 bit
 */
template <typename T>
int SaveBitmap(const char *fileName, const Image<T> *src)
{
	int i, linemod, linebyte;
	BitmapFileHeader head;
	BitmapInfoHeader info;
	FILE *fp;
	
	if(src == NULL || src->lines == NULL) {
		printf("%s save failed!\n", fileName);
		return 0;
	}
	
	linemod = src->width * 3 % 4;
	
	if(linemod == 0)
		linebyte = src->width * 3;
	else
		linebyte = src->width * 3 + (4 - linemod);
	
	// write bitmap file header
	head.bfType = 'B' | 'M' << 8;
	head.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + linebyte * src->height;
	head.bfReserved1 = 0;
	head.bfReserved2 = 0;
	head.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
	
	// write bitmap info header
	info.biSize = sizeof(BitmapInfoHeader);
	info.biWidth = src->width;
	info.biHeight = src->height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = linebyte * src->height;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant= 0;
	
	fopen_s(&fp, fileName, "wb");
	
	if(fileName == NULL || fp == NULL) return 0;
	
	// write header
	fwrite(&head, sizeof(BitmapFileHeader), 1, fp);
	fwrite(&info, sizeof(BitmapInfoHeader), 1, fp);
	
	const int padding = 0;
	
	// write data
	for(i = 0; i < src->height; i++) {
		WriteLine24(src->lines[info.biHeight - i - 1], fp, info.biWidth);
		// 4byteの境界にあわせて0を書き込む
		if(linemod != 0) fwrite(&padding, 4 - linemod, 1, fp);
	}
	
	fclose(fp);
	return 1;
}
#endif //_BMP_H
