#ifndef _COLOR_H
#define _COLOR_H

/*
color.h

色変換関数などを入れる予定

RGBToYCrCb : RGB -> YCrCb
RGBToHSV   : RGB -> HSV
ExtractHSV : HSVによる領域抽出
RGBToBGR   : RGBの並びをBGRに並び替える

notice
YCrCbの数値を要確認

*/

#include <cmath>
#include "format.h"

const double PI = 3.14159265;

struct YCrCb {
	float y, cr, cb;
};

template <typename T>
struct _HSV {
	T h, s, v;
};

typedef _HSV<float> HSV;


/*
template <typename T>
struct HSV {
	typedef T value_type;
	T h, s, v;
};
*/

/*
 * Linear Polaration
 */
inline RGB Lerp(const RGB &a, const RGB &b, double s)
{
	RGB rgb = {
		a.r + static_cast<RGB::type>(s * (b.r - a.r)),
		a.g + static_cast<RGB::type>(s * (b.g - a.g)),
		a.b + static_cast<RGB::type>(s * (b.b - a.b))
	};
	return rgb;
}

// RGB -> YCrCb
template <typename T, typename U>
inline void RGBToYCrCb(const T &r, const T &g, const T &b, U &y, U &cr, U &cb)
{
	y = 0.299f * r + 0.587f * g + 0.114f * b;
	cr = 0.500f * r - 0.419f * g - 0.081f * b;
	cb = -0.169f * r - 0.332f * g + 0.500f * b;
}

inline void RGBToYCrCb(const RGB *src, YCrCb *dst)
{
	RGBToYCrCb(src->r, src->g, src->b, dst->y, dst->cr, dst->cb);
}

template <typename T, typename U>
inline void YCrCbToRGB(const T &y, const T &cr, const T &cb, U &r, U &g, U &b)
{
	r = static_cast<U>(y + 1.40200f * cr);
	g = static_cast<U>(y - 0.71414f * cr - 0.34414f * cb);
	b = static_cast<U>(y + 1.77200f * cb);
}
template <typename T>
inline void YCrCbToRGB(const YCrCb *src, T *dst)
{
	YCrCbToRGB(src->y, src->cr, src->cb, dst->r, dst->g, dst->b);
}
template <typename T>
inline T YCrCbToRGB(const YCrCb &src)
{
	T dst;
	YCrCbToRGB(src->y, src->cr, src->cb, dst->r, dst->g, dst->b);
	return dst;
}

/*
 * RGB -> HSV
 * h: 色相(0 - 359)
 * s: 彩度(0 - 255)
 * v: 明度(0 - 255)
 */
template <typename T>
int RGBToHSV(unsigned char r, unsigned char g, unsigned char b, T &h, T &s, T &v)
{
	T max, min, tmp;
	
	h = s = v = 0;

	// 最大値・最小値計算
	if(r > g) {
		if(b > r) {
			max = b;
			min = g;
		} else {
			max = r;
			min = (g > b) ? b : g;
		}
	} else {
		if(b > g) {
			max = b;
			min = r;
		} else {
			max = g;
			min = (b > r) ? r : b;
		}
	}
	v = max;
	
	// max = 0 なら黒でありs, h = 0
	if(max == 0)
		return 0;
	tmp = max - min;

	// s算出
	s = tmp * 255 / max;
	
	// max = minの時はh = 0(白か灰色)
	if(tmp == 0)
		return 0;
	
	if(r == max)
		h = 60 * (g - b) / tmp;
	else if(g == max)
		h = 60 * (b - r) / tmp + 120;
	else
		h = 60 * (r - g) / tmp + 240;
	
	if(h < 0)
		h += 360;
	
	return 1;
}

template <typename T>
inline int RGBToHSV(const T &src, HSV &dst)
{
	return RGBToHSV(src.r, src.g, src.b, dst.h, dst.s, dst.v);
}

/*
 * 修正HSV
 * http://webcs.sit.ac.jp/wiki/index.php?%B4%F0%C1%C3%C3%CE%BC%B1
 */
void RGBToHQcV(int r, int g, int b, double &h, double &Qc, double &v)
{
	v = (r + g + b) / 3.0;
	Qc = sqrt((2 * r - g - b) * (2 * r - g - b) / 4.0 + 3.0 * (g - b) * (g - b) / 4.0);
	if(g < b)
		h = 2 * PI - h;
	else
		h = acos((2 * r - g - b) / 2.0 * sqrt((double)((r - g) * (r - g) + (r - b) * (g - b))));
}

/*
 * HSV -> RGB
 */
int HSVToRGB(int h, int s, int v,
	unsigned char &r, unsigned char &g, unsigned char &b)
{
	if(s == 0) {
		r = g = b = v & 255;
		return 0;
	}
	
	int I, F, M, N, K;
	I = h / 60;
	F = h - I * 60;
	M = v * (255 - s) / 255;
	N = v * (255 - s * F / 60) / 255;
	K = v * (255 - s * (60 - F) / 60) / 255;
	switch(I) {
	case 0:
		r = v & 255; g = K & 255; b = M & 255;
		break;
	case 1:
		r = N & 255; g = v & 255; b = M & 255;
		break;
	case 2:
		r = M & 255; g = v & 255; b = K & 255;
		break;
	case 3:
		r = M & 255; g = N & 255; b = v & 255;
		break;
	case 4:
		r = K & 255; g = M & 255; b = v & 255;
		break;
	case 5:
		r = v & 255; g = M & 255; b = N & 255;
		break;
	}
	return 1;
}

/*
 * HSV -> RGB 簡易版
 */
int HSVToRGB(int h, int s, int v)
{
	unsigned char r, g, b;
	HSVToRGB(h, s, v, r, g, b);
	return (b & 0xFF << 16)| (g & 0xFF << 8) | (r & 0xFF);
}


template <typename T, typename U>
void RGBToXYZ(const T &r, const T &g, const T &b, U &x, U &y, U &z)
{
	x = 0.412453 * r + 0.357580 * g + 0.180423 * b;
	y = 0.212671 * r + 0.715160 * g + 0.072169 * b;
	z = 0.019334 * r + 0.119193 * g + 0.950227 * b;
}

template <typename T, typename U>
void RGBToXYZ(const T &rgb, U &xyz)
{
	RGBToXYZ(rgb.r, rgb.g, rgb.b, xyz.x, xyz.y, xyz.z);
}

template <typename T, typename U>
void XYZToRGB(const T &x, const T &y, const T &z, U &r, U &g, U &b)
{
	r = 3.240479 * x - 1.53715 * y - 0.498535 * z;
	g = -0.969256 * x + 1.875991 * y + 0.041556 * z;
	b = 0.055648 * x - 0.204043 * y + 1.057311 * z;
}

template <typename T, typename U>
void XYZToRGB(const T &xyz, T &rgb)
{
	XYZToRGB(xyz.x, xyz.y, xyz.z, rgb.r, rgb.g, rgb.b);
}

template <typename T, typename U>
void XYZToLab()
{
	const t = (6.0 / 29.0) * (6.0 / 29.0) * (6.0 / 29.0);
	
	if(y > t) {
		l = 116 * pow(y, 1.0 / 3.0) - 16;
	} else {
		l = 903.3 * y;
	}
	double xx, yy, zz;
	if(x > t) {
		xx = pow(x, 1.0 / 3.0);
	} else {
		xx = (29.0 / 6.0) * (29.0 / 6.0) / 3.0 * x + 4.0 / 29.0;
	}
}

#endif // _COLOR_H
