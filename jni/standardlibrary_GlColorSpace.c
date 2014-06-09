#include "standardlibrary_GlColorSpace.h"
#include "utils.h"
#include <android/bitmap.h>
#include <stdio.h>

inline jint max(int32_t pValue1, int32_t pValue2)
{
	return ( pValue1 < pValue2 ) ? pValue2 : pValue1;
}

inline jint min(int32_t pValue1, int32_t pValue2)
{
	return ( pValue1 > pValue2 ) ? pValue2 : pValue1;
}

inline jint clamp(jint pValue, jint pLowest, jint pHighest)
{
	if (pValue < pLowest)
	{
		return pLowest;
	}

	else if (pValue > pHighest)
	{
		return pHighest;
	}

	else
	{
		return pValue;
	}
}

inline jfloat clampf(jfloat pValue, jfloat pLowest, jfloat pHighest)
{
	if (pValue < pLowest)
	{
		return pLowest;
	}

	else if (pValue > pHighest)
	{
		return pHighest;
	}

	else
	{
		return pValue;
	}
}

inline int32_t color(jint pColorR, jint pColorG, jint pColorB)
{
	return   0xFF000000 | ((pColorB << 6 ) & 0x00FF0000)
			  	  	    | ((pColorG >> 2 ) & 0x0000FF00)
			  	  	    | ((pColorR >> 10) & 0x000000FF);
}

void yCbCrToArgb8888(int32_t* pBmpOut, jint pBmpWidth, jint pBmpHeight, jbyte* pIn, jint pWidth, jint pHeight)
{
	jint lI, lJ;
	jint lR, lG, lB;
	jint lY, lCb, lCr;
	jint lYIdx, lCrCbIdx, lBmpIdx;
	jint lFrameSize = pWidth * pHeight;
	jint lStride = pBmpWidth - pWidth;
	jint lOffSet = ( ( pBmpHeight - pHeight) >> 1 ) * pBmpWidth + ( ( pBmpWidth - pWidth ) >> 1 );

	for  ( lJ = 0, lYIdx = 0, lBmpIdx = lOffSet ; lJ < pHeight ; lJ++ )
	{
		lCrCbIdx = lFrameSize + ( lJ >> 1 ) * pWidth;

		for ( lI = 0 ; lI < pWidth ; lI++ )
		{
			lY = max(( pIn[lYIdx++] & 0xff ) - 16 , 0);

			if ( !( lI % 2 ) )
			{
				lCr = ( pIn[lCrCbIdx++] & 0xff ) - 128;
				lCb = ( pIn[lCrCbIdx++] & 0xff ) - 128;
			}

			lY = 1192 * lY;

			lR = lY + 1634 * lCr;
			lG = lY -  833 * lCr - 400 * lCb;
			lB = lY + 2066 * lCb;

			lR = clamp(lR, 0, 262143);
			lG = clamp(lG, 0, 262143);
			lB = clamp(lB, 0, 262143);

			pBmpOut[lBmpIdx++] = color(lR,lG,lB);
		}

		lBmpIdx += lStride;
	}
}

