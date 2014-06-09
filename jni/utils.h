/*
 * utils.c
 *
 *  Created on: 26/08/2013
 *      Author: Thiago
 */
#ifndef UTILS
#define UTILS

#include <jni.h>

inline jint absDiff(jbyte pValue1, jbyte pValue2);

inline jint toInt(jbyte pValue);

void normalize(jbyte* pList, int pSize, jint pMax);

#endif



