#include <Arduino.h>
#include "Arduino.h"
#include "MitMotor.h"
#include "RmdMotor.h"
#include "array"
#include <algorithm>

constexpr float PERIOD_USEC = 1000;
constexpr float T = PERIOD_USEC / 1000000.0;

#define CS_0 1
#define INT_0 0

CanMotor *motors[] = {
    new RmdMotor(RmdMotor::RMD_X6, CS_0, INT_0, "MOTOR_1", SPI, false)
};

constexpr size_t NUM_MOTORS = sizeof(motors) / sizeof(motors[0]);

void (*interrupt_handlers[NUM_MOTORS])() = 
{
    [](){motors[0]->handleInterrupt();}
};

void setup()
{
    Serial.begin(115200);
    SPI.begin();
    delay(500);

    for (auto &motor : motors)
    {
        while (!motor->initialize())
        {
            Serial.print("Retrying to initialize ");
            Serial.print(motor->name());
            Serial.print(" MCP2515");
        }
    }
    Serial.println("All motors initialized succesfully");

    Serial.print("Tiempo de muestreo: ");
    Serial.println(T, 10);
}

void controlMotors()
{
    for (uint8_t i = 0; i < NUM_MOTORS; i++)
    {
        if (!motors[i]->setTorque(0, 2000))
        {
            Serial.print("Message NOT Sent to "); Serial.println(motors[i]->name());
        }
        if (!motors[i]->readMotorResponse(2000))
        {
          Serial.print("No response pending in "); Serial.print(motors[i]->name()); Serial.println("MCP2515 Buffer");
        }
    }
}

void loop(){
  Serial.print("Imprimiendo ando\n");
  
}
