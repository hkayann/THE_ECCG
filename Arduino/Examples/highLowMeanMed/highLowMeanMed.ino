#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE); 

// if x seconds have passed - for grove temp. sensor, you get data per half a second.
unsigned long interval = 10000;

void setup()
{
    Serial.begin(9600);
    setupBlueToothConnection();
}

void loop() {
  
  char recvChar;
  unsigned long elapsedTime, endTime;
  String WholeCommand = "";
  byte i = 0;
  float highest, lowest, average, median = 0.00;
  float myArray[] = {3,1,2,5,4,6,7};
  
  // float array for temp data
  float temp_hum_val[2] = {0};

  // initialize arrays to hold temp. and hum. values, 4 bytes per variable
  float temp_data[100];
  float hum_data[100];

  // clear arrays
  memset(temp_data, 0, sizeof(temp_data));
  memset(hum_data, 0, sizeof(hum_data));
  
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

  //initialize the timed loop variables
  elapsedTime = millis();
  endTime = elapsedTime;
     
  // let the sen. to initialize and then, do this for interval seconds
  while((unsigned long)(endTime - elapsedTime) <= interval && elapsedTime > 30000 && i < 100){
    // if(data)
    if(!dht.readTempAndHumidity(temp_hum_val)){
      temp_data[i] = temp_hum_val[1];
      hum_data[i] = temp_hum_val[0];
    }
    Serial.println(temp_data[i]);
    Serial.println(hum_data[i]);
    endTime = millis();
    i++;
  }
  highest = getHighest(temp_data, i);
  lowest = getLowest(temp_data, i);
  average = getMean(temp_data, i);
  getSorted(temp_data, i);
  median = getMedian(temp_data, i);
  Serial.println(highest); 
  Serial.println(lowest);
  Serial.println(average);
  Serial.println(median);
  delay(5000);
}

String SerialString() {   // Function to generate string from chars sent via Serial Monitor
  String inputString = "";  // to hold the string
  while (Serial.available()) {
    char inputChar = (char)Serial.read(); // get the new byte:
    inputString += inputChar;             // add it to the inputString:
    }
    return inputString;     // returns completed string 
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

//function that returns highest value
float getHighest(float passedArray[], byte arraySize){
  float maxData = passedArray[0];
  for(byte i = 1; i < arraySize; i++){
    if(passedArray[i] > maxData){
      maxData = passedArray[i];
    }
  }
  return maxData;
}

//function that returns min. value
float getLowest(float passedArray[], byte arraySize){
  float minData = passedArray[0];
  for(byte i = 1; i < arraySize; i++){
    if(passedArray[i] < minData){
      minData = passedArray[i];
    }
  }
  return minData;
}

// function that returns avg. value
float getMean(float passedArray[], byte arraySize){
  long sum = 0L ;
  for(byte i=0; i < arraySize; i++){
    sum += passedArray[i];
  }
  return ((float)sum) / arraySize; 
}

// sort the array in ascending order
void getSorted(float passedArray[], byte arraySize){
  for (int i=0; i < (arraySize - 1); i++){
    for(int j=0; j < (arraySize - (i+1)); j++){
      if(passedArray[j] > passedArray[j+1]){
        float x = passedArray[j];
        passedArray[j] = passedArray[j+1];
        passedArray[j+1] = x; 
      }
    }
  }
}

//pass already sorted array here
float getMedian(float passedArray[], byte arraySize){
  if(arraySize % 2 != 0){
    byte index = (arraySize-1) / 2;
    return passedArray[(arraySize-1) / 2];
  }else{
    byte midData = arraySize / 2;
    float medianData = (passedArray[midData] + passedArray[midData - 1]) / 2.0;
    return medianData;
  }
}
