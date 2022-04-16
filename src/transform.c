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
 * @brief produces a translation matrix from a given matrix (mat) and the desired 3D vector vec. 
 * 
 * @param mat the matrix to base the translation matrix on.
 * @param vec the vector to use as the translation factor.
 */
zmlMatrix zmlTranslated(zmlMatrix mat, zmlVector vec) {
	zmlMatrix r = zmlCopyMatrix(&mat);
	zmlTranslate(&r, vec);
	return r;
}

/**
 * @brief alternative to zmlTranslated() that modifies mat instead of allocating and returning a new matrix.
 * 
 * @param mat the matrix to base the translation matrix on.
 * @param vec the vector to use as the translation factor.
 */
void zmlTranslate(zmlMatrix *mat, zmlVector vec) {
	if (mat->cols != 4 || mat->rows != 4) {
		printf("zetaml: zmlTranslate(): given matrix is not 4x4, no transformation performed!\n");
	}
	if (vec.size != 3) {
		printf("zetaml: zmlTranslate(): given vector is not of size 3, no transformation performed!\n");
		return;
	}
	// if there is no translation value then skip the rest of the function
	if (zmlVecEqualsScalar(vec, (__floating) 0.0)) {
		return;
	}

	zmlVector r = zmlConstructVectorDefault(4, (__floating) 0.0); // allocate and initialise vector r as a zero vector
	// loop through the columns in the matrix (this is asserted to be 4)
	for (unsigned int i = 0; i < 4; i++) {
		zmlVector col = zmlGetMatrixCol(*mat, i);
		// multiply by the appropriate vector element except on the last column
		if (i < 3) zmlMultiplyVecScalar(&col, vec.elements[i]);
		
		// add this to the result vector
		zmlAddVecs(&r, col);

		zmlFreeVector(&col);
	}
	
	zmlSetMatrixCol(mat, 3, r);

	zmlFreeVector(&r);
}

/**
 * @brief produces a translation matrix from an identity matrix and the desired 3D vector vec. 
 * 
 * @param vec the vector to use as the translation factor.
 */
zmlMatrix zmlTranslateIdentity(zmlVector vec) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlTranslate(&r, vec);
	return r;
}

/**
 * @brief produces a rotation matrix by rotating mat by angle on the given axes.
 * 
 * @param mat the matrix to base the rotation matrix on.
 * @param angle the angle to rotate the specified axes by.
 * @param x the multiplier for the X axis of rotation (set to 0 if you don't want X rotation).
 * @param y the multiplier for the Y axis of rotation (set to 0 if you don't want Y rotation).
 * @param z the multiplier for the Z axis of rotation (set to 0 if you don't want Z rotation).
 */
zmlMatrix zmlRotated(zmlMatrix mat, __floating angle, __floating x, __floating y, __floating z) {
	zmlMatrix r = zmlCopyMatrix(&mat);
	zmlRotate(&r, angle, x, y, z);
	return r;
}
/**
 * @brief alternative to zmlRotated() that modifies mat instead of allocating and returning a new matrix.
 * 
 * @param mat the matrix to base the rotation matrix on.
 * @param angle the angle to rotate the specified axes by.
 * @param x the multiplier for the X axis of rotation (set to 0 if you don't want X rotation).
 * @param y the multiplier for the Y axis of rotation (set to 0 if you don't want Y rotation).
 * @param z the multiplier for the Z axis of rotation (set to 0 if you don't want Z rotation).
 */
