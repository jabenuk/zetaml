/* *************************************************************************************** */
/* 						THE ZETA MATHS LIBRARY LICENSE INFORMATION						   */
/* *************************************************************************************** */
/* Copyright (c) 2022 Jack Bennett														   */
/* --------------------------------------------------------------------------------------- */
/* THE  SOFTWARE IS  PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS  OR IMPLIED, */
/* INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS FOR  A */
/* PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN  NO EVENT SHALL  THE  AUTHORS  OR COPYRIGHT */
/* HOLDERS  BE  LIABLE  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF */
/* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.											   */
/* *************************************************************************************** */

#include "internal.h"
#include "zetamlc.h"

/**
 * @brief An undefined vector; no dimension.
 * 
 */
const zmlVector ZML_NULL_VECTOR = { 0, NULL };

/**
 * @brief Allocate memory for a vector struct, and return the empty vector. Elements are NOT initialised!
 * 
 * @param size the size of the vector.
 */

zmlVector zmlAllocVector(unsigned int size) {
	zmlVector r;
	r.size = size;
	r.elements = (__floating *) malloc(3 * size * sizeof(__floating));
	
	return r;
}
/**
 * @brief Free a vector's memory.
 * 
 * @param vec the vector to free.
 */
void zmlFreeVector(zmlVector *vec) {
	free(vec->elements);

	vec->elements = NULL;
	vec->size = 0;
}

/**
 * @brief Construct a vector with a default value.
 * 
 * @param size the size of the vector.
 * @param val the value to initialise the vector with.
 */
zmlVector zmlConstructVectorDefault(unsigned int size, __floating val) {
	zmlVector r = zmlAllocVector(size);
	memset(r.elements, val, size);

	return r;
}

/**
 * @brief Construct a vector with a default value.
 * 
 * @param size the size of the vector.
 * @param ... the values to initialise the vector with. Must be floating-point!
 */
zmlVector zmlConstructVector(unsigned int size, ...) {
	#ifdef ZML_USING_FLOATS
		#pragma message "Variadic arguments do not support floats. You should rebuild zetaml with -DZML_USE_FLOATS=OFF! (or avoid zetaml functions with variadic arguments)"
		return ZML_NULL_VECTOR;
	#endif

	zmlVector r = zmlAllocVector(size);

	va_list vl;
	va_start(vl, size);

	for (unsigned int i = 0; i < size; i++) {
		r.elements[i] = va_arg(vl, __floating);
	}
	va_end(vl);

	return r;
}

/**
 * @brief copy a vector's values
 * 
 * @param val the pointer to the vector to be copied
 */
zmlVector zmlCopyVector(zmlVector *val) {
	zmlVector r = zmlAllocVector(val->size);

	for (unsigned int i = 0; i < val->size; i++) {
		r.elements[i] = val->elements[i];
	}

	return r;
}

/**
 * @brief produces a vector that is the cross product of the two given vectors; this represents the vector perpendicular to the plane that v1 and v2 create. 
 * 
 * @param v1 the first vector to operate on.
 * @param v2 the second vector to operate on.
 */
zmlVector zmlCross(zmlVector *v1, zmlVector *v2) {
	if (v1->size != 3 || v2->size != 3) {
		printf("zetaml: zmlCross(): one or both given vectors are not 3-dimensional! ZML_NULL_VECTOR returned!\n");
		return ZML_NULL_VECTOR;
	}

	zmlVector r = zmlAllocVector(3);

	r.elements[0] = v1->elements[1] * v2->elements[2] - v1->elements[2] * v2->elements[1];
	r.elements[1] = v1->elements[2] * v2->elements[0] - v1->elements[0] * v2->elements[2];
	r.elements[2] = v1->elements[0] * v2->elements[1] - v1->elements[1] * v2->elements[0];

	return r;
}

/**
 * @brief produces a vector that is the dot (scalar) product of the two given vectors v1 and v2. 
 * 
 * @param v1 the first vector to operate on.
 * @param v2 the second vector to operate on.
 */
__floating zmlDot(zmlVector *v1, zmlVector *v2) {
	__floating r = (__floating) 0.0;

	// add each vector element to the result, multiplied by the other equivalent element.
	if (v1->size == v2->size) {
		for (unsigned int i = 0; i < v1->size; i++) {
			r += (__floating) (v1->elements[i] * v2->elements[i]);
		}
	} else {
		// 0 is returned if arguments are different dimensions
		printf("zetaml: zmlDot(): the given vectors are of different sizes! 0 returned!\n");
	}

	return r;
}