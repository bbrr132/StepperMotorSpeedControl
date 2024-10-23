#include "GlassStepper.h"

GlassStepper::GlassStepper(int stepPin, int dirPin, int stepperDriverEnablePin, float pitch, float anglePerStep)
    : stepPin(stepPin), dirPin(dirPin), stepperDriverEnablePin(stepperDriverEnablePin), pitch(pitch), anglePerStep(anglePerStep),
      stepsToMove(0), upDirection(true), stepperPosition(0), movingDistance(0), rpm(0), stepInterval(0), previousStepTimer(0), rotateFlag(0)
{
    moveMotorFlag = false;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(stepperDriverEnablePin, OUTPUT);
    digitalWrite(stepperDriverEnablePin, HIGH);
    digitalWrite(dirPin, HIGH);
}

void GlassStepper::setMoveDistance(double distance)
{
    this->movingDistance = distance;
    convertDistanceToSteps();
}

void GlassStepper::setMotorRPM(float rpm)
{
    this->rpm = rpm;
    float revPerSec = rpm / 60;
    float anglePerSec = revPerSec * 360;
    this->stepInterval = anglePerStep * 1000000 / anglePerSec;
    Serial.println(stepInterval);
}

void GlassStepper::start()
{
    this->rotateFlag = true;
    digitalWrite(stepperDriverEnablePin, LOW);
}

void GlassStepper::rotate()
{
    if (rotateFlag && micros() > previousStepTimer + stepInterval)
    {
        previousStepTimer = micros();
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1);
    }
}
void GlassStepper::stop()
{
    this->rotateFlag = false;
    digitalWrite(stepperDriverEnablePin, HIGH);
}

void GlassStepper::convertDistanceToSteps()
{
    float revolutions = movingDistance / pitch;
    stepsToMove = round(revolutions * 360 / anglePerStep);
    upDirection = (stepsToMove > 0);
    Serial.println(upDirection);
    if (upDirection)
    {
        digitalWrite(dirPin, HIGH);
        Serial.println("HIGH");
    }
    else
    {
        digitalWrite(dirPin, LOW);
        Serial.println("LOW");
    }
}

double GlassStepper::recalculateDistance()
{
    return stepsToMove * anglePerStep / 360.0 * pitch;
}

void GlassStepper::moveMotorByDistance()
{
    digitalWrite(stepperDriverEnablePin, LOW);
    for (int x = 0; x < abs(stepsToMove); x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(500);
    }
    digitalWrite(stepperDriverEnablePin, HIGH);
    stepperPosition += recalculateDistance();
}

void GlassStepper::zeroPosition()
{
    stepperPosition = 0;
}
