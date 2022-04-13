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

#include "utils.h"
#include "internal.h"

/**
 * @brief Sets a library flag 'flag' to the given boolean val.
 * 
 * @param flag the flag to modify
 * @param val the value to set the flag to
 */
void zmlSetLibFlag(unsigned int flag, unsigned char val) {
	switch (flag) {
		case ZML_USE_DEGREES:		_zml_usedegrees = val; return;
		case ZML_USE_LEFT_COORDS:	_zml_leftcoords = val; return;
	}
}

/**
 * @brief Takes a value rad, expressed in radians, and converts it to degrees. 
 * 
 * @param rad the value, in radians, to convert to degrees.
 */
__floating zmlToDegrees(__floating rad) {
	return rad / (PI / (__floating) 180.0);
}
/**
 * @brief Takes a value deg, expressed in degrees, and converts it to radians. 
 * 
 * @param rad the value, in degrees, to convert to radians.
 */
__floating zmlToRadians(__floating rad) {
	return rad * (PI / (__floating) 180.0);
}

/**
 * @brief Performs a linear interpolation operation on value val.
 * 
 * @param val the value to interpolate
 * @param start1 the min point of the input range
 * @param stop1 the max point of the input range
 * @param start2 the min point of the output range
 * @param stop2 the max point of the output range
 */
__floating zmlLerp(__floating val, __floating start1, __floating stop1, __floating start2, __floating stop2) {
	return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
}