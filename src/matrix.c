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
 * @brief An undefined matrix; no elements.
 * 
 */
const zmlMatrix ZML_NULL_MATRIX = { 0, 0, NULL };

/**
 * @brief Allocate memory for a matrix
 * @param rows the number of rows
 * @param cols the number of cols
 */
zmlMatrix zmlAllocMatrix(unsigned int rows, unsigned int cols) {
	zmlMatrix r;
	r.rows = rows;
	r.cols = cols;

	// allocate array of rows
	r.elements = (__zml_floating **) malloc(rows * sizeof(__zml_floating *));
	// allocate each row's memory
	for (unsigned int i = 0; i < rows; i++) {
		r.elements[i] = (__zml_floating *) malloc(cols * sizeof(__zml_floating));
	}

	return r;
}
/**
 * @brief Free a matrix's memory.
 * 
 * @param mat the matrix to free.
 */
void zmlFreeMatrix(zmlMatrix *mat) {
	// free each row
	for (unsigned int i = 0; i < mat->rows; i++) {
		free(mat->elements[i]);
	}
	// free array of rows
	free(mat->elements);

	mat->elements = NULL;
	mat->rows = 0;
	mat->cols = 0;
}

/**
 * @brief Allocate and return an identity matrix with width and height size.
 * 
 * @param size the size (rows and columns) of the matrix.
 */
zmlMatrix zmlIdentityMatrix(unsigned int rows, unsigned int cols) {
	zmlMatrix r = zmlAllocMatrix(rows, cols);

	for (unsigned int row = 0; row < rows; row++) {
		for (unsigned int col = 0; col < cols; col++) {
			// set to 1 if on the main diagonal
			// initialise all other values to 0.
			r.elements[row][col] = (row == col) ? (__zml_floating) 1.0 : (__zml_floating) 0.0;
		}
	}

	return r;
}

/**
 * @brief Allocate and return a zero matrix with width and height size.
 * 
 * @param size the size (rows and columns) of the matrix.
 */
zmlMatrix zmlZeroMatrix(unsigned int rows, unsigned int cols) {
	zmlMatrix r = zmlAllocMatrix(rows, cols);

	for (unsigned int row = 0; row < rows; row++) {
		for (unsigned int col = 0; col < cols; col++) {
			// init all values to 0.
			r.elements[row][col] = (__zml_floating) 0.0;
		}
	}

	return r;
}

/**
 * @brief copy a matrix's values
 * 
 * @param val the pointer to the matrix to be copied
 */
zmlMatrix zmlCopyMatrix(zmlMatrix *val) {
	zmlMatrix r = zmlAllocMatrix(val->rows, val->cols);

	for (unsigned int row = 0; row < val->rows; row++) {
		for (unsigned int col = 0; col < val->cols; col++) {
			r.elements[row][col] = val->elements[row][col];
		}
	}

	return r;
}

/**
 * @brief get a specified row from the given matrix as a vector.
 * 
 * @param val the matrix to be observed
 * @param index the index of the row to retrieve.
 */
zmlVector zmlGetMatrixRow(zmlMatrix val, unsigned int index) {
	zmlVector r = zmlAllocVector(val.cols);
	for (unsigned int i = 0; i < val.cols; i++) {
		r.elements[i] = val.elements[index][i];
	}
	return r;
}
/**
 * @brief set a row in the given matrix to a specified vector.
 * 
 * @param mat the matrix to be observed
 * @param index the index of the row to set.
 * @param vec the vector to set the row to.
 */
void zmlSetMatrixRow(zmlMatrix *mat, unsigned int index, zmlVector vec) {
	if (vec.size > mat->cols) {
		printf("zetaml: zmlSetMatrixRow(): invalid sized vector given, function aborted\n");
		return;
	}

	for (unsigned int i = 0; i < vec.size; i++) {
		mat->elements[index][i] = vec.elements[i];
	}
}

/**
 * @brief get a specified column from the given matrix as a vector.
 * 
 * @param val the matrix to be observed
 * @param index the index of the column to retrieve.
 */
zmlVector zmlGetMatrixCol(zmlMatrix val, unsigned int index) {
	zmlVector r = zmlAllocVector(val.rows);
	for (unsigned int i = 0; i < val.rows; i++) {
		r.elements[i] = val.elements[i][index];
	}
	return r;
}
/**
 * @brief set a column in the given matrix to a specified vector.
 * 
 * @param mat the matrix to be observed
 * @param index the index of the column to set.
 * @param vec the vector to set the column to.
 */
