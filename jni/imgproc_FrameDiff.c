#include "imgproc_FrameDiff.h"
#include "utils.h"
#include <malloc.h>

static jint sThreshold = 25;

JNIEXPORT void JNICALL Java_imgproc_FrameDiff_frameDiff
	(JNIEnv *pEnv, jclass pObj, jbyteArray pFrame1, jbyteArray pFrame2, jint pWidth, jint pHeight)
{
	jint lI, lJ, lIdx;
	jint lLenght = pWidth * pHeight;

	jbyte* lOldFrame = (*pEnv)->GetByteArrayElements(pEnv, pFrame1, 0);
	jbyte* lNewFrame = (*pEnv)->GetByteArrayElements(pEnv, pFrame2, 0);

	jbyte* lDiffFrame = (jbyte*) malloc(sizeof(jbyte)*lLenght);

	for ( lJ = 0 , lIdx = 0 ; lJ < pHeight ; lJ++ )
	{
		for ( lI = 0 ; lI < pWidth ; lI++ , lIdx++ )
		{
			if ( abs( lOldFrame[lIdx] - lNewFrame[lIdx] ) > sThreshold )
			{
				lDiffFrame[lIdx] = lNewFrame[lIdx];
			}

			else
			{
				lDiffFrame[lIdx] = 0;
			}
		}
	}

	(*pEnv)->SetByteArrayRegion(pEnv, pFrame1, 0, lLenght + ( lLenght >> 1 ), lNewFrame);
	(*pEnv)->SetByteArrayRegion(pEnv, pFrame2, 0, lLenght, lDiffFrame);
	(*pEnv)->ReleaseByteArrayElements(pEnv, pFrame1, lOldFrame, 0);
	(*pEnv)->ReleaseByteArrayElements(pEnv, pFrame2, lNewFrame, 0);
}
