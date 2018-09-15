/*
 * Calculates a root-mean-squared error for the given target & output.
 *
 * Parameters:
 * double *target - The array of target values.
 * double *output - The array of network output values.
 * int size		  - The size of the two arrays.
 */
float calculateError(double *target, double *output, int size);

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
double getOutput(int dataIndex, double *input, int neuronCount, double *centers, double *weights, double width, double *outputValues, double *activationValues);

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
void getOutput(double *input, int inputCount, int neuronCount, double *centers, double *weights, double width, double *outputValues, double *activationValues, double *output);

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
void train(double learningRate, int trainDataCount, double *trainOutput, double *trainActivationValues, double *trainTarget, int neuronCount, double *weights);

/*
 * Determines whether we've converged on a solution by checking if the test error has stopped changing 
 * or the network starts to overfit (seen by the test error starting to increase).
 *
 * Parameters:
 * double *epochRms - The matrix holding the training and testing root-mean-squared error. Stride 2.
 * int epoch		- The current training epoch.
 */
bool converged(float *epochRms, int epoch);