void zmlSetMatrixCol(zmlMatrix *mat, unsigned int index, zmlVector vec) {
	if (vec.size > mat->rows) {
		printf("zetaml: zmlSetMatrixRow(): invalid sized vector given, function aborted\n");
		return;
	}

	for (unsigned int i = 0; i < vec.size; i++) {
		mat->elements[i][index] = vec.elements[i];
	}
}

/**
 * @brief allocate and return the given matrix in its transposed state - that is to say, the rows and columns of the matrix are swapped. 
 * 
 * @param mat the matrix to transpose
 */
zmlMatrix zmlTransposed(zmlMatrix mat) {
	zmlMatrix r = zmlCopyMatrix(&mat);
	zmlTranspose(&r);
	return r;
}
/**
 * @brief transpose the given matrix by modifying it directly. 
 * 
 * @param mat the matrix to transpose
 */
void zmlTranspose(zmlMatrix *mat) {
	zmlMatrix buf = zmlCopyMatrix(mat);

	for (unsigned int r = 0; r < mat->rows; r++) {
		for (unsigned int c = 0; c < mat->cols; c++) {
			mat->elements[r][c] = buf.elements[c][r];
		}
	}

	zmlFreeMatrix(&buf);
}

/**
 * @brief augment vector 'vec' onto matrix 'mat'.
 * 
 * @param mat the matrix to modify.
 * @param vec the vector to augment onto mat.
 */
void zmlAugmentVec(zmlMatrix *mat, zmlVector vec) {
	if (vec.size != mat->cols) {
		printf("zetaml: zmlSetMatrixRow(): invalid sized vector given, function aborted\n");
		return;
	}

	// increase height by 1
	zmlMatrix buf = zmlAllocMatrix(mat->rows + 1, mat->cols);
	
	// copy mat's values into buf.
	for (unsigned int r = 0; r < mat->rows; r++) {
		for (unsigned int c = 0; c < mat->cols; c++) {
			buf.elements[r][c] = mat->elements[r][c];
		}
	}

	// copy vec's values into the new row in buf.
	for (unsigned int i = 0; i < buf.cols; i++) {
		buf.elements[buf.rows - 1][i] = vec.elements[i];
	}

	// replace mat with allocated buffer
	zmlFreeMatrix(mat);
	*mat = zmlCopyMatrix(&buf);

	// deallocate now-unnecessary buffer
	zmlFreeMatrix(&buf);
}

/**
 * @brief augment matrix 'val' onto matrix 'mat'
 * 
 * @param mat the matrix to modify.
 * @param val the matrix to augment onto mat.
 */
void zmlAugmentMat(zmlMatrix *mat, zmlMatrix val) {
	if (val.cols != mat->cols) {
		printf("zetaml: zmlSetMatrixRow(): invalid sized vector given, function aborted\n");
		return;
	}

	// allocate space for augmented matrix
	zmlMatrix buf = zmlAllocMatrix(mat->rows + val.rows, mat->cols);

	// copy mat's values into buf.
	for (unsigned int r = 0; r < mat->rows; r++) {
		for (unsigned int c = 0; c < mat->cols; c++) {
			buf.elements[r][c] = mat->elements[r][c];
		}
	}

	// copy val's values into buf, offset by the amount of rows in mat.
	for (unsigned int r = mat->rows; r < buf.rows; r++) {
		for (unsigned int c = 0; c < mat->cols; c++) {
			buf.elements[r][c] = val.elements[r - mat->rows][c];
		}
	}

	// replace mat with allocated buffer
	zmlFreeMatrix(mat);
	*mat = zmlCopyMatrix(&buf);

	// deallocate now-unnecessary buffer
	zmlFreeMatrix(&buf);
}

#define _zml_assertSameSize(x, y, rval) {\
	if (x.rows != y.rows || x.cols != y.cols) {\
		printf("zetaml: assertion: given matrices are not the same size!\n");\
		return rval;\
	}\
}

