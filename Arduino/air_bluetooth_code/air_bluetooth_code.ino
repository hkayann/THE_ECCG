`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include"Air_Quality_Sensor.h"
AirQualitySensor sensor(A1); //!R

//send data per interval seconds(end user decides)
unsigned long interval =`+textbox_timeInterval+`L;

void setup()
{
  Serial.begin(9600);
  sensor.init();
  setupBlueToothConnection();
}

void loop()
{
  char recvChar;
  //define time variables
  unsigned long endTime =0L, startTime =0L;
  String WholeCommand = "";
  byte i = 0;
  //define sensor variables
  float airData = 0.00;
  float airSum = 0.00;
  float airAverage = 0.00;

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
    airData = sensor.getValue();
    airSum = airData + airSum;
    endTime = millis();
    i++;
  }
  airAverage = airSum / i;
  Serial1.print("`+ id_loc + `");
  Serial1.print(";");
  Serial1.print("`+ textbox_nodeID + `");
  Serial1.print(";");
  Serial1.print("`+ id_air + `");
  Serial1.print(":");
  Serial1.print(airAverage);
  Serial1.print(":0;");
  Serial1.print("B\\n");
}

String SerialString()
{
  String inputString = "";
  while (Serial.available()){
    char inputChar = (char)Serial.read();
    inputString += inputChar;
  }
  return inputString;
} // returns completed string


void setupBlueToothConnection()
{
  Serial1.begin(9600);
  Serial1.print("AT");
  delay(400);
  Serial1.print("AT+ROLES"); // set the role as slave !R
  delay(400);
  Serial1.print("AT+NAMESlave"); // set the bluetooth name as "Slave" !R
  delay(400);
  Serial1.print("AT+PIN0000"); //!R
  delay(400);
  Serial1.print("AT+AUTH0");
  delay(400);
  Serial1.flush();
}
`
