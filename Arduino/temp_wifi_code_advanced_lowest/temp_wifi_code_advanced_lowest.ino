`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"

#define DHTPIN 4 //!R
#define DHTTYPE DHT11 //!R
DHT dht(DHTPIN, DHTTYPE);

unsigned long pass_time = millis();
int exit_while = 0;

void setup()
{
  /*Initialize ports*/
  Serial1.begin(115200);
  Serial.begin(115200);
    
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
  float tempData = 0.00;
  float tempLowest = 500.00;
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
      if(tempData < tempLowest)
      {
        tempLowest = tempData;
      }
    }
    endTime = millis();
  }
  String temp_data = String(tempLowest,2);
  byte total_bytes = temp_data.length()+16;
  String total_bytes_sent = String(total_bytes);
  Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
  delay(1000);
  Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_temp + `"+":"+temp_data+":0;"+"` + id_wifi +`"+"\\n");
  }
}
`
