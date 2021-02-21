`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"
#define DHTPIN 4 //!R
#define DHTTYPE DHT11 //!R

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(9600);
    dht.begin();

    //set the xbee device
    Serial1.print("+++"); //enter in command mode
    delay(1500);

    // set as router (options = end device, router, coordinator)
    Serial1.print("ATCE 0\\r"); //not a coordinator
    Serial1.print("ATSM 0\\r");
    Serial1.print("ATAP 0\\r"); //disable API mode
    Serial1.print("ATID 0xABCD\\r"); //PAN ID should same as coordinator !R
    Serial1.print("ATEE 0\\r"); //disable security

    //set destination to coordinator
    Serial1.print("ATDH 13A200\\r"); //!R
    Serial1.print("ATDL 41B790DE\\r"); //!R

    //apply changes
    Serial1.print("ATAC 1\\r");
    delay(1000);
    Serial1.print("ATWR\\r");
    Serial1.print("ATMY\\r");
    Serial1.print("ATAI\\r"); //debugging
    delay(1000);
}

void loop() {

    float temp_hum_val[2] = {0};
    char recvChar;
    unsigned long myTime;
    `+ debug_part + `
    myTime = millis();
    if (!dht.readTempAndHumidity(temp_hum_val) && myTime > 30000 ){
        Serial1.print("`+ id_loc + `");
        Serial1.print(";");
        Serial1.print("`+ textbox_nodeID + `");
        Serial1.print(";");
        Serial1.print("`+ id_temp + `");
        Serial1.print(":");
        Serial1.print(temp_hum_val[1]);
        Serial1.print(":0;");
        Serial1.print("`+ id_hum + `");
        Serial1.print(":");
        Serial1.print(temp_hum_val[0]);
        Serial1.print(":0;");
        Serial1.print("Z\\n");
        delay(`+ textbox + `);
    }
}
`