zmlMatrix zmlAddMats_r(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, ZML_NULL_MATRIX);
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlAddMats(&r, v2);
	return r;
}
void zmlAddMats(zmlMatrix *v1, zmlMatrix v2) {
	_zml_assertSameSize((*v1), v2,);
	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			v1->elements[row][col] += v2.elements[row][col];
		}
	}
}
zmlMatrix zmlSubtractMats_r(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, ZML_NULL_MATRIX);
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlSubtractMats(&r, v2);
	return r;
}
void zmlSubtractMats(zmlMatrix *v1, zmlMatrix v2) {
	_zml_assertSameSize((*v1), v2,);
	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			v1->elements[row][col] -= v2.elements[row][col];
		}
	}
}
zmlMatrix zmlMultiplyMats_r(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, ZML_NULL_MATRIX);
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlMultiplyMats(&r, v2);
	return r;
}
void zmlMultiplyMats(zmlMatrix *v1, zmlMatrix v2) {
	_zml_assertSameSize((*v1), v2,);

	// copy v1 into buffer so it isn't changed in the middle of the calculation
	zmlMatrix buf = zmlCopyMatrix(v1);

	// convert rows of v1 to an array of vectors
	zmlVector v1_rows[v1->rows];
	for (unsigned int i = 0; i < v1->rows; i++)	v1_rows[i] = zmlGetMatrixRow(*v1, i);

	// convert columns of v2 to array of vectors
	zmlVector v2_cols[v2.cols];
	for (unsigned int i = 0; i < v2.cols; i++) v2_cols[i] = zmlGetMatrixCol(v2, i);

	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			buf.elements[row][col] = zmlDot(v1_rows[row], v2_cols[col]);
		}
	}

	// set v1 to buf
	zmlFreeMatrix(v1);
	*v1 = zmlCopyMatrix(&buf);

	// free vectors
	for (unsigned int i = 0; i < v1->rows; i++)	zmlFreeVector(&v1_rows[i]);
	for (unsigned int i = 0; i < v2.cols; i++)	zmlFreeVector(&v2_cols[i]);

	// free matrix buffer
	zmlFreeMatrix(&buf);
}

zmlMatrix zmlAddMatScalar_r(zmlMatrix v1, __zml_floating v2) {
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlAddMatScalar(&r, v2);
	return r;
}
void zmlAddMatScalar(zmlMatrix *v1, __zml_floating v2) {
	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			v1->elements[row][col] += v2;
		}
	}
}
zmlMatrix zmlSubtractMatScalar_r(zmlMatrix v1, __zml_floating v2) {
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlSubtractMatScalar(&r, v2);
	return r;
}
void zmlSubtractMatScalar(zmlMatrix *v1, __zml_floating v2) {
	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			v1->elements[row][col] -= v2;
		}
	}
}
zmlMatrix zmlMultiplyMatScalar_r(zmlMatrix v1, __zml_floating v2) {
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlMultiplyMatScalar(&r, v2);
	return r;
}
void zmlMultiplyMatScalar(zmlMatrix *v1, __zml_floating v2) {
	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			v1->elements[row][col] *= v2;
		}
	}
}
zmlMatrix zmlDivideMatScalar_r(zmlMatrix v1, __zml_floating v2) {
	zmlMatrix r = zmlCopyMatrix(&v1);
	zmlDivideMatScalar(&r, v2);
	return r;
}
void zmlDivideMatScalar(zmlMatrix *v1, __zml_floating v2) {
	for (unsigned int row = 0; row < v1->rows; row++) {
		for (unsigned int col = 0; col < v1->cols; col++) {
			v1->elements[row][col] /= v2;
		}
	}
}

unsigned char zmlMatEquals(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, 0);
	for (unsigned int row = 0; row < v1.rows; row++) {
		for (unsigned int col = 0; col < v1.cols; col++) {
			if (v1.elements[row][col] != v2.elements[row][col])
				return 0;
		}
	}
	return 1;
}
unsigned char zmlMatGT(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, 0);
	for (unsigned int row = 0; row < v1.rows; row++) {
		for (unsigned int col = 0; col < v1.cols; col++) {
			if (v1.elements[row][col] <= v2.elements[row][col])
				return 0;
		}
	}
	return 1;
}
unsigned char zmlMatGTE(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, 0);
	for (unsigned int row = 0; row < v1.rows; row++) {
		for (unsigned int col = 0; col < v1.cols; col++) {
			if (v1.elements[row][col] < v2.elements[row][col])
				return 0;
		}
	}
	return 1;
}
unsigned char zmlMatLT(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, 0);
	for (unsigned int row = 0; row < v1.rows; row++) {
		for (unsigned int col = 0; col < v1.cols; col++) {
			if (v1.elements[row][col] >= v2.elements[row][col])
				return 0;
		}
	}
	return 1;
}
unsigned char zmlMatLTE(zmlMatrix v1, zmlMatrix v2) {
	_zml_assertSameSize(v1, v2, 0);
	for (unsigned int row = 0; row < v1.rows; row++) {
		for (unsigned int col = 0; col < v1.cols; col++) {
			if (v1.elements[row][col] > v2.elements[row][col])
				return 0;
		}
	}
	return 1;
}