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