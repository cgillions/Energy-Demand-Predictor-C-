#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

/*
* Returns the number of lines in the file specified by filename.
*
* Parameters:
* char *filename - The name of the file to count lines from.
*/
int getFileSize(char *filename) {
	// Open the file.
	ifstream data(filename);
	string   line;

	// Iterate through the lines.
	int count = 0;
	while (getline(data, line)) {
		count++;
	}
	return count;
}

/*
* Loads the input and target data from the file specified by filename into the relevant arrays,
* normalizing the data as it is loaded.
*
* Parameters:
* char *filename						- The name of the file to load from.
* const double *normalizationConstants	- Constants used to normalize the input data. The length of 
*										  this array should match the dimension of the input data.
* double *inputData					- A preallocated array to hold the input data.
* double *target						- A preallocated array to hold the target data.
*/
void loadData(char *filename, const double *normalizationConstants, double *resultingData, double *target) {
	// Open the file.
	ifstream data(filename);
	string   line;

	// Get each line from the file.
	int lineCount = 0;
	while (getline(data, line)) {

		// Get the elements from each line.
		int colCount = 0;
		size_t start = 0, end;
		while (start != string::npos) {

			// Find next occurence of ','.
			end = line.find(',', start);

			// Substring the text and store the (normalized) integer.
			int value = atoi(line.substr(start, end - start).c_str());

			if (colCount == 3) {
				target[lineCount] = value;
			} else {
				resultingData[lineCount * 3 + colCount] = value / normalizationConstants[colCount];
			}

			// Find the new starting position.
			start = line.find_first_not_of(',', end);
			colCount++;
		}
		lineCount++;
	}
}

/*
 * Generates a random matrix of the given size, where values are between 0 & max.
 *
 * Parameters:
 * double *matrix - A preallocated matrix to populate.
 * int rows		  - The number of rows in the matrix.
 * int columns	  - The number of columns in the matrix.
 * double max	  - The maximum value of the normal distribution.
 */
void randomMatrix(double *matrix, int rows, int columns, double max) {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < columns; c++) {
			matrix[r * columns + c] = rand() / double(RAND_MAX) * max;
		}
	}
}

/*
 * Prints the values of a matrix.
 *
 * Parameters:
 * double *matrix - The matrix to print.
 * int rows		  - The number of rows in the matrix.
 * int columns	  - The number of columns in the matrix.
 */
void printMatrix(double *matrix, int rows, int columns) {
	for (int row = 0; row < rows; row++) {
		printf("Row %d:\t", row + 1);
		for (int col = 0; col < columns; col++) {
			printf("%.4f\t", matrix[row * columns + col]);
		}
		printf("\n");
	}
}

/*
 * Saves the input data and output of the network to a text file.
 *
 * Parameters:
 * char *filename				  - The name of the file to write the data to.
 * double *input				  - The data that was fed to the network.
 * int rows						  - The number of rows in the input matrix and output array.
 * int columns					  - The number of columns in the input matrix.
 * double *normalizationConstants - The values used to normalise the input matrix.
 * double *output				  - The output of the network for the given input matrix.
 */
void ioToFile(char *filename, double *input, int rows, int columns, const double *normalizationConstants, double *output) {
	// Create a string stream that we can append to.
	ostringstream outputString;

	// Iterate through the data.
	for (int dataIndex = 0; dataIndex < rows; dataIndex++) {
		int rowIndex = dataIndex * columns;

		// Iterate through the columns.
		for (int colIndex = 0; colIndex < columns; colIndex++) {

			// Append the input to the string.
			outputString << input[rowIndex + colIndex] * normalizationConstants[colIndex] << "\t";
		}

		// Append the output to the string.
		outputString << output[dataIndex] << "\n";
	}

	// Open the output file, deleting any previous content.
	ofstream outputFile;
	outputFile.open(filename, ios::trunc);

	// Output the string to the file.
	outputFile << outputString.str();

	// Close the file.
	outputFile.close();
}

/*
 * Saves the input data to a text file.
 *
 * Parameters:
 * char *filename - The name of the file to write the data to.
 * double *input  - The data that was fed to the network.
 * int rows		  - The number of rows in the input matrix and output array.
 * int columns	  - The number of columns in the input matrix.
 */
void matrixToFile(char *filename, float *input, int rows, int columns) {
	// Create a string stream that we can append to.
	ostringstream outputString;

	// Iterate through the data.
	for (int dataIndex = 0; dataIndex < rows; dataIndex++) {
		int rowIndex = dataIndex * columns;

		// Iterate through the columns.
		for (int colIndex = 0; colIndex < columns; colIndex++) {

			// Append the input to the string.
			outputString << input[rowIndex + colIndex] << "\t";
		}

		// Start a new line.
		outputString << "\n";
	}

	// Open the output file, deleting any previous content.
	ofstream outputFile;
	outputFile.open(filename, ios::trunc);

	// Output the string to the file.
	outputFile << outputString.str();

	// Close the file.
	outputFile.close();
}