// PH stuffs
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.18            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;
static double pHValue,voltage;
// Turbidity
static double kekeruhan, volta;

#include <SoftwareSerial.h>
SoftwareSerial espSerial(0,1);
void setup() {
  espSerial.begin(115200);
}

void loop() {
    double ph = ph_calc(), turb = turbidity_calc();
    String data = String("|") + String(ph) + String(" | ") + String(turb) + String(" | ") + String(volta);
    espSerial.println(data);
    delay(500);
}

double ph_calc(){
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    return pHValue;
    
  }
}


double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}


double turbidity_calc(){
  volta=vr();
  //Serial.print("Nilai tegangan = ");
  //Serial.print(volta);
  kekeruhan = 100.00-(volta/4.21)*100.00;
//  Serial.print(" ");
//  Serial.print("Nilai kekeruhan = ");
  //Serial.print(kekeruhan);
//  Serial.println(" NTU"); 
  return kekeruhan<=0?0:kekeruhan; 
}

float vr() {
  float cnt = 0;
  float tot=0;
  float tr=0;
  float val = analogRead(A1);
  float teg= val * (5.0/1024);
  for (cnt=0;cnt<20;cnt++){
    tot=tot+teg;
  }
  tr=tot/20;
  return tr;
}
