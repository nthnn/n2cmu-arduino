#ifndef N2CMU_H
#define N2CMU_H

#include <SoftwareSerial.h>

#define N2CMU_RX_PIN 6
#define N2CMU_TX_PIN 5
#define N2CMU_RESET_TIMEOUT 4558

class N2Coprocessor {
private:
    SoftwareSerial *n2serial;

    bool getResultStatus();
    bool sendCommand(uint8_t command);

    float readF32();
    uint8_t readU8();
    uint16_t readU16();

    void writeF32(float data);
    void writeU16(uint16_t data);
    void writeData(const uint8_t *data, uint8_t length);

public:
    N2Coprocessor(
        uint8_t rx = N2CMU_RX_PIN,
        uint8_t tx = N2CMU_TX_PIN
    ): n2serial(new SoftwareSerial(rx, tx)) { }

    bool begin();
    bool handshake();
    bool cpuReset();

    void createNetwork(
        uint8_t inputCount,
        uint8_t hiddenCount,
        uint8_t outputCount
    );

    bool train(
        float* data,
        float* output,
        uint16_t len,
        float learningRate
    );

    bool infer(float* input, float* output);
    void resetNetwork();

    void setInputCount(uint8_t inputCount);
    void setHiddenCount(uint8_t hiddenCount);
    void setOutputCount(uint8_t outputCount);
    void setEpochCount(uint16_t epoch);

    uint8_t getInputCount();
    uint8_t getHiddenCount();
    uint8_t getOutputCount();
    uint16_t getEpochCount();

    bool setHiddenNeuron(float* hiddenNeuron);
    bool setOutputNeuron(float* outputNeuron);
    bool setHiddenWeights(float* hiddenWeights);
    bool setOutputWeights(float* outputWeights);
    bool setHiddenBias(float* hiddenBias);
    bool setOutputBias(float* outputBias);
    bool setHiddenGradient(float* hiddenGrad);
    bool setOutputGradient(float* outputGrad);

    void getHiddenNeuron(float* hiddenNeuron);
    void getOutputNeuron(float* outputNeuron);
    void getHiddenWeights(float* hiddenWeights);
    void getOutputWeights(float* outputWeights);
    void getHiddenBias(float* hiddenBias);
    void getOutputBias(float* outputBias);
    void getHiddenGradient(float* hiddenGrad);
    void getOutputGradient(float* outputGrad);
};

#endif