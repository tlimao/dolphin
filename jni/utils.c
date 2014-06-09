/*
 * utils.c
 *
 *  Created on: 26/08/2013
 *      Author: Thiago
 */
#include "utils.h"

inline jint absDiff(jbyte pValue1, jbyte pValue2)
{
	jint lDiff = pValue1 - pValue2;

	return ( lDiff > 0 ) ? lDiff : -lDiff;
}

inline jint toInt(jbyte pValue)
{
	return (0xff & pValue);
}
