#include <Arduino.h>

#include "n2cmu.h"
#include "n2cmu_commands.h"

void N2Coprocessor::writeData(const uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++)
        this->n2serial->write(data[i]);
}

bool N2Coprocessor::getResultStatus() {
    while(!this->n2serial->available());
    return this->n2serial->read() == 1;
}

bool N2Coprocessor::sendCommand(uint8_t command) {
    this->n2serial->write(command);
    return this->getResultStatus();
}

uint8_t N2Coprocessor::readU8() {
    while(this->n2serial->available() != 1);
    return (uint8_t) this->n2serial->read();
}

uint16_t N2Coprocessor::readU16() {
    while(this->n2serial->available() != 2);

    uint8_t array[2];
    array[0] = (uint8_t) this->n2serial->read();
    array[1] = (uint8_t) this->n2serial->read();

    uint16_t num = 0;
    num |= array[0];
    num |= ((uint16_t) array[1]) << 8;

    return num;
}

float N2Coprocessor::readF32() {
    while(this->n2serial->available() != 4);

    float num;
    uint8_t *ptr = (uint8_t*) &num;

    for(uint8_t i = 0; i < 4; i++)
      ptr[i] = this->n2serial->read();

    return num;
}

void N2Coprocessor::writeU16(uint16_t data) {
    uint8_t buf[2];
    buf[0] = (uint8_t)(data & 0xFF);
    buf[1] = (uint8_t)((data >> 8) & 0xFF);

    this->writeData(buf, 2);
}

void N2Coprocessor::writeF32(float data) {
    uint8_t buf[4];
    uint8_t *ptr = (uint8_t*)&data;

    for (uint8_t i = 0; i < 4; i++)
      buf[i] = ptr[i];
    this->writeData(buf, 4);
}

bool N2Coprocessor::begin() {
    this->n2serial->begin(31250);

    while(!this->n2serial);
    return this->handshake();
  }

bool N2Coprocessor::handshake() {
    return this->sendCommand(N2CMU_PROC_HANDSHAKE);
}

bool N2Coprocessor::cpuReset() {
    this->n2serial->write(N2CMU_PROC_CPU_RESET);
    delayMicroseconds(N2CMU_RESET_TIMEOUT);

    return this->handshake();
}

void N2Coprocessor::createNetwork(
    uint8_t inputCount,
    uint8_t hiddenCount,
    uint8_t outputCount
) {
    const uint8_t data[] = {
        N2CMU_NET_CREATE,
        inputCount,
        hiddenCount,
        outputCount
        };

    this->writeData(data, 4);
}

bool N2Coprocessor::train(
    float* data,
    float* output,
    uint16_t len,
    float learningRate
) {
    if(this->getEpochCount() == 0)
        return false;

    uint8_t inputCount = this->getInputCount();
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_NET_TRAIN);
    this->writeU16(len);

    for(uint8_t j = 0; j < len; j++)
        for(uint8_t k = 0; k < inputCount; k++)
            this->writeF32(data[j * inputCount + k]);

    for(uint8_t j = 0; j < len; j++)
        for(uint8_t k = 0; k < outputCount; k++)
            this->writeF32(output[j * outputCount + k]);

    this->writeF32(learningRate);
    return this->getResultStatus();
}

bool N2Coprocessor::infer(float* input, float* output) {
    uint8_t inputCount = this->getInputCount();
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_NET_INFER);
    for(uint8_t i = 0; i < inputCount; i++)
        this->writeF32(input[i]);

    for(uint8_t j = 0; j < outputCount; j++)
        output[j] = this->readF32();

    return this->getResultStatus();
}

void N2Coprocessor::resetNetwork() {
    this->n2serial->write(N2CMU_NET_RESET);
}

void N2Coprocessor::setInputCount(uint8_t inputCount) {
    const uint8_t data[] = {
        N2CMU_SET_INPUT_COUNT,
        inputCount
    };

    this->writeData(data, 2);
}

uint8_t N2Coprocessor::getInputCount() {
    this->n2serial->write(N2CMU_GET_INPUT_COUNT);
    return this->readU8();
}

void N2Coprocessor::setHiddenCount(uint8_t hiddenCount) {
    const uint8_t data[] = {
        N2CMU_SET_HIDDEN_COUNT,
        hiddenCount
    };

    this->writeData(data, 2);
}

uint8_t N2Coprocessor::getHiddenCount() {
    this->n2serial->write(N2CMU_GET_HIDDEN_COUNT);
    return this->readU8();
}

void N2Coprocessor::setOutputCount(uint8_t outputCount) {
    const uint8_t data[] = {
        N2CMU_SET_OUTPUT_COUNT,
        outputCount
    };

    this->writeData(data, 2);
}

uint8_t N2Coprocessor::getOutputCount() {
    this->n2serial->write(N2CMU_GET_OUTPUT_COUNT);
    return this->readU8();
}

