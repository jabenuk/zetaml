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

#define testf(x) printf("%s = %f\n", #x, x)

#include <zetamlc.h>

int main() {
	printf("%d-bit\n\n", sizeof(void *) * 8);

	// ======================
	// vectors
	// ======================

	zmlVector vec1 = zmlConstructVector(2, 2.23412, 6.15203);

	zmlPrintV(&vec1);

	zmlFreeVector(&vec1);

	printf("\n");
	
	// ======================
	// utility functions
	// ======================

	testf(PI);
	testf(zmlToDegrees(PI));
	testf(zmlToRadians(180));
	testf(zmlLerp(5, 0, 10, 0, 100));
	testf(zmlLerp(50, 0, 100, 0, 10));

	printf("\n");
	return 0;
}
