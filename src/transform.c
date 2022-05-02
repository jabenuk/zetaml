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
		return;
	}
	if (vec.size != 3) {
		printf("zetaml: zmlTranslate(): given vector is not of size 3, no transformation performed!\n");
		return;
	}
	// if there is no translation value then skip the rest of the function
	if (zmlVecEqualsScalar(vec, (__zml_floating) 0.0)) {
		return;
	}

	zmlVector r = zmlConstructVectorDefault(4, (__zml_floating) 0.0); // allocate and initialise vector r as a zero vector
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
zmlMatrix zmlRotated(zmlMatrix mat, __zml_floating angle, __zml_floating x, __zml_floating y, __zml_floating z) {
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
void zmlRotate(zmlMatrix *mat, __zml_floating angle, __zml_floating x, __zml_floating y, __zml_floating z) {
	if (mat->cols != 4 || mat->rows != 4) {
		printf("zetaml: zmlRotate(): given matrix is not 4x4, no transformation performed!\n");
	}
	// if there is no rotation then skip the rest of the function
	if (angle == (__zml_floating) 0.0 || (
		x == (__zml_floating) 0.0 &&
		y == (__zml_floating) 0.0 &&
		z == (__zml_floating) 0.0
	)) {
		return;
	}

	const __zml_floating cos_angle = (__zml_floating) cos(angle);
	const __zml_floating sin_angle = (__zml_floating) sin(angle);
	zmlMatrix rotated = zmlZeroMatrix(4, 4);

	// create a unit vector with axes
	zmlVector axes = zmlAllocVector(3);
	axes.elements[0] = x;
	axes.elements[1] = y;
	axes.elements[2] = z;
	zmlNormalise(&axes);
	
	// i have not got a clue:

	zmlVector temp = zmlMultiplyVecScalar_r(axes, (__zml_floating) 1.0 - cos_angle);

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
		zmlVector col = zmlConstructVectorDefault(4, (__zml_floating) 0.0);

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
zmlMatrix zmlRotateIdentity(__zml_floating angle, __zml_floating x, __zml_floating y, __zml_floating z) {
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
	if (zmlVecEqualsScalar(vec, (__zml_floating) 0.0)) {
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

/**
 * @brief allocates and initialises an orthographic projection matrix based on the given values. Uses left-handed coordinates!
 * 
 * @param lm the left-most boundary.
 * @param rm the right-most boundary.
 * @param bm the bottom-most boundary.
 * @param tm the top-most boundary.
 * @param zn the nearest Z coordinate that will be rendered.
 * @param zf the farthest Z coordinate that will be rendered.
 */
zmlMatrix zmlConstructOrthoMatrixLH(__zml_floating lm, __zml_floating rm, __zml_floating bm, __zml_floating tm, __zml_floating zn, __zml_floating zf) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlUpdateOrthoMatrixLH(&r, lm, rm, bm, tm, zn, zf);
	return r;
}
/**
 * @brief allocates and initialises an orthographic projection matrix based on the given values. Uses right-handed coordinates!
 * 
 * @param lm the left-most boundary.
 * @param rm the right-most boundary.
 * @param bm the bottom-most boundary.
 * @param tm the top-most boundary.
 * @param zn the nearest Z coordinate that will be rendered.
 * @param zf the farthest Z coordinate that will be rendered.
 */
zmlMatrix zmlConstructOrthoMatrixRH(__zml_floating lm, __zml_floating rm, __zml_floating bm, __zml_floating tm, __zml_floating zn, __zml_floating zf) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlUpdateOrthoMatrixRH(&r, lm, rm, bm, tm, zn, zf);
	return r;
}

/**
 * @brief modifies a matrix to be an orthographic projection matrix based on the given values. Uses left-handed coordinates!
 * 
 * @param mat the matrix to modify.
 * @param lm the left-most boundary.
 * @param rm the right-most boundary.
 * @param bm the bottom-most boundary.
 * @param tm the top-most boundary.
 * @param zn the nearest Z coordinate that will be rendered.
 * @param zf the farthest Z coordinate that will be rendered.
 */
void zmlUpdateOrthoMatrixLH(zmlMatrix *mat, __zml_floating lm, __zml_floating rm, __zml_floating bm, __zml_floating tm, __zml_floating zn, __zml_floating zf) {
	if (mat->rows != 4 || mat->cols != 4) {
		printf("zetaml: zmlUpdateOrthoMatrixLH(): given matrix is not 4x4, no transformation performed!\n");
		return;
	}
	
	// set the scale of the matrix to the given values
	mat->elements[0][0] = 2 / (rm - lm);
	mat->elements[1][1] = 2 / (tm - bm);
	mat->elements[2][2] = 2 / (zf - zn);

	// set the translation of the matrix to the given values.
	mat->elements[0][3] = -(rm + lm) / (rm - lm);
	mat->elements[1][3] = -(tm + bm) / (tm - bm);
	mat->elements[2][3] = -(zf + zn) / (zf - zn);
}
/**
 * @brief  modifies a matrix to be an orthographic projection matrix based on the given values. Uses right-handed coordinates!
 * 
 * @param mat the matrix to modify.
 * @param lm the left-most boundary.
 * @param rm the right-most boundary.
 * @param bm the bottom-most boundary.
 * @param tm the top-most boundary.
 * @param zn the nearest Z coordinate that will be rendered.
 * @param zf the farthest Z coordinate that will be rendered.
 */
void zmlUpdateOrthoMatrixRH(zmlMatrix *mat, __zml_floating lm, __zml_floating rm, __zml_floating bm, __zml_floating tm, __zml_floating zn, __zml_floating zf) {
	if (mat->rows != 4 || mat->cols != 4) {
		printf("zetaml: zmlUpdateOrthoMatrixRH(): given matrix is not 4x4, no transformation performed!\n");
		return;
	}
	
	// set the scale of the matrix to the given values
	mat->elements[0][0] = 2 / (rm - lm);
	mat->elements[1][1] = 2 / (tm - bm);
	mat->elements[2][2] = -2 / (zf - zn);

	// set the translation of the matrix to the given values.
	mat->elements[0][3] = -(rm + lm) / (rm - lm);
	mat->elements[1][3] = -(tm + bm) / (tm - bm);
	mat->elements[2][3] = -(zf + zn) / (zf - zn);
}


/**
 * @brief allocates and initialises a perspective projection matrix based on the given values. Uses left-handed coordinates!
 * 
 * @param near specifies the distance from the viewer to the nearest clipping plane.
 * @param far specifies the distance from the viewer to the farthest clipping plane.
 * @param fovy the angle of the field of view in the y direction.
 * @param aspect_ratio the aspect ratio of the viewport.
 */
zmlMatrix zmlConstructPerspectiveMatrixLH(__zml_floating near, __zml_floating far, __zml_floating fovy, __zml_floating aspect_ratio) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlUpdatePerspectiveMatrixLH(&r, near, far, fovy, aspect_ratio);
	return r;
}
/**
 * @brief allocates and initialises a perspective projection matrix based on the given values. Uses right-handed coordinates!
 * 
 * @param near specifies the distance from the viewer to the nearest clipping plane.
 * @param far specifies the distance from the viewer to the farthest clipping plane.
 * @param fovy the angle of the field of view in the y direction.
 * @param aspect_ratio the aspect ratio of the viewport.
 */
zmlMatrix zmlConstructPerspectiveMatrixRH(__zml_floating near, __zml_floating far, __zml_floating fovy, __zml_floating aspect_ratio) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlUpdatePerspectiveMatrixRH(&r, near, far, fovy, aspect_ratio);
	return r;
}

