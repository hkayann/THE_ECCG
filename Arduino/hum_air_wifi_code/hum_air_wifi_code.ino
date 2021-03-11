`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"
#include "Air_Quality_Sensor.h"

#define DHTPIN 4 //!R
#define DHTTYPE DHT11 //!R
DHT dht(DHTPIN, DHTTYPE);
AirQualitySensor sensor(A2); //!R
//send data per interval seconds(end user decides)
unsigned long interval =`+textbox_timeInterval+`L;
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
  float humData = 0.00, airData = 0.00;
  float humSum = 0.00, airSum = 0.00;
  float humAverage = 0.00, airAverage = 0.00;
  float temp_hum_val[2] = {0};
  char recvChar;
  byte i = 0;
    `+ debug_part +`
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
      humData = temp_hum_val[0];
      airData = sensor.getValue();
    }
    humSum = humData + humSum;
    airSum = airData + airSum;
    endTime = millis();
    i++;
  }
  humAverage = humSum / i;
  airAverage = airSum / i;

  String hum_data = String(humAverage,2);
  String air_data = String(airAverage);
  byte total_bytes = hum_data.length()+air_data.length()+22;
  String total_bytes_sent = String(total_bytes);
  Serial1.print("AT+CIPSEND="+total_bytes_sent+"\\r\\n");
  delay(1000);
  Serial1.print("`+ id_loc + `"+";"+"` + textbox_nodeID + `"+";"+"` + id_hum + `"+":"+hum_data+":0;"+"` + id_air + `"+":"+air_data+":0;"+"` + id_wifi +`"+"\\n");
  }
}
`
