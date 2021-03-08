#include "DHT.h"
#include "Air_Quality_Sensor.h"

#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);
AirQualitySensor sensor(A2);

//send data per interval seconds(end user decides)
unsigned long interval = 20000;

void setup()
{
    Serial.begin(9600);
    sensor.init();
    setupBlueToothConnection();
}

void loop() {

  char recvChar;
  //define time variables
  unsigned long interval = 15000L;
  unsigned long endTime =0L, startTime =0L;
  String WholeCommand = "";
  byte i = 0;
  //define sensor variables
  float tempData = 0.00, humData = 0.00, airData = 0.00;
  float tempHighest = 0.00, humHighest = 0.00, airHighest = 0.00;
  float tempLowest = 500.00, humLowest = 500.00, airLowest = 500.00;
  float tempSum = 0.00, humSum = 0.00, airSum = 0.00;
  float tempAverage = 0.00, humAverage = 0.00, airAverage = 0.00;
  float temp_hum_val[2] = {0};

  
  /*DEBUG START*/
  while(Serial1.available()){
    recvChar = Serial1.read();
    Serial.print(recvChar);
  }
  while(Serial.available()){
    WholeCommand = SerialString();
    Serial1.print(WholeCommand);
    delay(400);
  }
  /*DEBUG END*/
  
  startTime = millis();
  endTime = startTime;
  while((endTime-startTime) < interval)
  {
    if(!dht.readTempAndHumidity(temp_hum_val))
    {
      //get data
      tempData = temp_hum_val[1];
      humData = temp_hum_val[0];
      airData = sensor.getValue();

      //get highest
      if(tempData > tempHighest)
      {
        tempHighest = tempData;
      }
      if(humData > humHighest)
      {
        humHighest = humData;
      }
      if(airData > airHighest)
      {
        airHighest = airData;
      }
      
      //get lowest
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
    //sum data
    tempSum = tempData + tempSum;
    humSum = humData + humSum;
    airSum = airData + airSum;
    endTime = millis();
    i++;
  }
  //get averages (default)
  tempAverage = tempSum / i;
  humAverage = humSum / i;
  airAverage = airSum / i;
  //here we will send the data
}

String SerialString() {   
  String inputString = "";  
  while (Serial.available()) {
    char inputChar = (char)Serial.read();
    inputString += inputChar;             
    }
    return inputString; 
}

void setupBlueToothConnection()
{
  Serial1.begin(9600);
  Serial1.print("AT");
  delay(400);
  Serial1.print("AT+ROLES");  
  delay(400);
  Serial1.print("AT+NAMESlave");
  delay(400);
  Serial1.print("AT+PIN0000");
  delay(400);
  Serial1.print("AT+AUTH0");
  delay(400);
  Serial1.flush();
}