/**
 * @brief modifies a matrix to be a perspective projection matrix based on the given values. Uses left-handed coordinates!
 * 
 * @param near specifies the distance from the viewer to the nearest clipping plane.
 * @param far specifies the distance from the viewer to the farthest clipping plane.
 * @param fovy the angle of the field of view in the y direction.
 * @param aspect_ratio the aspect ratio of the viewport.
 */
void zmlUpdatePerspectiveMatrixLH(zmlMatrix *mat, __zml_floating near, __zml_floating far, __zml_floating fovy, __zml_floating aspect_ratio) {
	if (mat->rows != 4 || mat->cols != 4) {
		printf("zetaml: zmlUpdateOrthoMatrixRH(): given matrix is not 4x4, no transformation performed!\n");
		return;
	}
	
	const __zml_floating tfovy_half = tan(fovy / 2);

	mat->elements[0][0] = 1 / (aspect_ratio * tfovy_half);
	mat->elements[1][1] = 1 / tfovy_half;

	mat->elements[2][2] = (near + far) / (far - near);
	mat->elements[3][2] = 1;

	mat->elements[2][3] = -(2 * far * near) / (far - near);
}
/**
 * @brief modifies a matrix to be a perspective projection matrix based on the given values. Uses right-handed coordinates!
 * 
 * @param near specifies the distance from the viewer to the nearest clipping plane.
 * @param far specifies the distance from the viewer to the farthest clipping plane.
 * @param fovy the angle of the field of view in the y direction.
 * @param aspect_ratio the aspect ratio of the viewport.
 */
