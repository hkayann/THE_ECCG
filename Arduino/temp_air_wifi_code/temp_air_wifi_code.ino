`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"
#include "Air_Quality_Sensor.h"

#define DHTPIN 4 //!R
#define DHTTYPE DHT11 //!R
DHT dht(DHTPIN, DHTTYPE);
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
  Serial1.print("AT+CWJAP=\"Work\",\"GroveWork\"\\r\\n");
  delay(500);
}

void loop()
{
  float tempData = 0.00, airData = 0.00;
  float tempSum = 0.00, airSum = 0.00;
  float tempAverage = 0.00, airAverage = 0.00;
  float temp_hum_val[2] = {0};
  char recvChar;
  byte i = 0;
    `+ debug_part +
  /*Connect to Host*/
  if(millis() - pass_time > 20000 && exit_while == 0){
    Serial1.print("AT+CIPSTART=\"TCP\",\"192.168.43.28\",4448\\r\\n"); //!R
    exit_while = 1;
  }
  startTime = millis();
  endTime = startTime;
  while((endTime-startTime) < interval)
  {
    if(!dht.readTempAndHumidity(temp_hum_val))
    {
      tempData = temp_hum_val[1];
      airData = sensor.getValue();
    }
    tempSum = tempData + tempSum;
    airSum = airData + airSum;
    endTime = millis();
    i++;
  }
  tempAverage = tempSum / i;
  airAverage = airSum / i;

  String temp_data = String(tempAverage,2);
  String air_data = String(airAverage);
  byte total_bytes = temp_data.length()+air_data.length()+22;
  String total_bytes_sent = String(total_bytes);
  Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
  delay(1000);
  Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_temp + `"+":"+temp_data+":0;"+"` + id_air + `"+":"+air_data+":0;"+"` + id_wifi +`"+"\\n");
  }
}
`
