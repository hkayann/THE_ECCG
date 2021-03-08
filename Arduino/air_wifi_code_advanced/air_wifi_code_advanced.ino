`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "Air_Quality_Sensor.h"

AirQualitySensor sensor(A2); //!R
//send data per interval seconds(end user decides)
unsigned long interval = 30000L;
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
  char recvChar;
  unsigned long endTime =0L, startTime =0L;
  byte i = 0;
  float airData = 0.00;
  float airSum = 0.00;
  float airAverage = 0.00;
  
    `+ debug_part +
            `
  /*Connect to Host*/
  if(millis() - pass_time > 20000 && exit_while == 0){
    Serial1.print("AT+CIPSTART=\"TCP\",\"`+ textbox_hostip + `\",` + textbox_port +`\\r\\n");
    exit_while = 1;
  }
  startTime = millis();
  endTime = startTime;
  while((endTime-startTime) < interval)
  {
    airData = sensor.getValue();
    airSum = airData + airSum;
    endTime = millis();
    i++;
  }
  airAverage = airSum / i;
  String air_data = String(airAverage);
  byte total_bytes = air_data.length()+16;
  Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
  delay(1000);
  Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_air + `"+":"+air_data+":0;"+"` + id_wifi +`"+"\\n");
}
`
