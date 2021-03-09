`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"
#include "Air_Quality_Sensor.h"
#define DHTPIN 4 //!R
#define DHTTYPE DHT11 //!R
AirQualitySensor sensor(A2); //!R
DHT dht(DHTPIN, DHTTYPE);

//send data per interval seconds(end user decides)
unsigned long interval = 30000L;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  dht.begin();
  sensor.init();

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
  
  byte i = 0;
  float temp_hum_val[2] = {0};
  unsigned long endTime =0L, startTime =0L;
  char recvChar;
  float humData = 0.00, airData = 0.00;
  float humSum = 0.00, airSum = 0.00;
  float humAverage = 0.00, airAverage = 0.00;
  startTime = millis();
  endTime = startTime;
  `+ debug_part + `
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
  //get averages (default)
  humAverage = humSum / i;
  airAverage = airSum / i;
  
  Serial1.print("`+ id_loc + `");
  Serial1.print(";");
  Serial1.print("`+ textbox_nodeID + `");
  Serial1.print(";");
  Serial1.print("`+ id_hum + `");
  Serial1.print(":");
  Serial1.print(humAverage);
  Serial1.print(":0;");
  Serial1.print("`+ id_air + `");
  Serial1.print(":");
  Serial1.print(airAverage);
  Serial1.print(":0;");
  Serial1.print("Z\\n");
}
`
