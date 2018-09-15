#include "kmeans.hpp"
#include "network.h"
#include "io.h"

#define EPOCH_NUM	  200
#define LEARNING_RATE 0.02

using namespace std;

int main(int argc, char *argv) {
	// Define normalization constants: maxDayOfYear, maxHour, maxDay
	const double normalizationConstants[] = { 366.0, 24.0, 7.0 };

	// Seed the random number generator so that experiments are comparible.
	srand(10);

	// Read in the training data.
	printf("Loading training data...\n");
	char   *filename	   = "data/train.csv";
	int     trainDataCount = getFileSize(filename);
	double *trainData      = (double*) malloc(sizeof(double) * trainDataCount * 3);
	double *trainTarget    = (double*) malloc(sizeof(double) * trainDataCount);
	double *trainOutput	   = (double*) malloc(sizeof(double) * trainDataCount);
	loadData(filename, normalizationConstants, trainData, trainTarget);

	// Read in the testing data.
	printf("Loading testing data...\n");
	        filename	  = "data/test.csv";
	int     testDataCount = getFileSize(filename);
	double *testData	  = (double*) malloc(sizeof(double) * testDataCount * 3);
	double *testTarget    = (double*) malloc(sizeof(double) * testDataCount);
	double *testOutput	  = (double*) malloc(sizeof(double) * testDataCount);
	loadData(filename, normalizationConstants, testData, testTarget);

	// Read in the validation data.
	printf("Loading validation data...\n");
	filename	  = "data/validation.csv";
	int     validationtDataCount = getFileSize(filename);
	double *validationData	     = (double*) malloc(sizeof(double) * validationtDataCount * 3);
	double *validationTarget     = (double*) malloc(sizeof(double) * validationtDataCount);
	double *validationOutput     = (double*) malloc(sizeof(double) * validationtDataCount);
	loadData(filename, normalizationConstants, validationData, validationTarget);
	
	/*
	  Configuration range:
	  neuronCount 50:10:500      - 46 rows
	  neuronWidth 0.01:0.01:0.1  - 10 columns
	  Total trial configurations - 460
	*/

	// Allocate space for the optimisation results.
	float *optimisationResults = (float*) malloc(sizeof(float) * 46 * 10);

	// Allocate space for the training error.
	int    epochCount = EPOCH_NUM;
	float *epochRms   = (float*) malloc(sizeof(float) * epochCount * 2);

	// Run the optimisation loop.
	int countIndex = 0;
	for (int neuronCount = 50; neuronCount <= 500; neuronCount += 10) {

		// Prellocate space for calculating network output.
		double *weights					   = (double*) malloc(sizeof(double) * neuronCount);
		double *outputValues			   = (double*) malloc(sizeof(double) * neuronCount);
		double *clusterCenters			   = (double*) malloc(sizeof(double) * neuronCount * 3);
		double *testActivationValues	   = (double*) malloc(sizeof(double) * neuronCount * testDataCount);
		double *trainActivationValues	   = (double*) malloc(sizeof(double) * neuronCount * trainDataCount);
		double *validationActivationValues = (double*) malloc(sizeof(double) * neuronCount * validationtDataCount);

		// Allocate memory for kmeans.
		// printf("Allocating memory for kmeans...\n");
		double *clusterEnergies    = (double*) malloc(sizeof(double) * neuronCount);
		int	   *clusterAllocations = (int*)    malloc(sizeof(int)    * trainDataCount);
		int	   *clusterPopulations = (int*)    malloc(sizeof(int)    * neuronCount);

		// Initialise cluster centers to the first few data points.
		printf("\nRunning k-means algorithm...\n");
		for (int i = 0; i < neuronCount; i++) {
			for (int j = 0; j < 3; j++) {
				clusterCenters[i * 3 + j] = trainData[i * 3 + j];
			}
		}

		// Run the kmeans algorithm.
		int iterationCount = 0;
		kmeans_03(3, trainDataCount, neuronCount, 500, iterationCount, trainData, clusterAllocations, clusterCenters, clusterPopulations, clusterEnergies);
		printf("K-means converged in %d iterations.\n", iterationCount);

		// Free the uneeded kmeans variables.
		free(clusterEnergies);
		free(clusterAllocations);
		free(clusterPopulations);

		int widthIndex = 0;
		for (double neuronWidth = 0.01; neuronWidth <= 0.1; neuronWidth += 0.01) {			
			// Randomize the weights matrix.
			randomMatrix(weights, 1, neuronCount, 1);

			// Start the main training loop.
			for (int epoch = 0; epoch < epochCount; epoch++) {
				// Get the network's output for the training data set.
				getOutput(trainData, trainDataCount, neuronCount, clusterCenters, weights, neuronWidth, outputValues, trainActivationValues, trainOutput);

				// Get the network's output for the testing data set.
				getOutput(testData, testDataCount, neuronCount, clusterCenters, weights, neuronWidth, outputValues, testActivationValues, testOutput);

				// Calculate the network error for the training & testing data set.
				epochRms[2 * epoch]		= calculateError(trainTarget, trainOutput, trainDataCount);
				epochRms[2 * epoch + 1] = calculateError(testTarget,  testOutput,  testDataCount);

				// Check if we've converged on a solution.
				if (converged(epochRms, epoch)) {
					epochCount = epoch;
					break;
				}

				// We haven't converged. Keep training the network.
				train(LEARNING_RATE, trainDataCount, trainOutput, trainActivationValues, trainTarget, neuronCount, weights);
			}

			// The network is trained. Get the output for the validation dataset.
			getOutput(validationData, validationtDataCount, neuronCount, clusterCenters, weights, neuronWidth, outputValues, testActivationValues, validationOutput);

			// Calculate the error for the validation dataset.
			float finalError = calculateError(validationTarget, validationOutput, validationtDataCount);
			printf("Count %d\tWidth %.2f\tError %.4f\n", neuronCount, neuronWidth, finalError);

			// Now that we've trained the network, we should save the results.
			optimisationResults[10 * countIndex + widthIndex] = finalError;

			// Reset the epoch count incase we converged and changed the value.
			epochCount = EPOCH_NUM;
			widthIndex++;
		}
		free(weights);
		free(outputValues);
		free(clusterCenters);
		free(testActivationValues);
		free(trainActivationValues);
		free(validationActivationValues);

		countIndex++;
	}

	// Output the optimisation results to a file so that we can plot graphs in another program!
	matrixToFile("results/optimizationResults.txt", optimisationResults, 46, 10);

	// Free the allocated memory.
	free(optimisationResults);
	free(trainTarget);
	free(trainOutput);
	free(testTarget);
	free(testOutput);
	free(validationTarget);
	free(validationOutput);
	free(validationData);
	free(trainData);
	free(testData);
	free(epochRms);
	return 0;
}