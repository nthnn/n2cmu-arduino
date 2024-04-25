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
 * @file n2cmu.h
 * @brief Header file for N2CMU (Neural Network Coprocessing Microcontroller Unit).
 * @author [Nathanne Isip](https://github.com/nthnn)
 * 
 * This header file defines the N2Coprocessor class, providing an interface
 * for interacting with the N2CMU shield device. It includes functions for
 * initializing, configuring, training, and inferring with neural networks,
 * as well as setting and getting various parameters of the neural network
 * model.
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
 * The N2Coprocessor class offers an interface for controlling and interacting
 * with the N2CMU shield device. It encapsulates functions for initialization,
 * configuration, training, inference, and parameter manipulation of neural
 * networks.
 */
class N2Coprocessor {
private:
    SoftwareSerial *n2serial; ///< Pointer to SoftwareSerial object for serial communication with N2CMU.

    /**
     * @brief Checks the result status of the last operation.
     * 
     * This function checks the result status of the last
     * operation performed on the N2CMU device. It returns
     * true if the operation was successful, otherwise false.
     * 
     * @return True if the operation was successful, false otherwise.
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
     * 
     * Constructs a new N2Coprocessor object with
     * the specified RX and TX pins for serial communication.
     * 
     * @param rx Pin number for receiving data from N2CMU.
     * @param tx Pin number for transmitting data to N2CMU.
     */
    N2Coprocessor(
        uint8_t rx = N2CMU_RX_PIN,
        uint8_t tx = N2CMU_TX_PIN
    ): n2serial(new SoftwareSerial(rx, tx)) { }

    /**
     * @brief Initialize the N2CMU device.
     * 
     * This function initializes the N2CMU device by
     * establishing communication and performing
     * necessary setup operations. It must be called
     * before any other operations are performed
     * on the N2CMU device.
     * 
     * @return True if initialization was successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Perform handshake with the N2CMU device.
     * 
     * This function performs a handshake with the N2CMU device
     * to ensure proper communication and synchronization. It
     * verifies the connection and readiness of the N2CMU device for
     * subsequent operations.
     * 
     * @return True if handshake was successful, false otherwise.
     */
    bool handshake();

    /**
     * @brief Reset the CPU of the N2CMU device.
     * 
     * This function resets the CPU of the N2CMU device,
     * restoring it to a known state. It can be useful
     * for recovering from unexpected errors or initializing
     * the device before starting a new operation.
     * 
     * @return True if CPU reset was successful, false otherwise.
     */
    bool cpuReset();

    /**
     * @brief Create a neural network with specified input, hidden, and output neuron counts.
     * 
     * This function creates a neural network model
     * with the specified number of input, hidden,
     * and output neurons. It initializes the network
     * architecture for subsequent training and
     * inference tasks.
     * 
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
     * 
     * This function trains the neural network using
     * the provided input data and corresponding
     * output. It adjusts the network parameters based
     * on the training data to improve its
     * performance and accuracy.
     * 
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
     * 
     * This function performs inference with the
     * trained neural network using the provided
     * input data. It computes the output of the
     * network based on the input and returns the
     * result.
     * 
     * @param input Pointer to the input data array.
     * @param output Pointer to store the output data array.
     * @return True if inference was successful, false otherwise.
     */
    bool infer(float* input, float* output);

    /**
     * @brief Reset the neural network parameters.
     * 
     * This function resets the parameters of the
     * neural network to their initial values.
     * It clears any previously learned weights,
     * biases, or gradients, effectively resetting
     * the network to its initial state.
     */
    void resetNetwork();

    /**
     * @brief Set the number of input neurons.
     * 
     * This function sets the number of input
     * neurons for the neural network. It allows
     * dynamically configuring the input layer
     * of the network to match the requirements
     * of the input data.
     * 
     * @param inputCount Number of input neurons.
     * @note After calling this function, all the network data such as weights, biases, and gradients will return to default 0 values.
     */
    void setInputCount(uint8_t inputCount);

    /**
     * @brief Set the number of hidden neurons.
     * 
     * This function sets the number of hidden
     * neurons for the neural network. It allows
     * dynamically configuring the hidden layer
     * of the network to adjust its capacity and
     * complexity.
     * 
     * @param hiddenCount Number of hidden neurons.
     * @note After calling this function, all the network data such as weights, biases, and gradients will return to default 0 values.
     */
    void setHiddenCount(uint8_t hiddenCount);

    /**
     * @brief Set the number of output neurons.
     * 
     * This function sets the number of output
     * neurons for the neural network. It allows
     * dynamically configuring the output layer
     * of the network to match the requirements
     * of the output data.
     * 
     * @param outputCount Number of output neurons.
     * @note After calling this function, all the network data such as weights, biases, and gradients will return to default 0 values.
     */
    void setOutputCount(uint8_t outputCount);

    /**
     * @brief Set the epoch count for training.
     * 
     * This function sets the number of training
     * epochs for the neural network. An epoch
     * represents one complete pass through the
     * entire training dataset.
     * 
     * @param epoch Epoch count for training.
     */
    void setEpochCount(uint16_t epoch);

    /**
     * @brief Get the number of input neurons.
     * 
     * This function retrieves the number of input
     * neurons currently set for the neural network.
     * 
     * @return Number of input neurons.
     */
    uint8_t getInputCount();

    /**
     * @brief Get the number of hidden neurons.
     * 
     * This function retrieves the number of hidden
     * neurons currently set for the neural network.
     * 
     * @return Number of hidden neurons.
     */
    uint8_t getHiddenCount();

    /**
     * @brief Get the number of output neurons.
     * 
     * This function retrieves the number of output
     * neurons currently set for the neural network.
     * 
     * @return Number of output neurons.
     */
    uint8_t getOutputCount();

    /**
     * @brief Get the epoch count for training.
     * 
     * This function retrieves the number of training
     * epochs currently set for the neural network.
     * 
     * @return Epoch count for training.
     */
    uint16_t getEpochCount();

    /**
     * @brief Set hidden neuron values.
     * 
     * This function sets the values of the
     * hidden neurons in the neural network.
     * It allows specifying the initial values
     * of the hidden neurons before training
     * or inference.
     * 
     * @param hiddenNeuron Array of hidden neuron values.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenNeuron(float* hiddenNeuron);

    /**
     * @brief Set output neuron values.
     * 
     * This function sets the values of the output
     * neurons in the neural network. It allows
     * specifying the initial values of the output
     * neurons before training or inference.
     * 
     * @param outputNeuron Array of output neuron values.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputNeuron(float* outputNeuron);

    /**
     * @brief Set hidden neuron weights.
     * 
     * This function sets the weights of the connections
     * between input and hidden neurons in the neural
     * network. It allows specifying the initial weights
     * of these connections before training or inference.
     * 
     * @param hiddenWeights Array of hidden neuron weights.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenWeights(float* hiddenWeights);

    /**
     * @brief Set output neuron weights.
     * 
     * This function sets the weights of the connections
     * between hidden and output neurons in the neural
     * network. It allows specifying the initial weights
     * of these connections before training or inference.
     * 
     * @param outputWeights Array of output neuron weights.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputWeights(float* outputWeights);

    /**
     * @brief Set hidden neuron biases.
     * 
     * This function sets the biases of the hidden neurons
     * in the neural network. It allows specifying the
     * initial biases of the hidden neurons before training
     * or inference.
     * 
     * @param hiddenBias Array of hidden neuron biases.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenBias(float* hiddenBias);

    /**
     * @brief Set output neuron biases.
     * 
     * This function sets the biases of the output
     * neurons in the neural network. It allows
     * specifying the initial biases of the output
     * neurons before training or inference.
     * 
     * @param outputBias Array of output neuron biases.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputBias(float* outputBias);

    /**
     * @brief Set hidden neuron gradients.
     * 
     * This function sets the gradients of the
     * hidden neurons in the neural network. It
     * allows specifying the initial gradients
     * of the hidden neurons before training or
     * inference.
     * 
     * @param hiddenGrad Array of hidden neuron gradients.
     * @return True if setting was successful, false otherwise.
     */
    bool setHiddenGradient(float* hiddenGrad);

    /**
     * @brief Set output neuron gradients.
     * 
     * This function sets the gradients of the
     * output neurons in the neural network.
     * It allows specifying the initial gradients
     * of the output neurons before training or
     * inference.
     * 
     * @param outputGrad Array of output neuron gradients.
     * @return True if setting was successful, false otherwise.
     */
    bool setOutputGradient(float* outputGrad);

    /**
     * @brief Get hidden neuron values.
     * 
     * This function retrieves the values of
     * the hidden neurons in the neural network.
     * It allows obtaining the current state
     * of the hidden neurons during or after
     * training or inference.
     * 
     * @param hiddenNeuron Array to store hidden neuron values.
     */
    void getHiddenNeuron(float* hiddenNeuron);

    /**
     * @brief Get output neuron values.
     * 
     * This function retrieves the values of
     * the output neurons in the neural network.
     * It allows obtaining the current state
     * of the output neurons during or after
     * training or inference.
     * 
     * @param outputNeuron Array to store output neuron values.
     */
    void getOutputNeuron(float* outputNeuron);

    /**
     * @brief Get hidden neuron weights.
     * 
     * This function retrieves the weights of
     * the connections between input and hidden
     * neurons in the neural network. It allows
     * obtaining the current weights of these
     * connections during or after training or
     * inference.
     * 
     * @param hiddenWeights Array to store hidden neuron weights.
     */
    void getHiddenWeights(float* hiddenWeights);

    /**
     * @brief Get output neuron weights.
     * 
     * This function retrieves the weights of
     * the connections between hidden and output
     * neurons in the neural network. It allows
     * obtaining the current weights of these
     * connections during or after training or
     * inference.
     * 
     * @param outputWeights Array to store output neuron weights.
     */
    void getOutputWeights(float* outputWeights);

    /**
     * @brief Get hidden neuron biases.
     * 
     * This function retrieves the biases
     * of the hidden neurons in the neural
     * network. It allows obtaining the
     * current biases of the hidden neurons
     * during or after training or inference.
     * 
     * @param hiddenBias Array to store hidden neuron biases.
     */
    void getHiddenBias(float* hiddenBias);

    /**
     * @brief Get output neuron biases.
     * 
     * This function retrieves the biases of
     * the output neurons in the neural network.
     * It allows obtaining the current biases
     * of the output neurons during or after
     * training or inference.
     * 
     * @param outputBias Array to store output neuron biases.
     */
    void getOutputBias(float* outputBias);

    /**
     * @brief Get hidden neuron gradients.
     * 
     * This function retrieves the gradients
     * of the hidden neurons in the neural
     * network. It allows obtaining the current
     * gradients of the hidden neurons during
     * or after training.
     * 
     * @param hiddenGrad Array to store hidden neuron gradients.
     */
    void getHiddenGradient(float* hiddenGrad);

    /**
     * @brief Get output neuron gradients.
     * 
     * This function retrieves the gradients
     * of the output neurons in the neural
     * network. It allows obtaining the current
     * gradients of the output neurons during
     * or after training.
     * 
     * @param outputGrad Array to store output neuron gradients.
     */
    void getOutputGradient(float* outputGrad);
};

#endif