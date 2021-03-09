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
  float tempData = 0.00, humData = 0.00, airData = 0.00;
  float tempLowest = 500.00, humLowest = 500.00, airLowest = 500.00;
  float temp_hum_val[2] = {0};
  char recvChar;
    `+ debug_part +
  /*Connect to Host*/
  if(millis() - pass_time > 20000 && exit_while == 0){
    Serial1.print("AT+CIPSTART=\"TCP\",\"`+ textbox_hostip + `\",` + textbox_port +`\\r\\n");
    exit_while = 1;
  }

  startTime = millis();
  endTime = startTime;
  while((endTime-startTime) < interval)
  {
    if(!dht.readTempAndHumidity(temp_hum_val))
    {
      tempData = temp_hum_val[1];
      humData = temp_hum_val[0];
      airData = sensor.getValue();
      if(tempData < tempLowest)
      {
        tempLowest = tempData;
      }
      if(humData < humLowest)
      {
        humLowest = humData;
      }
      if(airData < airLowest)
      {
        airLowest = airData;
      }
    }
    endTime = millis();
  }
  
  String temp_data = String(tempLowest,2);
  String hum_data = String(humLowest,2);
  String air_data = String(airLowest);
  byte total_bytes = temp_data.length()+hum_data.length()+air_data.length()+28;
  String total_bytes_sent = String(total_bytes);
  Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
  delay(1000);
  Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_temp + `"+":"+temp_data+":0;"+"` + id_hum + `"+":"+hum_data+":0;"+"` + id_air + `"+":"+air_data+":0;"+"` + id_wifi +`"+"\\n");
  }
}
`
