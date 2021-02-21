`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "Air_Quality_Sensor.h"

AirQualitySensor sensor(A2); //!R

unsigned long pass_time = millis();
int exit_while = 0;

void setup()
{
    /*Initialize ports*/
    Serial1.begin(115200);
    Serial.begin(115200);
    sensor.init();

    /*Set-up Wi-Fi Module*/
    delay(1000);
    Serial1.print("AT\\r\\n");
    delay(500);
    Serial1.print("AT+CWMODE_DEF=3\\r\\n");
    delay(500);
    Serial1.print("AT+CWJAP=\"Work\",\"GroveWork\"\\r\\n"); //!R
    delay(500);
}

void loop()
{
    char recvChar;
    unsigned long pass_time2 = millis();
    int quality = sensor.getValue();
    `+ debug_part +
            `
    /*Connect to Host*/
    if(millis() - pass_time > 20000 && exit_while == 0){
        Serial1.print("AT+CIPSTART=\"TCP\",\"192.168.43.28\",4448\\r\\n"); //!R
        exit_while = 1;
    }

    /*Send Data to Host*/
    if( pass_time2 > 25000 ){
        String air_data = String(quality);
        int total_bytes = air_data.length()+16;
        String total_bytes_sent = String(total_bytes);
        Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
        delay(1000);
        Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_air + `"+":"+air_data+":0;"+"` + id_wifi +`"+"\\n");
        }
    delay(`+ textbox + `); //Desired delay
}
`
