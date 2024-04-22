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

void setup() {
    Serial.begin(9600);
    while(!Serial);

    N2Coprocessor coprocessor;
    if(coprocessor.begin())
        Serial.println(F("Co-processor initialized!"));
    else {
        Serial.println(F("Something went wrong. Halting..."));
        while(true);
    }

    if(coprocessor.cpuReset())
        Serial.println(F("CPU Resetted!"));
    else {
        Serial.println(F("Something went wrong. Halting..."));
        while(true);
    }

    Serial.println(F("Initializing neural network..."));
    coprocessor.createNetwork(2, 2, 1);
    coprocessor.setEpochCount(4000);

    float dataset[][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    float output[][1] = {{1}, {1}, {1}, {0}};

    Serial.println(F("Starting network training..."));
    if(coprocessor.train((float*) dataset, (float*) output, 4, 1.0f))
      Serial.println(F("Training done!"));
    else {
        Serial.println(F("Something went wrong. Halting..."));
        while(true);
    }

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

    Serial.println(F("Inference done, resetting network."));
    coprocessor.resetNetwork();
}

void loop() {
    delay(1000);
}
