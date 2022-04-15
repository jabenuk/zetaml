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

#define testf(x) printf("%s = %f\n", #x, x)

#include <zetamlc.h>
#include <stdio.h>

int main() {
	printf("%d-bit\n\n", sizeof(void *) * 8);

	// ======================
	// matrices
	// ======================

	{

		zmlMatrix m1 = zmlIdentityMatrix(4, 4);
		zmlMultiplyMatScalar(&m1, 4);
		zmlVector v0 = zmlConstructVector(4, 2.0, 5.0, 2.0, 1.0);
		zmlSetMatrixCol(&m1, 3, v0);
		zmlFreeVector(&v0);

		zmlVector v1 = zmlConstructVectorDefault(4, 1.0);
		zmlMultiplyVecMat(&v1, m1);

		zmlPrintM(m1);
		zmlPrintV(v1);

		zmlFreeMatrix(&m1);
		zmlFreeVector(&v1);

	}

	// ======================
	// vectors
	// ======================

	if (0) { // not using

		zmlVector vec1 = zmlConstructVector(2, 2.0, 3.0);
		zmlPrintV(vec1);
		zmlVector vec2 = zmlCopyVector(&vec1);
		zmlPrintV(vec2);

		zmlVector vec_cross1 = zmlConstructVector(3, 0.0, 0.0, 1.0);
		zmlVector vec_cross2 = zmlConstructVector(3, 1.0, 0.0, 0.0);
		zmlVector vec_crossp = zmlCross(vec_cross1, vec_cross2);
		zmlPrintV(vec_crossp);

		testf(zmlDot(vec1, vec1));

		testf(zmlMagnitude(vec1));

		zmlNormalise(&vec1);
		zmlPrintV(vec1);

		zmlFreeVector(&vec1);
		zmlFreeVector(&vec2);
		zmlFreeVector(&vec_cross1);
		zmlFreeVector(&vec_cross2);
		zmlFreeVector(&vec_crossp);

		printf("\n");

	}
	
	// ======================
	// utility functions
	// ======================

	if (0) { // not using

		testf(PI);
		testf(zmlToDegrees(PI));
		testf(zmlToRadians(180));
		testf(zmlLerp(5, 0, 10, 0, 100));
		testf(zmlLerp(50, 0, 100, 0, 10));

		printf("\n");
	
	}

	return 0;
}
