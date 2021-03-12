`
/*Please replace with your own correct variables if you see "!R" near the given variable.*/
#include "Air_Quality_Sensor.h"

AirQualitySensor sensor(A2); //!R
//send data per interval seconds(end user decides)
unsigned long interval =`+textbox_timeInterval+`L;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  sensor.init();

  //set the xbee device
  Serial1.print("+++"); //enter in command mode
  delay(1500);

  // set as router (options = end device, router, coordinator)
  Serial1.print("ATCE 0\\r"); //not a coordinator
  Serial1.print("ATSM 0\\r");
  Serial1.print("ATAP 0\\r"); //disable API mode
  Serial1.print("ATID `+ textbox_zigbee_pan +`\\r"); //PAN ID should same as coordinator !R
  Serial1.print("ATEE 0\\r"); //disable security

  //set destination to coordinator
  Serial1.print("ATDH `+ textbox_zigbee_DH + `\\r"); //!R
  Serial1.print("ATDL `+ textbox_zigbee_DL +`\\r"); //!R

  //apply changes
  Serial1.print("ATAC 1\\r");
  delay(1000);
  Serial1.print("ATWR\\r");
  Serial1.print("ATMY\\r");
  Serial1.print("ATAI\\r"); //debugging
  delay(1000);
}

void loop() {

  char recvChar;
  unsigned long endTime =0L, startTime =0L;
  byte i = 0;
  float airData = 0.00;
  float airHighest = 0.00;
    `+ debug_part + `
  startTime = millis();
  endTime = startTime;
  while((endTime-startTime) < interval)
  {
    airData = sensor.getValue();
    if(airData > airHighest)
    {
      airHighest = airData;
    }
    endTime = millis();
  }
  Serial1.print("`+ id_loc + `");
  Serial1.print(";");
  Serial1.print("`+ textbox_nodeID + `");
  Serial1.print(";");
  Serial1.print("`+ id_air + `");
  Serial1.print(":");
  Serial1.print(airHighest);
  Serial1.print(":0;");
  Serial1.print("Z\\n");
}
`
