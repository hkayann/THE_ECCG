`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"
#define DHTTYPE DHT11 //!R
#define DHTPIN 4 //!R
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);
    setupBlueToothConnection();
}

void loop()
{
    char recvChar;
    unsigned long myTime;
    String WholeCommand = "";
    float temp_hum_val[2] = {0};
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
        if (!dht.readTempAndHumidity(temp_hum_val) && myTime > 30000) {
            Serial1.print("`+ id_loc + `");
            Serial1.print(";");
            Serial1.print("`+ textbox_nodeID + `");
            Serial1.print(";");
            Serial1.print("`+ id_temp + `");
            Serial1.print(":");
            Serial1.print(temp_hum_val[1]);
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
    Serial1.print("AT+NAME`+ textbox_bluetooth_name +`"); // set the bluetooth name as "Slave" !R
    delay(400);
    Serial1.print("AT+PIN`+ textbox_bluetooth_pin +`"); //!R
    delay(400);
    Serial1.print("AT+AUTH0");
    delay(400);
    Serial1.flush();
}
`
