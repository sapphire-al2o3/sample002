#ifndef _IMAGE_H
#define _IMAGE_H

#include "format.h"

template<typename T>
class Image {
private:
	Image(const Image &);
	Image &operator=(const Image &);
public:
	T **lines;
	int width;
	int height;
	
	typedef T type;
	static const int depth = sizeof(type) * 8;
	
	static int GetDepth() {
		return sizeof(T);
	}
	
	Image() : lines(0), width(0), height(0) {}
	virtual ~Image() { Release(); }
	
	void Create(int w, int h) {
		if(w != width || h != height) {
			Release();
			lines = new T *[h];
			lines[0] = new T [w * h];
			for(int i = 1; i < h; i++) {
				lines[i] = &lines[0][w * i];
			}
		
			width = w;
			height = h;
		}
	}

	void Create(const Image<T> &image) {
		Create(image.width, image.height);

		for(int i = 0; i < image.width * image.height; i++) {
			lines[0][i] = image.lines[0][i];
		}
	}

	void Release() {
		if(lines) {
			delete [] lines[0];
			delete [] lines;
			lines = 0;
		}
		width = height = 0;
	}

	typedef T* iterator;
	iterator begin() { return *lines; }
	iterator end() { return &lines[0][width * height]; }
	T *operator[](int index) {
		return lines[index];
	}
	T *operator()(int i, int j) {
		return &lines[i][j];
	}
};

typedef Image<RGB> Image24;
typedef Image<RGBA> Image32;
typedef Image<unsigned char> Image8;

#endif // _IMAGE_H
