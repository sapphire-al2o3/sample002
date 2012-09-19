#include "color.h"
#include "image.h"

namespace IP {

template <typename T>
inline bool Range(T x, T max, T min)
{
	return max >= x && x >= min;
}

template <typename T>
void Convert(const Image<T> *src, Image<HSV> *dst)
{
	int w = src->width;
	int h = src->height;
	dst->Create(w, h);
	const T *ps = src->lines[0];
	HSV *pd = dst->lines[0];
	for(int i = 0; i < w * h; i++) {
		RGBToHSV(*ps++, *pd++);
	}
}

/*
 * Extract region by RGB color space
 */
template <typename T>
void ExtractRGB(const Image<T> *src, Image<unsigned char> *dst, T max, T min)
{
	int w = src->width;
	int h = src->height;
	dst->Create(w, h);
	const T *ps = src->lines[0];
	unsigned char *pd = dst->lines[0];
	for(int i = 0; i < w * h; i++) {
		//if(Range(ps->r, max.r, min.r) && Range(ps->g, max.g, min.g) && Range(ps->b, max.b, min.b))
		if(max.r >= ps->r && ps->r >= min.r && max.g >= ps->g && ps->g >= min.g && max.b >= ps->b && ps->b >= min.b) {
			*pd = 255;
		} else {
			*pd = 0;
		}
		ps++;
		pd++;
	}
}

void ExtractYCrCb(const Image<YCrCb> *src, Image<unsigned char> *dst, const YCrCb &max, const YCrCb &min)
{
	int w = src->width;
	int h = src->height;
	dst->Create(w, h);
	const YCrCb *ps = src->lines[0];
	unsigned char *pd = dst->lines[0];
	for(int i = 0; i < w * h; i++) {
		if(max.y >= ps->y && ps->y >= min.y) {
			*pd = 255;
		} else {
			*pd = 0;
		}
		ps++;
		pd++;
	}
}

/*
 * Extract region by HSV color space
 */
void ExtractHSV(const Image<HSV> *src, Image<unsigned char> *dst, const HSV &max, const HSV &min)
{
	dst->Create(src->width, src->height);
	int w = src->width;
	int h = src->height;
	const HSV *ps = src->lines[0];
	unsigned char *pd = dst->lines[0];
	for(int i = 0; i < w * h; i++) {
		if(max.h >= ps->h && ps->h >= min.h &&
			max.s >= ps->s && ps->s >= min.s &&
			max.v >= ps->v && ps->v >= min.v) {
			*pd = 255;
		} else {
			*pd = 0;
		}
		ps++;
		pd++;
	}
}

} // namespace IP