void zmlUpdatePerspectiveMatrixRH(zmlMatrix *mat, __zml_floating near, __zml_floating far, __zml_floating fovy, __zml_floating aspect_ratio) {
	if (mat->rows != 4 || mat->cols != 4) {
		printf("zetaml: zmlUpdateOrthoMatrixRH(): given matrix is not 4x4, no transformation performed!\n");
		return;
	}
	
	const __zml_floating tfovy_half = tan(fovy / 2);

	mat->elements[0][0] = 1 / (aspect_ratio * tfovy_half);
	mat->elements[1][1] = 1 / tfovy_half;

	mat->elements[2][2] = -(near + far) / (far - near);
	mat->elements[3][2] = -1;

	mat->elements[2][3] = -(2 * far * near) / (far - near);
}

/**
 * @brief allocates and initialises a look-at matrix based on the given values. Uses left-handed coordinates!
 * 
 * @param pos the position of the viewer/camera.
 * @param focus the position the viewer/camera is looking at.
 * @param up an absolute unit vector indicating the up direction. If Y is the 'up' axis, set this to be ( 0, 1, 0 ), for example.
 */
zmlMatrix zmlConstructLookAtMatrixLH(zmlVector pos, zmlVector focus, zmlVector up) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlUpdateLookAtMatrixLH(&r, pos, focus, up);
	return r;
}
/**
 * @brief allocates and initialises a look-at matrix based on the given values. Uses right-handed coordinates!
 * 
 * @param pos the position of the viewer/camera.
 * @param focus the position the viewer/camera is looking at.
 * @param up an absolute unit vector indicating the up direction. If Y is the 'up' axis, set this to be ( 0, 1, 0 ), for example.
 */
zmlMatrix zmlConstructLookAtMatrixRH(zmlVector pos, zmlVector focus, zmlVector up) {
	zmlMatrix r = zmlIdentityMatrix(4, 4);
	zmlUpdateLookAtMatrixRH(&r, pos, focus, up);
	return r;
}

/**
 * @brief modifies a matrix to be a look-at matrix based on the given values. Uses left-handed coordinates!
 * 
 * @param mat the matrix to modify.
 * @param pos the position of the viewer/camera.
 * @param focus the position the viewer/camera is looking at.
 * @param up an absolute unit vector indicating the up direction. If Y is the 'up' axis, set this to be ( 0, 1, 0 ), for example.
 */
