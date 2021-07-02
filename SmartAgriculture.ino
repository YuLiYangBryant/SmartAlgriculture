// 文彥的專題
#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"
int photocellPin = A0; // 光敏電阻 (photocell) 接在 anallog pin 2
int photocellVal = 0; // photocell variable
int minLight = 200;   // 最小光線門檻值
int ledPin = 5;
int motorPin = 2;
int ledState = 0; 
int pirPin = 17;
int pirVal = 0;
int beePin = 3;
int loraPin = 12;
int sensV = 0;
//int moivalue = 0;
String  lora_string;
int readbuffersize;
char temp_input;
String second_response;
#define _SSID "ILOVEBECKY"
#define _KEY  "01291002"
MCSDevice mcs("DwxODdnk", "mxkMzbVJnRjwfG9x");
MCSControllerOnOff aauto("aauto");
MCSControllerInteger moisture("moisture");
MCSControllerOnOff light("light");
MCSControllerOnOff pirsensor("pirsensor");
MCSDisplayInteger Moi("Moi");
MCSControllerOnOff moistureonoff("moistureonoff");



void setup() {
  pinMode(ledPin, OUTPUT); 
  Serial.begin(115200);
  pinMode(motorPin, OUTPUT);
  pinMode(loraPin,OUTPUT);
  pinMode(beePin, OUTPUT);
  

  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial1.begin(115200);
  while (!Serial1) {
    Serial.print(".");       // wait for serial1 port to connect
  }
  Serial.println("connected!");
  delay(1000);  
  // for rest EK-S76SXB
      // EK-S76SXB NRST(PIN5) to L7inkIt 7697 P12
  digitalWrite(loraPin, LOW);  //
  delay(100);             // wait for 0.1 second
  pinMode(loraPin, INPUT);     // for rest EK-S76SXB
  delay(2000);

  Serial1.print("mac join abp");  // Join LoRaWAN
  Serial.println("mac join abp");
  delay(5000);

  while(WL_CONNECTED != WiFi.status()){
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  //setup MCS connection
  //mcs.addChannel(led);
  mcs.addChannel(aauto);
  mcs.addChannel(moisture);
  mcs.addChannel(pirsensor);
  mcs.addChannel(light);
  mcs.addChannel(Moi);
  mcs.addChannel(moistureonoff);
  
  while(!mcs.connected()){
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  // read LED value from MCS server
  Serial.begin(115200); 
  




}
void loop() {

  while (!mcs.connected()) {                         //檢查與MCS的連線狀態，如果斷線會在此重連
    mcs.connect();
    if (mcs.connected()) {
      Serial.println("MCS Reconnected.");
    }
  }
  mcs.process(100);
  if(aauto.value()==1){
    Serial.println("現在是自動狀態");
    moisturefunction();
    lightfunction();
    pirsensorfunction();
    lorafunction();
    
  }
  else{
    Serial.println("現在是手動狀態");
    
    if(moistureonoff.value()==1){
      
      digitalWrite(motorPin,LOW);
      Serial.println("馬達打開");
      delay(1000);
    }
    else{
        digitalWrite(motorPin,HIGH);
        Serial.println("馬達關起來");
        delay(1000);
    }
    if(light.value()==1){
        digitalWrite(ledPin, HIGH);
        Serial.println("現在電燈應該要打開"+ light.value());
        delay(1000);
    }
    else{
        digitalWrite(ledPin, LOW);
        Serial.println("這時候電燈應該要關起來");
        delay(1000);
    }
    if(pirsensor.value()==0){
        digitalWrite(beePin,LOW);
        Serial.println("現在紅外線感測應該是沒用的狀態");
        delay(1000);
    }
    else{
        pirsensorfunction();
    }
 
  }//ifelse收到自動手動訊號下引號
  
  Moi.set(sensV);

}//void loop下引號



void moisturefunction(){
  int sensorValue = analogRead(A2); //土壤濕度感測器的接腳，讀取數值
  sensV = sensorValue/34;

  Serial.println(sensV);
    
  delay(500);
  if(sensV < 50){   //判斷是否太乾燥。可依不同環境及土壤，設定不同的數值。
  digitalWrite(motorPin,LOW);   //如果太乾燥，就啟動抽水泵給水
  }else{
  digitalWrite(motorPin,HIGH);    //如果濕度夠了就停止給水
  }
  Moi.set(sensV);
}


void lightfunction(){
  // 讀取光敏電阻並輸出到 Serial Port 
    photocellVal = analogRead(photocellPin);
  Serial.println(photocellVal);   
  
  // 光線不足時打開 LED
  if (photocellVal < 1000) {
  digitalWrite(ledPin, HIGH); // turn on LED
  ledState = 1;
  }

  // 光線充足時關掉 LED
  if (photocellVal > 1000) {
  digitalWrite(ledPin, LOW); // turn off LED
  ledState = 0;
  }  

  delay(100);
}


void pirsensorfunction(){
  pirVal = digitalRead(pirPin); //read state of the PIR
  Serial.println(pirVal);
  digitalWrite(beePin,LOW); 
  if (pirVal == LOW) {
  delay(1000);
  Serial.println("NO Motion"); //if the value read is low, there was no motion
  //digitalWrite(16,LOW);
  }
  else {
  delay(1000);
  Serial.println("Motion!"); //if the value read was high, there was motion
  digitalWrite(beePin,HIGH);
  delay(5000);
   }

}

void lorafunction(){
  
  lora_string = "mac tx ucnf 2 ";
  lora_string+=sensV;
  lora_string+=pirVal;
  lora_string+=photocellVal;

    // 印出資料
  Serial.println("LoRa - send data - " + lora_string);
  // 送出資料
  Serial1.print(lora_string);           // tx now
   
  // delay(5000);
  delay(1000);

  // first_response
  readbuffersize = Serial1.available();
  while(readbuffersize){
    temp_input = Serial1.read();
    Serial.print(temp_input);
    readbuffersize--;
    }
  delay(2000);


  // second_response
  readbuffersize = Serial1.available();
  while(readbuffersize){
    temp_input = Serial1.read();
    Serial.print(temp_input);
    readbuffersize--;
  }

  Serial.println("====================================================");
  delay(5000);
  
}
