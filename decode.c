#include <stdint.h>
#include "dsptool.h"
#include <limits.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static int DivideByRoundUp(int dividend, int divisor){
	return (dividend + divisor - 1) / divisor;
}

static char GetHighNibble(char value){
	return value >> 4 & 0xF;
}

static char GetLowNibble(char value){
	return value & 0xF;
}

static short Clamp16(int value){
	if (value > SHRT_MAX)
		return SHRT_MAX;
	if (value < SHRT_MIN)
		return SHRT_MIN;
	return value;
}

void decode(const uint8_t* src, int16_t* dst, ADPCMINFO* cxt, uint32_t samples){
	short hist1 = cxt->yn1;
	short hist2 = cxt->yn2;
	const short* coefs = cxt->coef;
	const int frameCount = DivideByRoundUp(samples, SAMPLES_PER_FRAME);
	int samplesRemaining = samples;

	for (int i = 0; i < frameCount; i++) {
		const int predictor = (uint8_t)GetHighNibble(*src);
		const int scale = 1 << GetLowNibble(*src++);
		const short coef1 = coefs[predictor * 2];
		const short coef2 = coefs[predictor * 2 + 1];

		const int samplesToRead = MIN(SAMPLES_PER_FRAME, samplesRemaining);

		for (int s = 0; s < samplesToRead; s++) {
			int sample = s % 2 == 0 ? GetHighNibble(*src) : GetLowNibble(*src++);
			sample = sample >= 8 ? sample - 16 : sample;
			sample = (((scale * sample) << 11) + 1024 + (coef1 * hist1 + coef2 * hist2)) >> 11;
			const short finalSample = Clamp16(sample);

			hist2 = hist1;
			hist1 = finalSample;

			*dst++ = finalSample;
		}

		samplesRemaining -= samplesToRead;
	}
}

void getLoopContext(const uint8_t* src, ADPCMINFO* cxt, uint32_t samples){
	short hist1 = cxt->yn1;
	short hist2 = cxt->yn2;
	const short* coefs = cxt->coef;
	uint8_t ps = 0;
	const int frameCount = DivideByRoundUp(samples, SAMPLES_PER_FRAME);
	uint32_t samplesRemaining = samples;

	for (int i = 0; i < frameCount; i++) {
		ps = *src;
		const int predictor = (uint8_t)GetHighNibble(*src);
		const int scale = 1 << GetLowNibble(*src++);
		const short coef1 = coefs[predictor * 2];
		const short coef2 = coefs[predictor * 2 + 1];

		const int samplesToRead = MIN(SAMPLES_PER_FRAME, samplesRemaining);

		for (int s = 0; s < samplesToRead; s++) {
			int sample = s % 2 == 0 ? GetHighNibble(*src) : GetLowNibble(*src++);
			sample = sample >= 8 ? sample - 16 : sample;
			sample = (((scale * sample) << 11) + 1024 + (coef1 * hist1 + coef2 * hist2)) >> 11;
			const short finalSample = Clamp16(sample);

			hist2 = hist1;
			hist1 = finalSample;
		}
		samplesRemaining -= samplesToRead;
	}

	cxt->loop_pred_scale = ps;
	cxt->loop_yn1 = hist1;
	cxt->loop_yn2 = hist2;
}