void zmlUpdateLookAtMatrixLH(zmlMatrix *mat, zmlVector pos, zmlVector focus, zmlVector up) {
	// the direction the camera is facing in (reverse to the actual direction, must be negated when used)
	zmlVector dir = zmlSubtractVecs_r(focus, pos); zmlNormalise(&dir);
	// right direction relative to the camera's direction
	zmlVector right = zmlCross(dir, up); zmlNormalise(&right);
	// up direction relative to the camera's direction
	zmlVector rup = zmlCross(right, dir);

	zmlMatrix r = zmlIdentityMatrix(4, 4);

	// first row = relative right direction
	zmlSetMatrixRow(&r, 0, right);
	// second row = relative up direction
	zmlSetMatrixRow(&r, 1, rup);
	// third row = relative forward direction
	zmlSetMatrixRow(&r, 2, dir);

	// fourth column = dot product of the vector on each row and the position of the camera
	// the fourth element in this column is left unchanged (identity matrix, so 1)
	zmlVector fcol = zmlAllocVector(3);
	fcol.elements[0] = -zmlDot(right, pos);
	fcol.elements[1] = -zmlDot(rup, pos);
	fcol.elements[2] = -zmlDot(dir, pos);
	zmlSetMatrixCol(&r, 3, fcol);

	// (the fourth row is therefore kept as [ 0, 0, 0, 1 ].)

	// return r into mat
	zmlFreeMatrix(mat);
	*mat = zmlCopyMatrix(&r);

	zmlFreeVector(&dir);
	zmlFreeVector(&right);
	zmlFreeVector(&rup);
	zmlFreeVector(&fcol);
	zmlFreeMatrix(&r);
}
/**
 * @brief modifies a matrix to be a look-at matrix based on the given values. Uses right-handed coordinates!
 * 
 * @param mat the matrix to modify.
 * @param pos the position of the viewer/camera.
 * @param focus the position the viewer/camera is looking at.
 * @param up an absolute unit vector indicating the up direction. If Y is the 'up' axis, set this to be ( 0, 1, 0 ), for example.
 */
void zmlUpdateLookAtMatrixRH(zmlMatrix *mat, zmlVector pos, zmlVector focus, zmlVector up) {
	// the direction the camera is facing in (reverse to the actual direction, must be negated when used)
	zmlVector dir = zmlSubtractVecs_r(focus, pos); zmlNormalise(&dir);
	// right direction relative to the camera's direction
	zmlVector right = zmlCross(dir, up); zmlNormalise(&right);
	// up direction relative to the camera's direction
	zmlVector rup = zmlCross(right, dir);

	zmlMatrix r = zmlIdentityMatrix(4, 4);

	// negative version of dir (for right handed coords)
	zmlVector ndir = zmlMultiplyVecScalar_r(dir, -1);

	// first row = relative right direction
	zmlSetMatrixRow(&r, 0, right);
	// second row = relative up direction
	zmlSetMatrixRow(&r, 1, rup);
	// third row = relative forward direction (reverse)
	zmlSetMatrixRow(&r, 2, ndir);

	// fourth column = dot product of the vector on each row and the position of the camera
	// the fourth element in this column is left unchanged (identity matrix, so 1)
	zmlVector fcol = zmlAllocVector(3);
	fcol.elements[0] = -zmlDot(right, pos);
	fcol.elements[1] = -zmlDot(rup, pos);
	fcol.elements[2] = zmlDot(dir, pos);
	zmlSetMatrixCol(&r, 3, fcol);

	// (the fourth row is therefore kept as [ 0, 0, 0, 1 ].)

	// return r into mat
	zmlFreeMatrix(mat);
	*mat = zmlCopyMatrix(&r);

	zmlFreeVector(&dir);
	zmlFreeVector(&ndir);
	zmlFreeVector(&right);
	zmlFreeVector(&rup);
	zmlFreeVector(&fcol);
	zmlFreeMatrix(&r);
}
