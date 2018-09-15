% Load the results from the optimisation process.
data = load("optimisationResults.txt");

% Get the number of different configuation variables.
numCountConfigs = size(data, 1);
numWidthConfigs = size(data, 2);

% As we know the step and scale, create two arrays of the count and width variables.
neuronCounts = zeros(1, numCountConfigs);
for i = 1:numCountConfigs
    neuronCounts(i) = 50 + (i-1) * 10;
end

neuronWidths = zeros(1, numWidthConfigs);
for i = 1:numWidthConfigs
    neuronWidths(i) = 0.01 + (i-1) * 0.01;
end

% Create a matrix holding the values.
results = zeros(numCountConfigs * numWidthConfigs, 3);
for countIndex = 1:numCountConfigs
    for widthIndex = 1:numWidthConfigs
        results((countIndex - 1) * numWidthConfigs + widthIndex, :) = [50 + (countIndex - 1) * 10, 0.01 + (widthIndex - 1) * 0.01, data(countIndex, widthIndex)];
    end
end

% Find the indices of the optimum results.
[minError, indices] = min(data(:));
[countIndex, widthIndex] = ind2sub([numCountConfigs, numWidthConfigs], indices);

fprintf("Best configuration: width %f count %d\n", neuronWidths(widthIndex), neuronCounts(countIndex));

figure;
plot3(results(:, 1), results(:, 2), results(:, 3), 'o');

% Plot the change in neuron count against final network error,
% highlighting the different widths in different colours.
figure;
labels = strings(1, numWidthConfigs-4);
for i = 1:numWidthConfigs-4
    plot(neuronCounts, data(:, i));
    if i == 1
        hold on;
    end
    labels(i) = sprintf("Width %.2f", neuronWidths(i));
end
legend(labels);
title("Effect of node count on accuracy.");
xlabel("node count");
ylabel("Final error");

% Plot the node width against demand.
figure;
plot(results(:, 2), results(:, 3), 'x');
title("Effect of node width on accuracy.");
xlabel("node width");
ylabel("Final error");


% Plot the predicted demand in 2017.
data = load("validationOutput.txt");
target = load("../data/validation.csv");
target = target(:, 4);

figure;
plot(data(:, 1), data(:, 4));
hold on;
plot(data(:, 1), target(:, 1));
xlabel("Day of year");
ylabel("Energy demand");
title("2017 Energy Demand");
legend(["Predicted", "Actual"]);

% die;
% figure;
% plot(data(1:24:168, 1), data(1:24:168, 2));
% title("Week 1");
% hold on;
% plot(data(1:24:168, 1), target(1:24:168, 1));
% hold off;
% xlabel(filename);
% ylabel("Demand");
% legend(["Predicted", "Actual"]);
% 
% figure;
% plot(data(168:24:168*2, 1), data(168:24:168*2, 2));
% title("Week 2");
% hold on;
% plot(data(168:24:168*2, 1), target(168:24:168*2, 1));
% hold off;
% xlabel(filename);
% ylabel("Demand");