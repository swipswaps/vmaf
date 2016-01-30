#include "alignment.h"
#include "convolution.h"
#include "convolution_internal.h"

void convolution_x_c_s(const float *filter, int filter_width, const float *src, float *dst, int width, int height, int src_stride, int dst_stride, int step)
{
	int radius = filter_width / 2;
	int borders_left = vmaf_ceiln(radius, step);
	int borders_right = vmaf_floorn(width - (filter_width - radius), step);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < borders_left; j += step) {
			dst[i * dst_stride + j / step] = convolution_edge_s(true, filter, filter_width, src, width, height, src_stride, i, j);
		}

		for (int j = borders_left; j < borders_right; j += step) {
			float accum = 0;
			for (int k = 0; k < filter_width; ++k) {
				accum += filter[k] * src[i * src_stride + j - radius + k];
			}
			dst[i * dst_stride + j / step] = accum;
		}

		for (int j = borders_right; j < width; j += step) {
			dst[i * dst_stride + j / step] = convolution_edge_s(true, filter, filter_width, src, width, height, src_stride, i, j);
		}
	}
}

void convolution_y_c_s(const float *filter, int filter_width, const float *src, float *dst, int width, int height, int src_stride, int dst_stride, int step)
{
	int radius = filter_width / 2;
	int borders_top = vmaf_ceiln(radius, step);
	int borders_bottom = vmaf_floorn(height - (filter_width - radius), step);

	for (int i = 0; i < borders_top; i += step) {
		for (int j = 0; j < width; ++j) {
			dst[(i / step) * dst_stride + j] = convolution_edge_s(false, filter, filter_width, src, width, height, src_stride, i, j);
		}
	}
	for (int i = borders_top; i < borders_bottom; i += step) {
		for (int j = 0; j < width; ++j) {
			float accum = 0;
			for (int k = 0; k < filter_width; ++k) {
				accum += filter[k] * src[(i - radius + k) * src_stride + j];
			}
			dst[(i / step) * dst_stride + j] = accum;
		}
	}
	for (int i = borders_bottom; i < height; i += step) {
		for (int j = 0; j < width; ++j) {
			dst[(i / step) * dst_stride + j] = convolution_edge_s(false, filter, filter_width, src, width, height, src_stride, i, j);
		}
	}
}

void convolution_f32_c_s(const float *filter, int filter_width, const float *src, float *dst, float *tmp, int width, int height, int src_stride, int dst_stride)
{
	// convolve along y first then x
	convolution_y_c_s(filter, filter_width, src, tmp, width, height, src_stride, dst_stride, 1);
	convolution_x_c_s(filter, filter_width, tmp, dst, width, height, src_stride, dst_stride, 1);
}

void convolution_x_c_d(const double *filter, int filter_width, const double *src, double *dst, int width, int height, int src_stride, int dst_stride, int step)
{
	int radius = filter_width / 2;
	int borders_left = vmaf_ceiln(radius, step);
	int borders_right = vmaf_floorn(width - (filter_width - radius), step);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < borders_left; j += step) {
			dst[i * dst_stride + j / step] = convolution_edge_d(true, filter, filter_width, src, width, height, src_stride, i, j);
		}

		for (int j = borders_left; j < borders_right; j += step) {
			double accum = 0;
			for (int k = 0; k < filter_width; ++k) {
				accum += filter[k] * src[i * src_stride + j - radius + k];
			}
			dst[i * dst_stride + j / step] = accum;
		}

		for (int j = borders_right; j < width; j += step) {
			dst[i * dst_stride + j / step] = convolution_edge_d(true, filter, filter_width, src, width, height, src_stride, i, j);
		}
	}
}

void convolution_y_c_d(const double *filter, int filter_width, const double *src, double *dst, int width, int height, int src_stride, int dst_stride, int step)
{
	int radius = filter_width / 2;
	int borders_top = vmaf_ceiln(radius, step);
	int borders_bottom = vmaf_floorn(height - (filter_width - radius), step);

	for (int i = 0; i < borders_top; i += step) {
		for (int j = 0; j < width; ++j) {
			dst[(i / step) * dst_stride + j] = convolution_edge_d(false, filter, filter_width, src, width, height, src_stride, i, j);
		}
	}
	for (int i = borders_top; i < borders_bottom; i += step) {
		for (int j = 0; j < width; ++j) {
			double accum = 0;
			for (int k = 0; k < filter_width; ++k) {
				accum += filter[k] * src[(i - radius + k) * src_stride + j];
			}
			dst[(i / step) * dst_stride + j] = accum;
		}
	}
	for (int i = borders_bottom; i < height; i += step) {
		for (int j = 0; j < width; ++j) {
			dst[(i / step) * dst_stride + j] = convolution_edge_d(false, filter, filter_width, src, width, height, src_stride, i, j);
		}
	}
}

void convolution_f32_c_d(const double *filter, int filter_width, const double *src, double *dst, double *tmp, int width, int height, int src_stride, int dst_stride)
{
	// convolve along y first then x
	convolution_y_c_d(filter, filter_width, src, tmp, width, height, src_stride, dst_stride, 1);
	convolution_x_c_d(filter, filter_width, tmp, dst, width, height, src_stride, dst_stride, 1);
}