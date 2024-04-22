/*
 * This file is part of the N2CMU Arduino library (https://github.com/nthnn/n2cmu-arduino).
 * Copyright (c) 2024 Nathanne Isip.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file N2CMU.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for N2CMU (Neural Network Coprocessing Microcontroller Unit).
 *
 * This header file defines the N2Coprocessor class, which provides an interface
 * for interacting with the N2CMU device. It includes functions for initializing,
 * configuring, training, and inferring with neural networks, as well as setting
 * and getting various parameters of the neural network model.
 */
#ifndef N2CMU_H
#define N2CMU_H

#include <SoftwareSerial.h>

#define N2CMU_RX_PIN 6 ///< Pin number for receiving data from N2CMU.
#define N2CMU_TX_PIN 5 ///< Pin number for transmitting data to N2CMU.
#define N2CMU_RESET_TIMEOUT 4558 ///< Timeout duration for resetting N2CMU device.

/**
 * @class N2Coprocessor
 * @brief Class representing the N2CMU device.
 *
 * The N2Coprocessor class provides an interface for controlling and interacting
 * with the N2CMU device. It includes functions for initialization, configuration,
 * training, inference, and parameter manipulation of neural networks.
 */
class N2Coprocessor {
private:
    SoftwareSerial *n2serial; ///< Pointer to SoftwareSerial object for serial communication with N2CMU.

    /**
     * @brief Check the result status of the last operation.
     * @return True if operation was successful, false otherwise.
     */
    bool getResultStatus();

    /**
     * @brief Send a command to the N2CMU device.
     * @param command The command to send.
     * @return True if command was sent successfully, false otherwise.
     */
    bool sendCommand(uint8_t command);

    /**
     * @brief Read a 32-bit floating point number from N2CMU.
     * @return The read floating point number.
     */
    float readF32();

    /**
     * @brief Read an 8-bit unsigned integer from N2CMU.
     * @return The read unsigned integer.
     */
    uint8_t readU8();

    /**
     * @brief Read a 16-bit unsigned integer from N2CMU.
     * @return The read unsigned integer.
     */
    uint16_t readU16();

    /**
     * @brief Write a 32-bit floating point number to N2CMU.
     * @param data The floating point number to write.
     */
    void writeF32(float data);

    /**
     * @brief Write a 16-bit unsigned integer to N2CMU.
     * @param data The unsigned integer to write.
     */
    void writeU16(uint16_t data);

    /**
     * @brief Write data to N2CMU.
     * @param data Pointer to the data to write.
     * @param length Length of the data to write.
     */
    void writeData(const uint8_t *data, uint8_t length);

public:
    /**
     * @brief Constructor for N2Coprocessor class.
     * @param rx Pin number for receiving data from N2CMU.
     * @param tx Pin number for transmitting data to N2CMU.
     */
    N2Coprocessor(
        uint8_t rx = N2CMU_RX_PIN,
        uint8_t tx = N2CMU_TX_PIN
    ): n2serial(new SoftwareSerial(rx, tx)) { }

    /**
     * @brief Initialize the N2CMU device.
     * @return True if initialization was successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Perform handshake with the N2CMU device.
     * @return True if handshake was successful, false otherwise.
     */
    bool handshake();

    /**
     * @brief Reset the CPU of the N2CMU device.
     * @return True if CPU reset was successful, false otherwise.
     */
    bool cpuReset();

    /**
     * @brief Create a neural network with specified input, hidden, and output neuron counts.
     * @param inputCount Number of input neurons.
     * @param hiddenCount Number of hidden neurons.
     * @param outputCount Number of output neurons.
     */
    void createNetwork(
        uint8_t inputCount,
        uint8_t hiddenCount,
        uint8_t outputCount
    );

    /**
     * @brief Train the neural network with provided data and output.
     * @param data Pointer to the input data array.
     * @param output Pointer to the output data array.
     * @param len Length of the data arrays.
     * @param learningRate Learning rate for training.
     * @return True if training was successful, false otherwise.
     */
    bool train(
        float* data,
        float* output,
        uint16_t len,
        float learningRate
    );

