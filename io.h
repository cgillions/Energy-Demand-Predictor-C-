/*
 * Returns the number of lines in the file specified by filename.
 *
 * Parameters:
 * char *filename -	The name of the file to count lines from.
 */
int	getFileSize(char *filename);

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
void loadData(char *filename, const double *normalizationConstants, double *inputData, double *target);

/*
 * Generates a random matrix of the given size, where values are between 0 & max.
 *
 * Parameters:
 * double *matrix - A preallocated matrix to populate.
 * int rows		  - The number of rows in the matrix.
 * int columns	  - The number of columns in the matrix.
 * double max	  - The maximum value of the normal distribution.
 */
void randomMatrix(double *matrix, int rows, int columns, double max);

/*
 * Prints the values of a matrix.
 *
 * Parameters:
 * double *matrix - The matrix to print.
 * int rows		  - The number of rows in the matrix.
 * int columns	  - The number of columns in the matrix.
 */
void printMatrix(double *matrix, int rows, int columns);

/*
 * Saves the input data and output of the network to a text file.
 *
 * Parameters:
 * char *filename				  - The name of the file to write the data to.
 * double * input				  - The data that was fed to the network.
 * int rows						  - The number of rows in the input matrix and output array.
 * int columns					  - The number of columns in the input matrix.
 * double *normalizationConstants - The values used to normalise the input matrix.
 * double *output				  - The output of the network for the given input matrix.
 */
void ioToFile(char *filename, double *input, int rows, int columns, const double *normalizationConstants, double *output);

/*
 * Saves the input data to a text file.
 *
 * Parameters:
 * char *filename - The name of the file to write the data to.
 * double *input  - The data that was fed to the network.
 * int rows		  - The number of rows in the input matrix and output array.
 * int columns	  - The number of columns in the input matrix.
 */
void matrixToFile(char *filename, float *input, int rows, int columns);