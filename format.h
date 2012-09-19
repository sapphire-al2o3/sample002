#ifndef _FORMAT_H
#define _FORMAT_H

//#include <limits>

//struct RGB;
struct BGR;
struct RGBA;
struct BGRA;
struct BGRA16;

template <typename T>
struct _RGB {
	typedef _RGB<T> self;
	typedef T type;
	type r, g, b;
	
	// value max, min
	// numeric_limits<T>::max()
	const static type max = 255;
	const static type min = 0;
	// component count
	const static int count = 3;
	
	/*
	_RGB(const T &_r, const T &_g, const T &_b) {
		r = _r;
		g = _g;
		b = _b;
	}
	*/
	
	static self Max() {
		self rgb = { max, max, max };
		return rgb;
	}
	static self Min() {
		self rgb = { min, min, min };
		return rgb;
	}
	static self Zero() {
		self rgb = { 0, 0, 0 };
		return rgb;
	}
	
	self operator +(const self &rhs) {
		self rgb = { r + rhs.r, g + rhs.g, b + rhs.b };
		return rgb;
	}
	
	self &operator +=(const self &rhs) {
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		return *this;
	}
	
	self operator -(const self &rhs) {
		self rgb = { r - rhs.r, g - rhs.g, b - rhs.b };
		return rgb;
	}
	
	self &operator-=(const self &rhs) {
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		return *this;
	}
	
	self operator*(const self &rhs) {
		//self rgb = { r * rhs.r, g * rhs.g, b * rhs.b };
		self rgb = {
			r * rhs.r > max ? max : r * rhs.r,
			g * rhs.g > max ? max : g * rhs.g,
			b * rhs.b > max ? max : b * rhs.b
		};
		return rgb;
	}
	
	template <typename U>
	self operator*(const U &rhs) {
		_RGB<U> rgb = {
			r * rhs, g * rhs, b * rhs
		};
		return rgb;
	}

	self operator /(type rhs) {
		self rgb = { r / rhs, g / rhs, b / rhs };
		return rgb;
	}

	self operator /(const self &rhs) {
		self rgb = { r / rhs.r, g / rhs.g, b / rhs.b };
		return rgb;
	}

	self operator/=(const self &rhs) {
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		return *this;
	}

	bool operator>(const self &rgb) const {
		return (r + g + b) > (rgb.r + rgb.g + rgb.b);
	}
	bool operator>(type y) const {
		return (r + g + b) > y * 3;
	}
	bool operator==(const self &rgb) const {
		return r == rgb.r && g == rgb.g && b == rgb.b;
	}
	bool operator!=(const self &rhs) const {
		return !(*this == rhs);
	}
	friend bool operator==(type y, const self &rgb) {
		return y == rgb.r && y == rgb.g && y == rgb.b;
	}

	self &operator=(type rhs) {
		r = rhs;
		g = rhs;
		b = rhs;
		return *this;
	}
	
	self &operator=(const BGRA16 &rhs);
	
	//operator BGR() {
	//	BGR bgr = { r, g, b };
	//	return bgr;
	//}
};

typedef _RGB<unsigned char> RGB;
typedef _RGB<unsigned char> RGB8;

struct BGR {
	unsigned char b, g, r;

	bool operator==(const BGR &rhs) const {
		return r == rhs.r && g == rhs.g && b == rhs.b;
	}

	bool operator!=(const BGR &rhs) const {
		return r != rhs.r || g != rhs.g || b != rhs.b;
	}

	//BGR &operator=(const RGBA &rhs) {
	//	r = rhs.r;
	//	g = rhs.g;
	//	b = rhs.b;
	//}

	BGR &operator=(unsigned char rhs) {
		r = g = b = rhs;
		return *this;
	}
	
	BGR &operator=(const BGRA &rhs);
	BGR &operator=(const BGRA16 &rhs);
};

struct RGBA {
	unsigned char r, g, b, a;
};

struct BGRA {
	typedef BGRA self_type;
	unsigned char b, g, r, a;
	static const int count = 4;
	
	bool operator==(const self_type &rhs) const {
		return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
	}

	bool operator!=(const self_type &rhs) const {
		return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a;
	}
	
	BGRA &operator=(unsigned char rhs) {
		r = g = b = rhs;
		a = 255;
		return *this;
	}
	BGRA &operator=(const BGR &rhs) {
		b = rhs.b;
		g = rhs.g;
		r = rhs.r;
		a = 255;
		return *this;
	}
	BGRA &operator=(const BGRA16 &rhs);
	
	operator unsigned char() {
		return r;
	}
};

struct BGRA16 {
	unsigned short b, g, r, a: 4;
	BGRA16 &operator=(const BGR &rhs) {
		b = rhs.b;
		g = rhs.g;
		r = rhs.r;
	}
};

template <typename T>
struct Color {
	typename T::type e[T::value_num];

};


inline RGB &RGB::operator=(const BGRA16 &rhs) {
	r = static_cast<RGB::type>(rhs.r * 8);
	g = static_cast<RGB::type>(rhs.g * 8);
	b = static_cast<RGB::type>(rhs.b * 8);
	return *this;
}

inline BGR &BGR::operator=(const BGRA &rhs) {
	b = rhs.b;
	g = rhs.g;
	r = rhs.r;
	return *this;
}

inline BGR &BGR::operator=(const BGRA16 &rhs) {
	r = static_cast<RGB::type>(rhs.r * 8);
	g = static_cast<RGB::type>(rhs.g * 8);
	b = static_cast<RGB::type>(rhs.b * 8);
	return *this;
}

inline BGRA &BGRA::operator=(const BGRA16 &rhs) {
	b = static_cast<RGB::type>(rhs.b * 8);
	g = static_cast<RGB::type>(rhs.g * 8);
	r = static_cast<RGB::type>(rhs.r * 8);
	a = static_cast<RGB::type>(rhs.a * 8);
	return *this;
}

#endif _FORMAT_H