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
	r.elements = (__floating **) malloc(rows * sizeof(__floating *));
	// allocate each row's memory
	for (unsigned int i = 0; i < rows; i++) {
		r.elements[i] = (__floating *) malloc(cols * sizeof(__floating));
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
			r.elements[row][col] = (row == col) ? 1.0 : 0.0;
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
 * @param vec the vector to set the row to. Must be the same length as the amount of columns in mat.
 */
void zmlSetMatrixRow(zmlMatrix *mat, unsigned int index, zmlVector vec) {
	if (vec.size != mat->cols) {
		printf("zetaml: zmlSetMatrixRow(): invalid sized vector given, function aborted\n");
		return;
	}

	for (unsigned int i = 0; i < mat->cols; i++) {
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
 * @param vec the vector to set the column to. Must be the same length as the amount of rows in mat.
 */
void zmlSetMatrixCol(zmlMatrix *mat, unsigned int index, zmlVector vec) {
	if (vec.size != mat->rows) {
		printf("zetaml: zmlSetMatrixRow(): invalid sized vector given, function aborted\n");
		return;
	}

	for (unsigned int i = 0; i < mat->rows; i++) {
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