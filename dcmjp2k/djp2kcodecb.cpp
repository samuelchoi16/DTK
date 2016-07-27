#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcerror.h"
#include "dcmjp2k/djp2kcodecb.h"
#include "dcmjp2k/djp2kinternal.h"

CodecBase::CodecBase()
	: DcmCodec()
{
}

CodecBase::~CodecBase()
{
}

OFCondition CodecBase::convertToPlanarConfiguration1(
	Uint8 *target,
	const Uint8 *source,
	Uint16 components,
	Uint32 width,
	Uint32 height,
	Uint16 bitsAllocated) const
{
	Uint8 bytesAllocated = bitsAllocated / 8;
	Uint32 planeSize = width * height * bytesAllocated;

	if (bitsAllocated % 8 != 0)
		return EC_IllegalCall;

	for (Uint32 pos = 0; pos < width * height; pos++) {
		for (int i = 0; i < components; i++) {
			memcpy(&target[i * planeSize + pos * bytesAllocated], source, bytesAllocated);
			source += bytesAllocated;
		}
	}
	return EC_Normal;
}

OFCondition CodecBase::convertToPlanarConfiguration0(
	Uint8 *target,
	const Uint8 *source,
	Uint16 components,
	Uint32 width,
	Uint32 height,
	Uint16 bitsAllocated) const
{
	Uint8 bytesAllocated = bitsAllocated / 8;
	Uint32 planeSize = width * height * bytesAllocated;

	if (bitsAllocated % 8 != 0)
		return EC_IllegalCall;

	for (Uint32 pos = 0; pos < width * height; pos++) {
		for (int i = 0; i < components; i++) {
			memcpy(target, &source[i * planeSize + pos * bytesAllocated], bytesAllocated);
			target += bytesAllocated;
		}
	}
	return EC_Normal;
}

OFCondition CodecBase::convertToPlanarConfiguration1Byte(
	Uint8 *imageFrame,
	Uint16 columns,
	Uint16 rows)
{
	if (imageFrame == NULL) return EC_IllegalCall;

	unsigned long numPixels = columns * rows;
	if (numPixels == 0) return EC_IllegalCall;

	Uint8 *buf = new Uint8[3 * numPixels + 3];
	if (buf)
	{
		memcpy(buf, imageFrame, (size_t)(3 * numPixels));
		Uint8 *s = buf;                        // source
		Uint8 *r = imageFrame;                 // red plane
		Uint8 *g = imageFrame + numPixels;     // green plane
		Uint8 *b = imageFrame + (2 * numPixels); // blue plane
		for (unsigned long i = numPixels; i; i--)
		{
			*r++ = *s++;
			*g++ = *s++;
			*b++ = *s++;
		}
		delete[] buf;
	}
	else return EC_MemoryExhausted;
	return EC_Normal;
}

OFCondition CodecBase::convertToPlanarConfiguration1Word(
	Uint16 *imageFrame,
	Uint16 columns,
	Uint16 rows)
{
	if (imageFrame == NULL) return EC_IllegalCall;

	unsigned long numPixels = columns * rows;
	if (numPixels == 0) return EC_IllegalCall;

	Uint16 *buf = new Uint16[3 * numPixels + 3];
	if (buf)
	{
		memcpy(buf, imageFrame, (size_t)(3 * numPixels*sizeof(Uint16)));
		Uint16 *s = buf;                        // source
		Uint16 *r = imageFrame;                 // red plane
		Uint16 *g = imageFrame + numPixels;     // green plane
		Uint16 *b = imageFrame + (2 * numPixels); // blue plane
		for (unsigned long i = numPixels; i; i--)
		{
			*r++ = *s++;
			*g++ = *s++;
			*b++ = *s++;
		}
		delete[] buf;
	}
	else return EC_MemoryExhausted;
	return EC_Normal;
}

OFCondition CodecBase::convertToPlanarConfiguration0Byte(
	Uint8 *imageFrame,
	Uint16 columns,
	Uint16 rows)
{
	if (imageFrame == NULL) return EC_IllegalCall;

	unsigned long numPixels = columns * rows;
	if (numPixels == 0) return EC_IllegalCall;

	Uint8 *buf = new Uint8[3 * numPixels + 3];
	if (buf)
	{
		memcpy(buf, imageFrame, (size_t)(3 * numPixels));
		Uint8 *t = imageFrame;          // target
		Uint8 *r = buf;                 // red plane
		Uint8 *g = buf + numPixels;     // green plane
		Uint8 *b = buf + (2 * numPixels); // blue plane
		for (unsigned long i = numPixels; i; i--)
		{
			*t++ = *r++;
			*t++ = *g++;
			*t++ = *b++;
		}
		delete[] buf;
	}
	else return EC_MemoryExhausted;
	return EC_Normal;
}

OFCondition CodecBase::convertToPlanarConfiguration0Word(
	Uint16 *imageFrame,
	Uint16 columns,
	Uint16 rows)
{
	if (imageFrame == NULL) return EC_IllegalCall;

	unsigned long numPixels = columns * rows;
	if (numPixels == 0) return EC_IllegalCall;

	Uint16 *buf = new Uint16[3 * numPixels + 3];
	if (buf)
	{
		memcpy(buf, imageFrame, (size_t)(3 * numPixels*sizeof(Uint16)));
		Uint16 *t = imageFrame;          // target
		Uint16 *r = buf;                 // red plane
		Uint16 *g = buf + numPixels;     // green plane
		Uint16 *b = buf + (2 * numPixels); // blue plane
		for (unsigned long i = numPixels; i; i--)
		{
			*t++ = *r++;
			*t++ = *g++;
			*t++ = *b++;
		}
		delete[] buf;
	}
	else return EC_MemoryExhausted;
	return EC_Normal;
}
