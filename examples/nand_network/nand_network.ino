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
