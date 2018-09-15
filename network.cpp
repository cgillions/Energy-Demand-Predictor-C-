#include <stdio.h>
#include <cmath>
#include <float.h>

using namespace std;

/*
 * Calculates the sum of a single-dimension array.
 *
 * Parameters:
 * double *vector - The array to sum.
 * int size		  - The size of the array.
 */
double sum(double *vector, int size);

/*
 * Calculates a root-mean-squared error for the given target & output.
 *
 * Parameters:
 * double *target - The array of target values.
 * double *output - The array of network output values.
 * int size		  - The size of the two arrays.
 */
float calculateError(double *target, double *output, int size) {
	float rms = 0.0f;
	for (int dataIndex = 0; dataIndex < size; dataIndex++) {
		rms += pow(target[dataIndex] - output[dataIndex], 2);
	}	
	return sqrt(rms / size);
}

/*
 * Calculates the output of the network for a single input data point.
 * 
 * Parameters:
 * double *input			- One data point, which is an array of 3 values.
 * int neuronCount			- The number of RBF neurons in the network.
 * double *centers			- An matrix of centers, size neuronCount x 3. Stride 3.
 * double *weights			- An array of weights, size neuronCount.
 * double width				- The width of each RBF neuron.
 * double *outputValues		- An array to hold a partial sum for the output. Re-use for performance.
 * double *activationValues - An array to hold a partial sum for the output. Re-use for performance.
 */
double getOutput(int dataIndex, double *input, int neuronCount, double *centers, double *weights, double width, double *outputValues, double *activationValues) {
	double activationSum = 0, activationValue;
	for (int neuronIndex = 0; neuronIndex < neuronCount; neuronIndex++) {
		// Get the center values for the neuron.
		int index = neuronIndex * 3;
		double neuronCenter[] = { centers[index], centers[index + 1], centers[index + 2] };

		// Find the squared difference of the input compared to the neuron centers.
		double squareDiffs[] = { pow(neuronCenter[0] - input[0], 2), pow(neuronCenter[1] - input[1], 2), pow(neuronCenter[2] - input[2], 2) };

		// Calculate the activation value for the neuron by multiplying together the activation value for each dimension.
		activationValue = 1;
		for (int i = 0; i < 3; i++) {
			activationValue *= exp(-squareDiffs[i] / (2 * pow(width, 2)));
		}

		// Store the activation and output value for this neuron and increase the total activation sum.
		activationSum += activationValues[dataIndex * neuronCount + neuronIndex] = activationValue;
		outputValues[neuronIndex] = activationValue * weights[neuronIndex];
	}

	// Now we have the activation & weighted output for each neuron, calculate the normalized network output.
	return sum(outputValues, neuronCount) / activationSum;
}

/*
 * Calculates the output of the network for an input data matrix.
 *
 * Parameters:
 * double *input			- An matrix of inputCount data points, where each point is an array of 3 values. Stride 3.
 * int inputCount			- The number of input data points.
 * int neuronCount			- The number of RBF neurons in the network.
 * double *centers			- An matrix of centers, size neuronCount x 3. Stride 3.
 * double *weights			- An array of weights, size neuronCount.
 * double width				- The width of each RBF neuron.
 * double *outputValues		- An array to hold a partial sum for the output. Re-use for performance.
 * double *activationValues - An array to hold a partial sum for the output. Re-use for performance.
 * double *output			- A preallocated array to hold the result. Length is inputCount.
 */
void getOutput(double *input, int inputCount, int neuronCount, double *centers, double *weights, double width, double *outputValues, double *activationValues, double *output) {
	for (int dataIndex = 0; dataIndex < inputCount; dataIndex++) {
		int dIndex = dataIndex * 3;
		double dataPoint[] = { input[dIndex], input[dIndex + 1], input[dIndex + 2] };
		output[dataIndex]  = getOutput(dataIndex, dataPoint, neuronCount, centers, weights, width, outputValues, activationValues);
	}
}

/*
 * Updates the weights of a network based on the difference between network output & target output.
 * 
 * Parameters:
 * double learningRate			 - The learning rate of the network.
 * int trainDataCount			 - The number of training data points.
 * double *trainOutput			 - A preallocated array to hold the output of the network.
 * double *trainActivationValues - A preallocated array to hold a partial sum for training.
 * double *trainTarget			 - The target array.
 * int neuronCount				 - The number of neurons in the network.
 * double *weights				 - The weight array for the network.
 */
void train(double learningRate, int trainDataCount, double *trainOutput, double *trainActivationValues, double *trainTarget, int neuronCount, double *weights) {
	for (int neuronIndex = 0; neuronIndex < neuronCount; neuronIndex++) {
		for (int dataIndex = 0; dataIndex < trainDataCount; dataIndex++) {
			weights[neuronIndex] += learningRate * (trainTarget[dataIndex] - trainOutput[dataIndex]) * trainActivationValues[dataIndex * neuronCount + neuronIndex];
		}
	}
}

/*
 * Determines whether we've converged on a solution by checking
 * 1) has the train error stopped changing (difference in error values negligible).
 * 2) has the network started to overfit (train or test error starts to increase).
 *
 * Parameters:
 * double *epochRms - The matrix holding the training and testing root-mean-squared error. Stride 2.
 * int epoch		- The current training epoch.
 */
bool converged(float *epochRms, int epoch) {
	epoch *= 2;
	return epoch >= 4 
		&& ((abs(epochRms[epoch - 2] - epochRms[epoch]) < 0.0001 
			&& abs(epochRms[epoch - 4] - epochRms[epoch - 2]) < 0.0001)
		||   (epochRms[epoch - 1] < epochRms[epoch + 1]
			|| epochRms[epoch - 2] < epochRms[epoch]));
}

/*
 * Calculates the sum of a single-dimension vector.
 *
 * Parameters:
 * double *vector - The array to sum.
 * int size		  - The size of the array.
 */
double sum(double *vector, int size) {
	double output = 0;
	for (int i = 0; i < size; i++) {
		output += vector[i];
	}
	return output;
}