void zmlRotate(zmlMatrix *mat, __floating angle, __floating x, __floating y, __floating z) {
	if (mat->cols != 4 || mat->rows != 4) {
		printf("zetaml: zmlRotate(): given matrix is not 4x4, no transformation performed!\n");
	}
	// if there is no rotation then skip the rest of the function
	if (angle == (__floating) 0.0 || (
		x == (__floating) 0.0 &&
		y == (__floating) 0.0 &&
		z == (__floating) 0.0
	)) {
		return;
	}

	const __floating cos_angle = (__floating) cos(angle);
	const __floating sin_angle = (__floating) sin(angle);
	zmlMatrix rotated = zmlZeroMatrix(4, 4);

	// create a unit vector with axes
	zmlVector axes = zmlConstructVector(3, x, y, z);
	zmlNormalise(&axes);
	
	// i have not got a clue:

	zmlVector temp = zmlMultiplyVecScalar_r(axes, (__floating) 1.0 - cos_angle);

	rotated.elements[0][0] = cos_angle + temp.elements[0] * axes.elements[0];
	rotated.elements[1][0] = temp.elements[0] * axes.elements[1] + sin_angle * axes.elements[2];
	rotated.elements[2][0] = temp.elements[0] * axes.elements[2] - sin_angle * axes.elements[1];

	rotated.elements[0][1] = temp.elements[1] * axes.elements[0] - sin_angle * axes.elements[2];
	rotated.elements[1][1] = cos_angle + temp.elements[1] * axes.elements[1];
	rotated.elements[2][1] = temp.elements[1] * axes.elements[2] + sin_angle * axes.elements[0];

	rotated.elements[0][2] = temp.elements[2] * axes.elements[0] + sin_angle * axes.elements[1];
	rotated.elements[1][2] = temp.elements[2] * axes.elements[1] - sin_angle * axes.elements[0];
	rotated.elements[2][2] = cos_angle + temp.elements[2] * axes.elements[2];

	zmlMatrix result = zmlCopyMatrix(mat);

	// apply this rotated matrix onto mat
	for (unsigned int i = 0; i < 3; i++) {
		// get columns from mat + store them as vectors in a buffer array
		zmlVector matcols[3];
		for (unsigned int ii = 0; ii < 3; ii++) matcols[ii] = zmlGetMatrixCol(*mat, ii);

		// col is the resulting column that will replace the current column in the result matrix
		zmlVector col = zmlConstructVectorDefault(4, (__floating) 0.0);

		for (unsigned int j = 0; j < 3; j++) {
			// multiply each matcol by the appropriate element in rotated
			zmlVector m = zmlMultiplyVecScalar_r(matcols[j], rotated.elements[j][i]);
			// add the product of this to col
			zmlAddVecs(&col, m);

			zmlFreeVector(&m);
		}

		// update the columns in result
		zmlSetMatrixCol(&result, i, col);

		for (unsigned int ii = 0; ii < 3; ii++) zmlFreeVector(&matcols[ii]);
		zmlFreeVector(&col);
	}

	// return r into mat
	zmlFreeMatrix(mat);
	*mat = zmlCopyMatrix(&result);
	
	zmlFreeVector(&axes);
	zmlFreeVector(&temp);
	zmlFreeMatrix(&result);
	zmlFreeMatrix(&rotated);
}
/**
 * @brief produces a rotation matrix by rotating a new identity matrix by angle on the given axes.
 * 
 * @param angle the angle to rotate the specified axes by.
 * @param x the multiplier for the X axis of rotation (set to 0 if you don't want X rotation).
 * @param y the multiplier for the Y axis of rotation (set to 0 if you don't want Y rotation).
 * @param z the multiplier for the Z axis of rotation (set to 0 if you don't want Z rotation).
 */
zmlMatrix zmlRotateIdentity(__floating angle, __floating x, __floating y, __floating z) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlRotate(&r, angle, x, y, z);
	return r;
}

/**
 * @brief produces a scale matrix from a given matrix (mat) and the desired 3D vector vec. 
 * 
 * @param mat the matrix to base the scale matrix on.
 * @param vec the vector to use as the scale factor.
 */
zmlMatrix zmlScaled(zmlMatrix *mat, zmlVector vec) {
	zmlMatrix r = zmlCopyMatrix(mat);
	zmlScale(&r, vec);
	return r;
}

/**
 * @brief alternative to zmlScaled() that modifies mat instead of allocating and returning a new matrix.
 * 
 * @param mat the matrix to base the scale matrix on.
 * @param vec the vector to use as the scale factor.
 */
void zmlScale(zmlMatrix *mat, zmlVector vec) {
	if (mat->cols != 4 || mat->rows != 4) {
		printf("zetaml: zmlScale(): given matrix is not 4x4, no transformation performed!\n");
	}
	if (vec.size != 3) {
		printf("zetaml: zmlScale(): given vector is not of size 3, no transformation performed!\n");
		return;
	}
	// if there is no scale value then skip the rest of the function
	if (zmlVecEqualsScalar(vec, (__floating) 0.0)) {
		return;
	}

	zmlMatrix r = zmlCopyMatrix(mat);

	// the fourth column, similarly to in zmlRotate(), is not modified.
	for (unsigned int i = 0; i < 3; i++) {
		zmlVector col = zmlGetMatrixCol(*mat, i);
		zmlMultiplyVecScalar(&col, vec.elements[i]);

		// update column in r
		zmlSetMatrixCol(&r, i, col);

		zmlFreeVector(&col);
	}

	// return r
	zmlFreeMatrix(mat);
	*mat = zmlCopyMatrix(&r);

	zmlFreeMatrix(&r);
}

/**
 * @brief produces a scale matrix from an identity matrix and the desired 3D vector vec. 
 * 
 * @param vec the vector to use as the scale factor.
 */
zmlMatrix zmlScaleIdentity(zmlVector vec) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlScale(&r, vec);
	return r;
}