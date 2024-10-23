#include <Arduino.h>
#include <GlassStepper.h>

int stepPin = 12;
int dirPin = 14;
int stepperDriverEnablePin = 27;
int pitchPerRev = 500;
double anglePerStep = 1.8;
GlassStepper stepper(stepPin, dirPin, stepperDriverEnablePin, pitchPerRev, anglePerStep);

String inputString = "";  // A string to hold incoming data
bool newData = false;     // Flag to mark when new data is received


// Function to read serial input non-blocking
void readSerialInput() {
    while (Serial.available() > 0) {
        char inChar = (char)Serial.read();
        
        if (inChar == '\n') {  // End of command
            newData = true;    // Mark that we have new data
        } else {
            inputString += inChar;  // Append character to the input string
        }
    }
}

// Function to process the received command
void processInput() {
    if (inputString.length() > 0) {
        char command = inputString.charAt(0);  // Get the first character as command
        if (command == 'g') {
            stepper.start();  // Start rotation
            Serial.println("Rotating...");
        } 
        else if (command == 'r') {
            // Handle 'r' followed by a number (RPM)
            if (inputString.length() > 1) {
                int rpm = inputString.substring(1).toInt();  // Get the number after 'r'
                stepper.setMotorRPM(rpm);                    // Set the RPM
                Serial.print("RPM set to: ");
                Serial.println(rpm);
            } else {
                Serial.println("Error: No RPM value provided.");
            }
        } 
        else if (command == 's') {
            stepper.stop();  // Stop the motor
            Serial.println("Stopped.");
        } else {
            Serial.println("Unknown command.");
        }
    }

    inputString = "";  // Clear the input string after processing
}

void setup() {
    Serial.begin(115200);    // Start serial communication at 9600 baud rate
    inputString.reserve(10);  // Reserve memory for the input string
}

void loop() {
    readSerialInput();  // Check for any serial input

    if (newData) {
        processInput();  // Process the received command
        newData = false; // Reset the flag for new data
    }
    stepper.rotate();
}

