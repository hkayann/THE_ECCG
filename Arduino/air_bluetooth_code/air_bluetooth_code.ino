`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include"Air_Quality_Sensor.h"
AirQualitySensor sensor(A1); //!R

void setup()
{
    Serial.begin(9600);
    setupBlueToothConnection();

    if (sensor.init()){
        Serial.println("Sensor ready.");
    }else{
        Serial.println("Sensor ERROR!"); //if you get an error add initialization delay
    }
}

void loop()
{
    char recvChar;
    unsigned long myTime;
    String WholeCommand = "";
    int quality = sensor.slope();
    while(1)
        {
        if(Serial1.available()){
            recvChar = Serial1.read();
            Serial.print(recvChar);
        }

        if(Serial.available()){
            WholeCommand = SerialString();
            Serial1.print(WholeCommand);
            delay(400);
        }

        myTime = millis();
        if (myTime > 30000) {
            Serial1.print("`+ id_loc + `");
            Serial1.print(";");
            Serial1.print("`+ textbox_nodeID + `");
            Serial1.print(";");
            Serial1.print("`+ id_air + `");
            Serial1.print(":");
            Serial1.print(quality);
            Serial1.print(":0;");
            Serial1.print("B\\n");
            delay(`+ textbox + `);
        }
    }
}

String SerialString()
{
    String inputString = "";
    while (Serial.available()){
        char inputChar = (char)Serial.read();
        inputString += inputChar;
    }
    return inputString;
} // returns completed string


void setupBlueToothConnection()
{
    Serial1.begin(9600);
    Serial1.print("AT");
    delay(400);
    Serial1.print("AT+ROLES"); // set the role as slave !R
    delay(400);
    Serial1.print("AT+NAMESlave"); // set the bluetooth name as "Slave" !R
    delay(400);
    Serial1.print("AT+PIN0000"); //!R
    delay(400);
    Serial1.print("AT+AUTH0");
    delay(400);
    Serial1.flush();
}
`