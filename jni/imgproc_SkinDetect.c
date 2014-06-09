#include "imgproc_SkinDetect.h"
#include "utils.h"
#include <malloc.h>
#include <math.h>

inline int isSkin(jint pCr, jint pCb)
{
	if ( pCr > 133 )
	{
		if ( pCr < 173 )
		{
			if ( pCb > 80 )
			{
				if ( pCb < 120 )
				{
					return 1;
				}

				else
				{
					return 0;
				}
			}

			else
			{
				return 0;
			}
		}

		else
		{
			return 0;
		}
	}

	else
	{
		return 0;
	}
}

JNIEXPORT void JNICALL Java_imgproc_SkinDetect_skinDetector1
	(JNIEnv *pEnv, jclass pObj, jbyteArray pFrame, jint pWidth, jint pHeight)
{
	jint lI, lJ;
	jint lCb, lCr;
	jint lIdx, lCbCrIdx;
	jint lOffSet = pWidth * pHeight;

	jbyte* lInFrame = (*pEnv)->GetByteArrayElements(pEnv, pFrame, 0);

	for ( lJ = 0 , lIdx = 0 ;  lJ < pHeight ; lJ++ )
	{
		lCbCrIdx = lOffSet + ( lJ >> 1 ) * pWidth;

		for ( lI = 0 ; lI < pWidth ; lI++ , lIdx++ )
		{
			if ( !( lI % 2 ) )
			{
				lCr = lInFrame[lCbCrIdx++] & 0xff;
				lCb = lInFrame[lCbCrIdx++] & 0xff;
			}

			if ( !isSkin(lCr, lCb) )
			{
				lInFrame[lIdx] = 0;
			}
		}
	}

	(*pEnv)->ReleaseByteArrayElements(pEnv, pFrame, lInFrame, 0);
}

JNIEXPORT void JNICALL Java_imgproc_SkinDetect_skinDetector2
	(JNIEnv *pEnv, jclass pObj, jbyteArray pFrame, jint pWidth, jint pHeight)
{
	jint lI, lJ;
	jint lR, lG, lB;
	jint lY, lCr, lCb;
	jint lU;
	jint lYIdx, lCrCbIdx;
	jint lOffSet = pWidth * pHeight;

	jfloat lRG;

	jbyte* lInFrame = (*pEnv)->GetByteArrayElements(pEnv, pFrame, 0);

	for  ( lJ = 0, lYIdx = 0 ; lJ < pHeight ; lJ++ )
	{
		lCrCbIdx = lOffSet + ( lJ >> 1 ) * pWidth;

		for ( lI = 0 ; lI < pWidth ; lI++ , lYIdx++ )
		{
			lY = max(( lInFrame[lYIdx] & 0xff ) - 16 , 0);

			if ( !( lI % 2 ) )
			{
				lCr = ( lInFrame[lCrCbIdx++] & 0xff ) - 128;
				lCb = ( lInFrame[lCrCbIdx++] & 0xff ) - 128;
			}

			lR = lY + (( 359 * lCr ) >> 8 );
			lG = lY - ((  88 * lCb + 183 * lCr ) >> 8 );
			//lB = lY + (( 454 * lCb ) >> 8 );

			lR = clamp(lR, 1, 255);
			lG = clamp(lG, 1, 255);
			//lB = clamp(lB, 0, 255);

			lRG = ((jfloat) lR ) / lG;

			lU = ( lR > lG && lRG < 1.5 && lRG > 1.05 ) ? (jint) ( 256.0 / lRG ) : 255;

			lU = clamp(lU, 0 , 255);

			if ( !(( lU >= 90 ) && ( lU <= 249 ) && ( lCr + 128 >= 133 ) && ( lCr + 128 <= 173 )) )
			{
				lInFrame[lYIdx] = 0;
			}
		}
	}

	(*pEnv)->ReleaseByteArrayElements(pEnv, pFrame, lInFrame, 0);
}

