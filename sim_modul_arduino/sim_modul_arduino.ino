 // PH stuffs
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.18            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0, start=0, stopTime=0;
static double pHValue,voltage;


// Editable
int minutes = 300000; // Interval waktu untuk notifikasi selanjutnya jika penampung belum dibersihkan. Default 5 menit
String telpNumber = "\"+6282118988435\""; // No Telp yang ingin dikirimkan notifikasi

// Turbidity Stuffs
#include "GravityTDS.h"
 
#define TdsSensorPin A1 // Where Analog pin of TDS sensor is connected to arduino
GravityTDS gravityTds;
float tdsValue = 0;

#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial simSerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2
SoftwareSerial espSerial(1, 0); // Serial Port to esp

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  Serial.begin(115200);
  pinMode(TdsSensorPin,INPUT);
  
  //Begin serial communication with Arduino and SIM800L
  simSerial.begin(115200);
  espSerial.begin(115200);
  
  Serial.println("Initializing..."); 
  delay(1000);
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    simSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(simSerial.available()) 
  {
    Serial.write(simSerial.read());//Forward what Software Serial received to Serial Port
  }
}

void sendMessage(int State){
  simSerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  simSerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  String data = String("AT+CMGS=") + telpNumber;
  simSerial.println(data);//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  if(state){
      simSerial.print("Tangki sudah keruh, silahkan dibersihkan");
      updateSerial();
    }
  else{
      simSerial.print("Recalibrating Sensors, Please Wait........");
      updateSerial();
    }
  simSerial.write(26);
  }


void loop() {
    double ph = ph_calc(), tds = tds_calc();
    if(tds > 1000 || (ph < 6 || ph > 9)){
      if((abs(stopTime-start) > minutes){ // 5-minute interval
      sendMessage(1);
      }
      else if(stopTime-start == 0) SendMessage(0);
      start = millis();
    }
    String data = String("|") + String(ph) + String("|") + String(tds) + String("|");
    Serial.println(data);
    espSerial.println(data); // Sending data to esp via serial communication (rx tx pin)
    delay(500);
    stopTime = millis();
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
  if(millis() - printTime > printInterval) {  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
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


double tds_calc(){
  gravityTds.update(); //calculation done here from gravity library
  tdsValue = gravityTds.getTdsValue(); // then get the TDS value
  return tdsValue;
}


int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}
