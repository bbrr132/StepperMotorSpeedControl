#ifndef GLASS_STEPPER_H
#define GLASS_STEPPER_H

#include <Arduino.h>

class GlassStepper {
public:
    GlassStepper(int stepPin, int dirPin, int stepperDriverEnablePin, float pitch, float anglePerStep);
    
    void setMoveDistance(double distance);  // Sets the desired move distance
    void moveMotorByDistance();  // Moves the motor by the recalculated distance
    void setMotorRPM(float rpm);
    void rotate();
    void stop();
    void start();

    void zeroPosition();  // Resets the motor position to zero
    double getPosition() const { return stepperPosition; }
    double getMovingDistance() const { return movingDistance; }
    bool moveMotorFlag;

private:
    int stepPin;
    int dirPin;
    int stepperDriverEnablePin;
    float pitch;
    float anglePerStep;
    int stepsToMove;
    bool upDirection;
    double stepperPosition;
    double movingDistance;
    float rpm;
    int stepInterval;
    unsigned long previousStepTimer;
    bool rotateFlag;

    void convertDistanceToSteps();
    double recalculateDistance();  // Recalculates the distance based on the rounded steps
};

#endif
