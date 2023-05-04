﻿#include "bgpch.h"
#include "Dense.h"

namespace Layers
{
    Dense::Dense(int inputSize, int outputSize, Core::ActivationType activationType) :
        inputSize(inputSize), outputSize(outputSize), activationType(activationType), biases(outputSize), weights(outputSize, std::vector<double>(inputSize))
    {
        
    }

    void Dense::InitializeWeights()
    {
        for (int i = 0; i < outputSize; i++)
        {
            for (int j = 0; j < inputSize; j++)
            {
                weights[i][j] = Core::Random::NormalDistribution(0, 1);
            }
            biases[i] = Core::Random::NormalDistribution(0, 1);
        }
    }

    std::vector<double> Dense::Forward(std::vector<double> &input)
    {
        Core::Matrix inputMatrix = Core::Matrix(1, input);

        Core::Matrix dotProduct = Core::MatrixMultiplication(inputMatrix, Core::MatrixTransposition(weights));
        Core::Matrix resultMatrix = Core::MatrixAddition(dotProduct, Core::Matrix(1, biases));

        std::vector<double> result = resultMatrix[0];
        return Core::Activation::Apply(result, activationType);
    }

    std::vector<double> Dense::Backward(std::vector<double> &input, const std::vector<double> &gradients, double learningRate)
    {
        std::vector<double> inputActivationDerivative = Core::Activation::ApplyDerivative(input, activationType);
        std::vector<double> newGradients(inputSize, 0);

        for (int i = 0; i < outputSize; i++)
        {
            double delta = gradients[i] * inputActivationDerivative[i];

            for (int j = 0; j < inputSize; j++)
            {
                newGradients[j] += delta * weights[i][j];
                weights[i][j] -= learningRate * delta * input[j];
            }

            biases[i] -= learningRate * delta;
        }

        return newGradients;
    }

}
