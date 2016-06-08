#ifndef DJP2KCODECB_H
#define DJP2KCODECB_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dccodec.h"  /* for class DcmCodec */
#include "djp2kdefine.h"

class DCMJP2K_EXPORT DJP2KCodecBase : public DcmCodec
{
protected:
	/// default constructor
	DJP2KCodecBase();

	/// destructor
	virtual ~DJP2KCodecBase();

	/** Convert an image from sample interleaved to uninterleaved.
	 *  @param target A buffer where the converted image will be stored
	 *  @param source The image buffer to be converted
	 *  @param components Color components used in the image
	 *  @param width The width of the image
	 *  @param height The height of the image
	 *  @param bitsAllocated The number of bits allocated in the image.
	 *  @return EC_Normal if succesful, an error code otherwise
	 */
	OFCondition convertToPlanarConfiguration1(
	  Uint8 *target,
	  const Uint8 *source,
	  Uint16 components,
	  Uint32 width,
	  Uint32 height,
	  Uint16 bitsAllocated) const;

	/** Convert an image from uninterleaved to sample interleaved.
	 *  @param target A buffer where the converted image will be stored
	 *  @param source The image buffer to be converted
	 *  @param components Color components used in the image
	 *  @param width The width of the image.
	 *  @param height The height of the image.
	 *  @param bitsAllocated The number of bits allocated in the image.
	 *  @return EC_Normal if succesful, an error code otherwise
	 */
	OFCondition convertToPlanarConfiguration0(
	  Uint8 *target,
	  const Uint8 *source,
	  Uint16 components,
	  Uint32 width,
	  Uint32 height,
	  Uint16 bitsAllocated) const;

	/** converts an RGB or YBR frame with 8 bits/sample from
	 *  color-by-pixel to color-by-plane planar configuration.
	 *  @param imageFrame pointer to image frame, must contain
	 *    at least 3*columns*rows bytes of pixel data.
	 *  @param columns columns
	 *  @param rows rows
	 *  @return EC_Normal if successful, an error code otherwise
	 */
	static OFCondition convertToPlanarConfiguration1Byte(
	  Uint8 *imageFrame,
	  Uint16 columns,
	  Uint16 rows);

	/** converts an RGB or YBR frame with 16 bits/sample from
	 *  color-by-pixel to color-by-plane planar configuration.
	 *  @param imageFrame pointer to image frame, must contain
	 *    at least 3*columns*rows words of pixel data.
	 *  @param columns columns
	 *  @param rows rows
	 *  @return EC_Normal if successful, an error code otherwise
	 */
	static OFCondition convertToPlanarConfiguration1Word(
	  Uint16 *imageFrame,
	  Uint16 columns,
	  Uint16 rows);

	/** converts an RGB or YBR frame with 8 bits/sample from
	 *  color-by-plane to color-by-pixel planar configuration.
	 *  @param imageFrame pointer to image frame, must contain
	 *    at least 3*columns*rows bytes of pixel data.
	 *  @param columns columns
	 *  @param rows rows
	 *  @return EC_Normal if successful, an error code otherwise
	 */
	static OFCondition convertToPlanarConfiguration0Byte(
	  Uint8 *imageFrame,
	  Uint16 columns,
	  Uint16 rows);

	/** converts an RGB or YBR frame with 16 bits/sample from
	 *  color-by-plane to color-by-pixel planar configuration.
	 *  @param imageFrame pointer to image frame, must contain
	 *    at least 3*columns*rows words of pixel data.
	 *  @param columns columns
	 *  @param rows rows
	 *  @return EC_Normal if successful, an error code otherwise
	 */
	static OFCondition convertToPlanarConfiguration0Word(
	  Uint16 *imageFrame,
	  Uint16 columns,
	  Uint16 rows);
};

#endif // DJP2KCODECB_H
