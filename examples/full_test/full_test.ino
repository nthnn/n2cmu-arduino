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

#include <n2cmu.h>

void testFeedbackResult(String message, bool result) {
    Serial.print(message);

    if(result)
        Serial.println(F("done!"));
    else {
        Serial.println(F("error!"));
        while(true);
    }
}

void setup() {
    Serial.begin(9600);
    while(!Serial);

    N2Coprocessor coprocessor;
    testFeedbackResult(
        F("Initializing N2 co-processor..."),
        coprocessor.begin()
    );

    testFeedbackResult(
        F("Soft resetting co-processor CPU... "),
        coprocessor.cpuReset()
    );

    testFeedbackResult(
        F("Handshaking to co-processor... "),
        coprocessor.handshake()
    );

    Serial.println(F("Creating neural network..."));
    coprocessor.createNetwork(1, 1, 2);

    Serial.print(F("Network input count: "));
    Serial.println(coprocessor.getInputCount());

    Serial.println(F("Setting network input count to 2"));
    coprocessor.setInputCount(2);

    Serial.print(F("Network input count: "));
    Serial.println(coprocessor.getInputCount());

    Serial.print(F("Network hidden count: "));
    Serial.println(coprocessor.getHiddenCount());

    Serial.println(F("Setting network hidden count to 2"));
    coprocessor.setHiddenCount(2);

    Serial.print(F("Network hidden count: "));
    Serial.println(coprocessor.getHiddenCount());

    Serial.print(F("Network output count: "));
    Serial.println(coprocessor.getOutputCount());

    Serial.println(F("Setting network output count to 1"));
    coprocessor.setOutputCount(1);

    Serial.print(F("Network output count: "));
    Serial.println(coprocessor.getOutputCount());

    Serial.println(F("Setting epoch count to 4000... "));
    coprocessor.setEpochCount(4000);

    Serial.print(F("Training epoch count was set to: "));
    Serial.println(coprocessor.getEpochCount());

    float dataset[][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    float output[][1] = {{0}, {0}, {0}, {1}};
  
    testFeedbackResult(
        F("Starting data set training... "),
        coprocessor.train((float*) dataset, (float*) output, 4, 1.0f)
    );

    Serial.println(F("Attempting inferences..."));
    for(uint8_t i = 0; i < 4; i++) {
        float output[1];

        if(coprocessor.infer(dataset[i], output)) {
            Serial.print(F("\t["));
            Serial.print(dataset[i][0]);
            Serial.print(F(", "));
            Serial.print(dataset[i][1]);
            Serial.print(F("]: "));
            Serial.println(output[0]);
        }
        else Serial.println(F("Inference attemp failed."));
    }

    float hiddenNeuron[2];
    coprocessor.getHiddenNeuron(hiddenNeuron);

    Serial.print(F("Hidden Neurons: ["));
    Serial.print(hiddenNeuron[0]);
    Serial.print(F(" "));
    Serial.print(hiddenNeuron[1]);
    Serial.println(F("]"));

    float newHiddenNeuron[] = {3.14, 6.28};
    testFeedbackResult(
        F("Updating new hidden neurons... "),
        coprocessor.setHiddenNeuron(newHiddenNeuron)
    );
    coprocessor.getHiddenNeuron(hiddenNeuron);

    Serial.print(F("New Hidden Neurons: ["));
    Serial.print(hiddenNeuron[0]);
    Serial.print(F(" "));
    Serial.print(hiddenNeuron[1]);
    Serial.println(F("]"));

    float outputNeuron[1];
    coprocessor.getOutputNeuron(outputNeuron);

    Serial.print(F("Output Neurons: ["));
    Serial.print(outputNeuron[0]);
    Serial.println(F("]"));

    float newOutputNeuron[] = {0.12};
    testFeedbackResult(
        F("Updating new output neurons... "),
        coprocessor.setOutputNeuron(newOutputNeuron)
    );
    coprocessor.getOutputNeuron(outputNeuron);

    Serial.print(F("New Output Neurons: ["));
    Serial.print(outputNeuron[0]);
    Serial.println(F("]"));

    float hiddenWeights[4];
    coprocessor.getHiddenWeights(hiddenWeights);

    Serial.print(F("Hidden Weights: ["));
    Serial.print(hiddenWeights[0]);
    Serial.print(F(" "));
    Serial.print(hiddenWeights[1]);
    Serial.print(F(" "));
    Serial.print(hiddenWeights[2]);
    Serial.print(F(" "));
    Serial.print(hiddenWeights[3]);
    Serial.println(F("]"));

    float newHiddenWeights[] = {1.0, 2.0, 3.0, 4.0};
    testFeedbackResult(
        F("Updating new hidden weights... "),
        coprocessor.setHiddenWeights(newHiddenWeights)
    );
    coprocessor.getHiddenWeights(hiddenWeights);

    Serial.print(F("New Hidden Weights: ["));
    Serial.print(hiddenWeights[0]);
    Serial.print(F(" "));
    Serial.print(hiddenWeights[1]);
    Serial.print(F(" "));
    Serial.print(hiddenWeights[2]);
    Serial.print(F(" "));
    Serial.print(hiddenWeights[3]);
    Serial.println(F("]"));

    float outputWeights[2];
    coprocessor.getOutputWeights(outputWeights);

    Serial.print(F("Output Weights: ["));
    Serial.print(outputWeights[0]);
    Serial.print(F(" "));
    Serial.print(outputWeights[1]);
    Serial.println(F("]"));

    float newOutputWeights[] = {1.0, 2.0};

    testFeedbackResult(
        F("Updating new output weights... "),
        coprocessor.setOutputWeights(newOutputWeights)
    );
    coprocessor.getOutputWeights(outputWeights);

    Serial.print(F("New Output Weights: ["));
    Serial.print(outputWeights[0]);
    Serial.print(F(" "));
    Serial.print(outputWeights[1]);
    Serial.println(F("]"));

    float hiddenBias[2];
    coprocessor.getHiddenBias(hiddenBias);

    Serial.print(F("Hidden Bias: ["));
    Serial.print(hiddenBias[0]);
    Serial.print(F(" "));
    Serial.print(hiddenBias[1]);
    Serial.println(F("]"));

    float newHiddenBias[] = {2.0, 1.0};
    testFeedbackResult(
        F("Updating new hidden biases... "),
        coprocessor.setHiddenBias(newHiddenBias)
    );
    coprocessor.getHiddenBias(hiddenBias);

    Serial.print(F("New Hidden Bias: ["));
    Serial.print(hiddenBias[0]);
    Serial.print(F(" "));
    Serial.print(hiddenBias[1]);
    Serial.println(F("]"));

    float outputBias[1];
    coprocessor.getOutputBias(outputBias);

    Serial.print(F("Output Bias: ["));
    Serial.print(outputBias[0]);
    Serial.println(F("]"));

    float newOutputBias[] = {0.5};
    testFeedbackResult(
        F("Updating new output biases... "),
        coprocessor.setOutputBias(newOutputBias)
    );
    coprocessor.getOutputBias(outputBias);

    Serial.print(F("New Output Bias: ["));
    Serial.print(outputBias[0]);
    Serial.println(F("]"));

    float hiddenGrad[2];
    coprocessor.getHiddenGradient(hiddenGrad);

    Serial.print(F("Hidden Gradient: ["));
    Serial.print(hiddenGrad[0]);
    Serial.print(F(" "));
    Serial.print(hiddenGrad[1]);
    Serial.println(F("]"));

    float newHiddenGrad[] = {2.2, 1.1};
    testFeedbackResult(
        F("Updating new hidden gradient... "),
        coprocessor.setHiddenGradient(newHiddenGrad)
    );
    coprocessor.getHiddenGradient(hiddenGrad);

    Serial.print(F("New Hidden Gradient: ["));
    Serial.print(hiddenGrad[0]);
    Serial.print(F(" "));
    Serial.print(hiddenGrad[1]);
    Serial.println(F("]"));

    float outputGrad[1];
    coprocessor.getOutputGradient(outputGrad);

    Serial.print(F("Output Gradient: ["));
    Serial.print(outputGrad[0]);
    Serial.println(F("]"));

    float newOutputGrad[] = {0.3};
    testFeedbackResult(
        F("Updating new output gradientes... "),
        coprocessor.setOutputGradient(newOutputGrad)
    );
    coprocessor.getOutputGradient(outputGrad);

    Serial.print(F("New Output Gradient: ["));
    Serial.print(outputGrad[0]);
    Serial.println(F("]"));

    Serial.println(F("Resetting neural network... "));
    coprocessor.resetNetwork();
}

void loop() {
    delay(1000);
}