    /**
     * @brief Make inference with the neural network using provided input data.
     * @param input Pointer to the input data array.
     * @param output Pointer to store the output data array.
     * @return True if inference was successful, false otherwise.
     */
    bool infer(float* input, float* output);

    /**
     * @brief Reset the neural network parameters.
     */
    void resetNetwork();

    /**
     * @brief Set the number of input neurons.
     * @param inputCount Number of input neurons.
     */
    void setInputCount(uint8_t inputCount);

    /**
     * @brief Set the number of hidden neurons.
     * @param hiddenCount Number of hidden neurons.
     */
    void setHiddenCount(uint8_t hiddenCount);

    /**
     * @brief Set the number of output neurons.
     * @param outputCount Number of output neurons.
     */
    void setOutputCount(uint8_t outputCount);

    /**
     * @brief Set the epoch count for training.
     * @param epoch Epoch count for training.
     */
    void setEpochCount(uint16_t epoch);

    /**
     * @brief Get the number of input neurons.
     * @return Number of input neurons.
     */
    uint8_t getInputCount();

    /**
     * @brief Get the number of hidden neurons.
     * @return Number of hidden neurons.
     */
    uint8_t getHiddenCount();

    /**
     * @brief Get the number of output neurons.
     * @return Number of output neurons.
     */
    uint8_t getOutputCount();

    /**
     * @brief Get the epoch count for training.
     * @return Epoch count for training.
     */
    uint16_t getEpochCount();

    /**
     * @brief Set hidden neuron values.
     * @param hiddenNeuron Array of hidden neuron values.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenNeuron(float* hiddenNeuron);

    /**
     * @brief Set output neuron values.
     * @param outputNeuron Array of output neuron values.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputNeuron(float* outputNeuron);

    /**
     * @brief Set hidden neuron weights.
     * @param hiddenWeights Array of hidden neuron weights.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenWeights(float* hiddenWeights);

    /**
     * @brief Set output neuron weights.
     * @param outputWeights Array of output neuron weights.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputWeights(float* outputWeights);

    /**
     * @brief Set hidden neuron biases.
     * @param hiddenBias Array of hidden neuron biases.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenBias(float* hiddenBias);

    /**
     * @brief Set output neuron biases.
     * @param outputBias Array of output neuron biases.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputBias(float* outputBias);

    /**
     * @brief Set hidden neuron gradients.
     * @param hiddenGrad Array of hidden neuron gradients.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenGradient(float* hiddenGrad);

    /**
     * @brief Set output neuron gradients.
     * @param outputGrad Array of output neuron gradients.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputGradient(float* outputGrad);

    /**
     * @brief Get hidden neuron values.
     * @param hiddenNeuron Array to store hidden neuron values.
     */
    void getHiddenNeuron(float* hiddenNeuron);

    /**
     * @brief Get output neuron values.
     * @param outputNeuron Array to store output neuron values.
     */
    void getOutputNeuron(float* outputNeuron);

    /**
     * @brief Get hidden neuron weights.
     * @param hiddenWeights Array to store hidden neuron weights.
     */
    void getHiddenWeights(float* hiddenWeights);

    /**
     * @brief Get output neuron weights.
     * @param outputWeights Array to store output neuron weights.
     */
    void getOutputWeights(float* outputWeights);

    /**
     * @brief Get hidden neuron biases.
     * @param hiddenBias Array to store hidden neuron biases.
     */
    void getHiddenBias(float* hiddenBias);

    /**
     * @brief Get output neuron biases.
     * @param outputBias Array to store output neuron biases.
     */
    void getOutputBias(float* outputBias);

    /**
     * @brief Get hidden neuron gradients.
     * @param hiddenGrad Array to store hidden neuron gradients.
     */
    void getHiddenGradient(float* hiddenGrad);

    /**
     * @brief Get output neuron gradients.
     * @param outputGrad Array to store output neuron gradients.
     */
    void getOutputGradient(float* outputGrad);
};

#endif