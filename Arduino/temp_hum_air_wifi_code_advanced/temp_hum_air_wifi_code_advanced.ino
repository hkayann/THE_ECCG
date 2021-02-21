`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"
#include "Air_Quality_Sensor.h"

#define DHTPIN 4 //!R
#define DHTTYPE DHT11 //!R
DHT dht(DHTPIN, DHTTYPE);
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
    Serial1.print("AT+CWJAP=\"`+ textbox_ssid + `\",\"` + textbox_passwd +`\"\\r\\n");
    delay(500);
}

void loop()
{
    float temp_hum_val[2] = {0};
    char recvChar;
    unsigned long pass_time2 = millis();
    int quality = sensor.getValue();
    `+ debug_part +
            `
    /*Connect to Host*/
    if(millis() - pass_time > 20000 && exit_while == 0){
        Serial1.print("AT+CIPSTART=\"TCP\",\"`+ textbox_hostip + `\",` + textbox_port +`\\r\\n");
        exit_while = 1;
    }

    /*Send Data to Host*/
    if(!dht.readTempAndHumidity(temp_hum_val) && pass_time2 > 25000 ){
        String temp_data = String(temp_hum_val[1],2);
        String hum_data = String(temp_hum_val[0],2);
        String air_data = String(quality);
        int total_bytes = temp_data.length()+hum_data.length()+air_data.length()+28; //!R make sure its right!
        String total_bytes_sent = String(total_bytes);
        Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
        delay(1000);
        Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_temp + `"+":"+temp_data+":0;"+"` + id_hum + `"+":"+hum_data+":0;"+"` + id_air + `"+":"+air_data+":0;"+"` + id_wifi +`"+"\\n");
    }
    delay(`+ textbox + `); //Desired delay
}
`
