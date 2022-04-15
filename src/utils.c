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
 * @param deg the value, in degrees, to convert to radians.
 */
__floating zmlToRadians(__floating deg) {
	return deg * (PI / (__floating) 180.0);
}

/**
 * @brief Takes a vector value, val, and converts it to a formatted string.
 * 
 * @param val the vector to format and express as a string.
 * @param str the string to return the value into.
 */
void zmlToStringV(zmlVector *val, char *str) {
	// TODO: this function can probably be optimised!

	// getting the total maximum length of the string
	// start off with 1 for null terminator.
	unsigned int maxlen = 1;

	// each number (floating-point so digits + 5 decimal places + decimal point + potential minus sign) + ", " on every one (except the last)
	for (unsigned int i = 0; i < val->size - 1; i++) {
		maxlen += _zml_getDigitsi(val->elements[i]) + 9;
	}
	maxlen += _zml_getDigitsi(val->elements[val->size - 1]) + 7;

	// (this is a good time to allocate memory for the list of comma-separated values)
	char elementsbuffer[maxlen];
	memset(elementsbuffer, 0, maxlen);

	// there is also "( " and " )" on either side (4) AND the type descriptor (+ ()). This is "vec[size]" so it is 9 + however-many-digits characters long + null term.
	maxlen += 10 + _zml_getDigitsi(val->size);
	
	// the list of elements is in the format 3, 6, 8, 2, ...
	for (unsigned int i = 0; i < val->size; i++) {
		// the amount of digits in the number
		unsigned int numdigits = _zml_getDigitsi(val->elements[i]) + 6; // digits + 5 decimals + decimal point.

		char e[numdigits + 3]; // 3 because of ", \0"
		snprintf(e, numdigits + 3, "%.5f", val->elements[i]);

		if (i < val->size - 1) {
			strncat(e, ", ", 3);
		}

		strncat(elementsbuffer, e, numdigits + 3);
	}

	// combine all of this into the resulting string 'r'
	char r[maxlen];
	memset(r, 0, maxlen);
	snprintf(r, maxlen, "(vec%d) ( %s )", val->size, elementsbuffer);

	strncpy(str, r, maxlen);
}

/**
 * @brief Takes a matrix value, val, and converts it to a formatted string.
 * 
 * @param val the matrix to format and express as a string.
 * @param str the string to return the value into.
 */
void zmlToStringM(zmlMatrix *val, char *str) {
	// TODO: this function can probably be optimised!

	// getting the amount of characters in the type descriptor to indent each line if necessary
	// 'mat' plus cols + 'x' + rows e.g. mat2x2 (also a space) + brackets
	unsigned int typedescramt = 7 + _zml_getDigitsi(val->cols) + _zml_getDigitsi(val->rows);

	// getting the total maximum length of the string
	// start off with 1 for null terminator.
	unsigned int maxlen = 1;

	// get the total amount of space for just the elements + commas
	for (unsigned int r = 0; r < val->rows; r++) {
		// each number in the row (floating-point so digits + 5 decimal places + decimal point + potential minus sign) + ", " on every one (except the last in the row)
		for (unsigned int c = 0; c < val->cols - 1; c++) {
			maxlen += _zml_getDigitsi(val->elements[r][c]) + 9;
		}
		maxlen += _zml_getDigitsi(val->elements[r][val->cols - 1]) + 7;
	}

	// loop through each row
	for (unsigned int r = 0; r < val->rows; r++) {
		// each row has the indentation of the type descriptor + '[ ' + ' ]' + a comma on each row except the last + newline
		maxlen += typedescramt + 6;
	}

	// start appending to the final string
	char res[maxlen];
	memset(res, 0, maxlen);

	// add the type descriptor first
	snprintf(res, maxlen, "(mat%dx%d) ", val->rows, val->cols);

	// the list of elements is in the format 3, 6, 8, 2, ...
	for (unsigned int r = 0; r < val->rows; r++) {
		// indent every row after the first one (first one has the type descriptor before it anyway)
		for (unsigned int i = 0; i < typedescramt && r > 0; i++) {
			strncat(res, " ", 2);
		}
		
		strncat(res, "[ ", 3);
		for (unsigned int c = 0; c < val->cols; c++) {
			// the amount of digits in the number
			unsigned int numdigits = _zml_getDigitsi(val->elements[r][c]) + 6; // digits + 5 decimals + decimal point.

			// get current element as a string
			// this is like this because it needs to be formatted as %.5f. otherwise I would just concatenate it directly onto res.
			char e[numdigits + 3]; // 3 because of ", \0"
			snprintf(e, numdigits + 3, "%.5f", val->elements[r][c]);

			// append a comma to each element in the row
			if (c < val->cols - 1) {
				strncat(e, ", ", 3);
			}

			// add the element to the elementsbuffer string.
			strncat(res, e, numdigits + 3);
		}
		strncat(res, " ]", 3);

		// add a newline and a comma to every row except the last.
		if (r < val->rows - 1) {
			strncat(res, ",\n", 3);
		}
	}

	// return res
	strncpy(str, res, maxlen);
}

/**
 * @brief Prints the output of zmlToStringV(val) to stdout (with new line!).
 * 
 * @param val the vector to format and print.
 */
void zmlPrintV(zmlVector *val) {
	// get the length of the string
	// yes, this is a mess - it is also done in zmlToStringV() but in a more readable format (and it's commented).
	unsigned int maxlen = 18 + _zml_getDigitsi(val->elements[val->size - 1]) + _zml_getDigitsi(val->size);
	for (unsigned int i = 0; i < val->size - 1; i++) maxlen += _zml_getDigitsi(val->elements[i]) + 9;

	char r[maxlen];
	zmlToStringV(val, r);

	printf("%s\n", r);
}

/**
 * @brief Prints the output of zmlToStringM(val) to stdout (with new line!).
 * 
 * @param val the matrix to format and print.
 */
void zmlPrintM(zmlMatrix *val) {
	// get the length of the string
	// NEVER touch this code. it works fine
	unsigned int maxlen = 1 + (val->rows * ((7 + _zml_getDigitsi(val->cols) + _zml_getDigitsi(val->rows)) + 6));
	for (unsigned int r = 0; r < val->rows; r++) {
		for (unsigned int c = 0; c < val->cols - 1; c++) maxlen += _zml_getDigitsi(val->elements[r][c]) + 9;
		maxlen += _zml_getDigitsi(val->elements[r][val->cols - 1]) + 7;
	}

	char r[maxlen];
	zmlToStringM(val, r);

	// an extra newline is printed due to the more complex nature of the ToStringM output compared to ToStringV.
	// this just organises the output a little better.
	printf("%s\n\n", r);
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