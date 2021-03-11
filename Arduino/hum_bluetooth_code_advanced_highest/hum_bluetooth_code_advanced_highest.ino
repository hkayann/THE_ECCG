`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "DHT.h"

#define DHTTYPE DHT11 //!R
#define DHTPIN 4 //!R
DHT dht(DHTPIN, DHTTYPE);

//send data per interval seconds(end user decides)
unsigned long interval =`+textbox_timeInterval+`L;

void setup()
{
  Serial.begin(9600);
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
  float humData = 0.00;
  float humHighest = 0.00;
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
      humData = temp_hum_val[0];
      if(humData > humHighest)
      {
        humHighest = humData;
      }
    }
    endTime = millis();
    i++;
  }
  Serial1.print("`+ id_loc + `");
  Serial1.print(";");
  Serial1.print("`+ textbox_nodeID + `");
  Serial1.print(";");
  Serial1.print("`+ id_hum + `");
  Serial1.print(":");
  Serial1.print(humHighest);
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
}

void setupBlueToothConnection()
{
  Serial1.begin(9600);
  Serial1.print("AT");
  delay(400);
  Serial1.print("AT+ROLES"); // set the role as slave !R
  delay(400);
  Serial1.print("AT+NAME`+ textbox_bluetooth_name +`"); // set the bluetooth name as "Slave" !R
  delay(400);
  Serial1.print("AT+PIN`+ textbox_bluetooth_pin +`");
  delay(400);
  Serial1.print("AT+AUTH0");
  delay(400);
  Serial1.flush();
}
`