void yCbCrToRgb565(int16_t* pBmpOut, jint pBmpWidth, jint pBmpHeight, jbyte* pIn, jint pWidth, jint pHeight)
{
	jint lI, lJ;
	jint lR, lG, lB;
	jint lY, lCr, lCb;
	jint lYIdx, lCrCbIdx, lBmpIdx;
	jint lFrameSize = pWidth * pHeight;
	jint lStride = pBmpWidth - pWidth;
	jint lOffSet = (( pBmpHeight - pHeight) >> 1 ) * pBmpWidth + (( pBmpWidth - pWidth ) >> 1 );

	for  ( lJ = 0, lYIdx = 0, lBmpIdx = lOffSet ; lJ < pHeight ; lJ++ )
	{
		lCrCbIdx = lFrameSize + ( lJ >> 1 ) * pWidth;

		for ( lI = 0 ; lI < pWidth ; lI++ )
		{
			lY = max(( pIn[lYIdx++] & 0xff ) - 16 , 0);

			if ( !( lI % 2 ) )
			{
				lCr = ( pIn[lCrCbIdx++] & 0xff ) - 128;
				lCb = ( pIn[lCrCbIdx++] & 0xff ) - 128;
			}

			lR = lY + (( 359 * lCr ) >> 8 );
			lG = lY - ((  88 * lCb + 183 * lCr ) >> 8 );
			lB = lY + (( 454 * lCb ) >> 8 );

			lR = clamp(lR, 0, 255);
			lG = clamp(lG, 0, 255);
			lB = clamp(lB, 0, 255);

			lR >>= 3;
			lG >>= 2;
			lB >>= 3;

			pBmpOut[lBmpIdx++] = ( 0x0000 ) | ( lR << 11 ) | ( lG << 5 ) | ( lB << 0 );
		}

		lBmpIdx += lStride;
	}
}

void yCbCrToGray(int8_t* pBmpOut, jint pBmpWidth, jint pBmpHeight, jbyte* pFrame, jint pWidth, jint pHeight)
{
	jint lI, lJ;
	jint lIdx, lBmpIdx;
	jint lStride = pBmpWidth - pWidth;
	jint lOffSet = (( pBmpHeight - pHeight) >> 1 ) * pBmpWidth + (( pBmpWidth - pWidth ) >> 1 );

	for  ( lJ = 0 , lIdx = 0 , lBmpIdx = lOffSet ; lJ < pHeight ; lJ++ )
	{
		for ( lI = 0 ; lI < pWidth ; lI++ )
		{
			pBmpOut[lBmpIdx++] = pFrame[lIdx++];
		}

		lBmpIdx += lStride;
	}
}

JNIEXPORT void JNICALL Java_standardlibrary_GlColorSpace_toRGB
	(JNIEnv *pEnv, jclass pClass, jobject pTarget, jbyteArray pSource, jint pWidth, jint pHeight)
{
	AndroidBitmapInfo lBitmapInfo;

	if ( AndroidBitmap_getInfo(pEnv, pTarget, &lBitmapInfo) < 0 )
	{
		return;
	}

	jbyte* lSource = (*pEnv)->GetByteArrayElements(pEnv, pSource, 0);

	if ( lSource == NULL )
	{
		return;
	}

	switch ( lBitmapInfo.format )
	{
		case ANDROID_BITMAP_FORMAT_RGBA_8888:
		{
			int32_t* lBitmapContent;

			if ( AndroidBitmap_lockPixels(pEnv, pTarget, (void**)&lBitmapContent) < 0 )
			{
				return;
			}

			yCbCrToArgb8888(lBitmapContent, lBitmapInfo.width, lBitmapInfo.height, lSource, pWidth, pHeight);
			break;
		}

		case ANDROID_BITMAP_FORMAT_RGB_565:
		{
			int16_t* lBitmapContent;

			if ( AndroidBitmap_lockPixels(pEnv, pTarget, (void**)&lBitmapContent) < 0 )
			{
				return;
			}

			yCbCrToRgb565(lBitmapContent, lBitmapInfo.width, lBitmapInfo.height, lSource, pWidth, pHeight);
			break;
		}

		case ANDROID_BITMAP_FORMAT_A_8:
		{
			int8_t* lBitmapContent;

			if ( AndroidBitmap_lockPixels(pEnv, pTarget, (void**)&lBitmapContent) < 0 )
			{
				return;
			}

			yCbCrToGray(lBitmapContent, lBitmapInfo.width, lBitmapInfo.height, lSource, pWidth, pHeight);
			break;
		}
	}

	(*pEnv)->ReleaseByteArrayElements(pEnv, pSource, lSource, 0);

	AndroidBitmap_unlockPixels(pEnv, pTarget);
}
