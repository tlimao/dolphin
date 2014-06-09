#include "imgproc_HomomorphicFilter.h"
#include "utils.h"
#include <math.h>
#include <malloc.h>

static int sKl[] = { 1,1,1,
					  1,1,1,
					  1,1,1, };

static int sKh[] = { -1,-1,-1,
					  -1, 8,-1,
					  -1,-1,-1 };

static float sA = 0.9;
static float sB = 1.2;

JNIEXPORT void JNICALL Java_imgproc_HomomorphicFilter_homomorphicFilter
	(JNIEnv *pEnv, jclass pObj, jbyteArray pFrame, jint pWidth, jint pHeight)
{
	jint lI, lJ, lK, lKi, lKj;
	jint lIdx;
	jint lOffSet = 0;
	jint lStride = 1;
	jint lSize = ( 2 * lStride + 1 ) * ( 2 * lStride + 1 );
	jint lSum = 9;
	jint lW = pWidth + 2 * lStride;
	jdouble lMax = 0;
	jdouble lMin = 9999;
	jfloat lMedian[lSize];

	jint lFrameSize = pWidth * pHeight;
	jint lReajustedSize = ( pWidth + 2 * lStride ) * ( pHeight + 2 * lStride );

	jbyte* lInFrame = (*pEnv)->GetByteArrayElements(pEnv, pFrame, 0);

	jdouble* lLogF = (jdouble*) malloc(sizeof(jdouble)*lReajustedSize);
	jdouble* lLogI = (jdouble*) malloc(sizeof(jdouble)*lFrameSize);
	jdouble* lLogR = (jdouble*) malloc(sizeof(jdouble)*lFrameSize);

	for ( lJ = 0 , lIdx = 0 ; lJ < pHeight ; lJ++ )
	{
		lOffSet = lW * ( lStride + lJ ) + lStride;

		for ( lI = 0 ; lI < pWidth ; lI++ , lIdx++ )
		{
			lLogF[lOffSet + lI] = log(1 + ( lInFrame[lIdx] & 0xff ));
		}
	}

	// Passa-Baixa & Passa-Alta
	for ( lJ = 0 , lIdx = 0 ; lJ < pHeight ; lJ++ )
	{
		lOffSet = lW * ( lStride + lJ ) + lStride;

		for ( lI = 0 ; lI < pWidth ; lI++ , lIdx++ )
		{
			for ( lKj = -lStride , lK = 0 ; lKj <= lStride ; lKj++ )
			{
				for ( lKi = -lStride ; lKi <= lStride ; lKi++ , lK++ )
				{
					lLogI[lIdx] += sKl[lK] * lLogF[lOffSet + lI + lKj * lW + lKi];

					lLogR[lIdx] += sKh[lK] * lLogF[lOffSet + lI + lKj * lW + lKi];
				}
			}

			lLogI[lIdx] = lLogI[lIdx] / lSum;

			lLogR[lIdx] = lLogR[lIdx] / lSize;
		}
	}

	// Resaltando Detalhes
	for ( lJ = 0 , lIdx = 0 ; lJ < pHeight ; lJ++ )
	{
		lOffSet = lW * ( lStride + lJ ) + lStride;

		for ( lI = 0 ; lI < pWidth ; lI++ , lIdx++ )
		{
			double val = sA * lLogI[lIdx] + sB * lLogR[lIdx];

			lLogF[lOffSet + lI] = val;

			if ( lMin > val )
			{
				lMin = val;
			}

			if ( lMax < val )
			{
				lMax = val;
			}
		}
	}

	// Normalização e Reconstrução
	for ( lJ = 0 , lIdx = 0 ; lJ < pHeight ; lJ++ )
	{
		lOffSet = lW * ( lStride + lJ ) + lStride;

		for ( lI = 0 ; lI < pWidth ; lI++ , lIdx++ )
		{
			lInFrame[lIdx] = clamp((jint) ( 255 * ( lLogF[lOffSet + lI] - lMin ) / ( lMax - lMin )), 0, 255);
		}
	}

	free(lLogF);
	free(lLogI);
	free(lLogR);

	(*pEnv)->SetByteArrayRegion(pEnv, pFrame, 0, lFrameSize, lInFrame);
	(*pEnv)->ReleaseByteArrayElements(pEnv, pFrame, lInFrame, 0);
}
