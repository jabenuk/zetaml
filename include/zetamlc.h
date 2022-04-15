/* *************************************************************************************** */
/* 						THE ZETA MATHS LIBRARY LICENSE INFORMATION						   */
/* *************************************************************************************** */
/* Copyright (c) 2021 Jack Bennett														   */
/* --------------------------------------------------------------------------------------- */
/* THE  SOFTWARE IS  PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS  OR IMPLIED, */
/* INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS FOR  A */
/* PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN  NO EVENT SHALL  THE  AUTHORS  OR COPYRIGHT */
/* HOLDERS  BE  LIABLE  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF */
/* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.											   */
/* *************************************************************************************** */

#pragma once
#ifndef __ZETAML_H__
#define __ZETAML_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ZML_32_BIT
#	ifndef ZML_USING_FLOATS
#		warning "You are using a 32-bit build of zetaml but are not using floats. Using doubles might cause problems, you should rebuild zetaml with -DZML_USE_FLOATS=ON!"
#	endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define PI (__floating) 3.141592653589793238463

// ---
// The following macros are flags that can be used in zmlSetLibFlag().

#define ZML_USE_DEGREES 0x01		// use degrees instead of radians.
#define ZML_USE_LEFT_COORDS 0x02	// use left-handed coordinates instead of right-handed coordinates

// ==============================================================================
// *****					 CONFIGURATION FUNCTIONS						*****
// ==============================================================================

/**
 * @brief Sets a library flag 'flag' to the given boolean val.
 * 
 * @param flag the flag to modify
 * @param val the value to set the flag to
 */
extern void zmlSetLibFlag(unsigned int flag, unsigned char val);

// ==============================================================================
// *****				   PUBLIC VECTOR FUNCTIONALITY						*****
// ==============================================================================

/**
 * @brief A vector construct of any given size. 
 * 
 */
typedef struct {
	unsigned int size;
	__floating *elements;
} zmlVector;

/**
 * @brief An undefined vector; no dimension.
 * 
 */
extern const zmlVector ZML_NULL_VECTOR;

/**
 * @brief Allocate memory for a vector struct, and return the empty vector. Elements are NOT initialised!
 * 
 * @param size the size of the vector.
 */
extern zmlVector zmlAllocVector(unsigned int size);
/**
 * @brief Free a vector's memory.
 * 
 * @param vec the vector to free.
 */
extern void zmlFreeVector(zmlVector *vec);

/**
 * @brief Construct a vector with a default value.
 * 
 * @param size the size of the vector.
 * @param val the value to initialise the vector with.
 */
extern zmlVector zmlConstructVectorDefault(unsigned int size, __floating val);

/**
 * @brief Construct a vector with a default value.
 * 
 * @param size the size of the vector.
 * @param ... the values to initialise the vector with. Must be floating-point!
 */
extern zmlVector zmlConstructVector(unsigned int size, ...);

/**
 * @brief copy a vector's values
 * 
 * @param val the pointer to the vector to be copied
 */
extern zmlVector zmlCopyVector(zmlVector *val);

/**
 * @brief produces a vector that is the cross product of the two given vectors; this represents the vector perpendicular to the plane that v1 and v2 create. 
 * 
 * @param v1 the first vector to operate on.
 * @param v2 the second vector to operate on.
 */
extern zmlVector zmlCross(zmlVector *v1, zmlVector *v2);

/**
 * @brief produces a vector that is the dot (scalar) product of the two given vectors v1 and v2. 
 * 
 * @param v1 the first vector to operate on.
 * @param v2 the second vector to operate on.
 */
extern __floating zmlDot(zmlVector *v1, zmlVector *v2);

// ==============================================================================
// *****					PUBLIC UTILITY FUNCTIONS						*****
// ==============================================================================

/**
 * @brief Takes a value rad, expressed in radians, and converts it to degrees. 
 * 
 * @param rad the value, in radians, to convert to degrees.
 */
extern __floating zmlToDegrees(__floating rad);

/**
 * @brief Takes a value deg, expressed in degrees, and converts it to radians. 
 * 
 * @param rad the value, in degrees, to convert to radians.
 */
extern __floating zmlToRadians(__floating rad);

/**
 * @brief Takes a vector value, val, and converts it to a formatted string.
 * 
 * @param val the vector to format and express as a string.
 * @param str the string to return the value into.
 */
extern void zmlToStringV(zmlVector *val, char *str);

/**
 * @brief Prints the output of zmlToStringV(val) to stdout (with new line!).
 * 
 * @param val the vector to format and print.
 */
extern void zmlPrintV(zmlVector *val);

/**
 * @brief Performs a linear interpolation operation on value val.
 * 
 * @param val the value to interpolate
 * @param start1 the min point of the input range
 * @param stop1 the max point of the input range
 * @param start2 the min point of the output range
 * @param stop2 the max point of the output range
 */
extern __floating zmlLerp(__floating val, __floating start1, __floating stop1, __floating start2, __floating stop2);

#ifdef __cplusplus
}
#endif

#endif