void N2Coprocessor::setEpochCount(uint16_t epoch) {
    this->n2serial->write(N2CMU_SET_EPOCH_COUNT);
    this->writeU16(epoch);
}

uint16_t N2Coprocessor::getEpochCount() {
    this->n2serial->write(N2CMU_GET_EPOCH_COUNT);
    return this->readU16();
}

bool N2Coprocessor::setHiddenNeuron(float* hiddenNeuron) {
    uint8_t hiddenCount = this->getHiddenCount();

    this->n2serial->write(N2CMU_SET_HIDDEN_NEURON);
    for(uint8_t i = 0; i < hiddenCount; i++)
        this->writeF32(hiddenNeuron[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getHiddenNeuron(float* hiddenNeuron) {
    uint8_t hiddenCount = this->getHiddenCount();

    this->n2serial->write(N2CMU_GET_HIDDEN_NEURON);
    for(uint8_t i = 0; i < hiddenCount; i++)
        hiddenNeuron[i] = this->readF32();
}

bool N2Coprocessor::setOutputNeuron(float* outputNeuron) {
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_SET_OUTPUT_NEURON);
    for(uint8_t i = 0; i < outputCount; i++)
        this->writeF32(outputNeuron[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getOutputNeuron(float* outputNeuron) {
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_GET_OUTPUT_NEURON);
    for(uint8_t i = 0; i < outputCount; i++)
        outputNeuron[i] = this->readF32();
}

bool N2Coprocessor::setHiddenWeights(float* hiddenWeights) {
    uint8_t count = this->getInputCount() *
        this->getHiddenCount();

    this->n2serial->write(N2CMU_SET_HIDDEN_WEIGHTS);
    for(uint8_t i = 0; i < count; i++)
        this->writeF32(hiddenWeights[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getHiddenWeights(float* hiddenWeights) {
    uint8_t count = this->getInputCount() *
        this->getHiddenCount();

    this->n2serial->write(N2CMU_GET_HIDDEN_WEIGHTS);
    for(uint8_t i = 0; i < count; i++)
        hiddenWeights[i] = this->readF32();
}

bool N2Coprocessor::setOutputWeights(float* outputWeights) {
    uint8_t count = this->getHiddenCount() *
        this->getOutputCount();

    this->n2serial->write(N2CMU_SET_OUTPUT_WEIGHTS);
    for(uint8_t i = 0; i < count; i++)
        this->writeF32(outputWeights[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getOutputWeights(float* outputWeights) {
    uint8_t count = this->getHiddenCount() *
        this->getOutputCount();

    this->n2serial->write(N2CMU_GET_OUTPUT_WEIGHTS);
    for(uint8_t i = 0; i < count; i++)
        outputWeights[i] = this->readF32();
}

bool N2Coprocessor::setHiddenBias(float* hiddenBias) {
    uint8_t hiddenCount = this->getHiddenCount();

    this->n2serial->write(N2CMU_SET_HIDDEN_BIAS);
    for(uint8_t i = 0; i < hiddenCount; i++)
        this->writeF32(hiddenBias[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getHiddenBias(float* hiddenBias) {
    uint8_t hiddenCount = this->getHiddenCount();

    this->n2serial->write(N2CMU_GET_HIDDEN_BIAS);
    for(uint8_t i = 0; i < hiddenCount; i++)
        hiddenBias[i] = this->readF32();
}

bool N2Coprocessor::setOutputBias(float* outputBias) {
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_SET_OUTPUT_BIAS);
    for(uint8_t i = 0; i < outputCount; i++)
        this->writeF32(outputBias[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getOutputBias(float* outputBias) {
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_GET_OUTPUT_BIAS);
    for(uint8_t i = 0; i < outputCount; i++)
        outputBias[i] = this->readF32();
}

bool N2Coprocessor::setHiddenGradient(float* hiddenGrad) {
    uint8_t hiddenCount = this->getHiddenCount();

    this->n2serial->write(N2CMU_SET_HIDDEN_GRAD);
    for(uint8_t i = 0; i < hiddenCount; i++)
        this->writeF32(hiddenGrad[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getHiddenGradient(float* hiddenGrad) {
    uint8_t hiddenCount = this->getHiddenCount();

    this->n2serial->write(N2CMU_GET_HIDDEN_GRAD);
    for(uint8_t i = 0; i < hiddenCount; i++)
        hiddenGrad[i] = this->readF32();
}

bool N2Coprocessor::setOutputGradient(float* outputGrad) {
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_SET_OUTPUT_GRAD);
    for(uint8_t i = 0; i < outputCount; i++)
        this->writeF32(outputGrad[i]);

    return this->getResultStatus();
}

void N2Coprocessor::getOutputGradient(float* outputGrad) {
    uint8_t outputCount = this->getOutputCount();

    this->n2serial->write(N2CMU_GET_OUTPUT_GRAD);
    for(uint8_t i = 0; i < outputCount; i++)
        outputGrad[i] = this->readF32();
}