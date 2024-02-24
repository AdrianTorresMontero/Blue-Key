#include "BluetoothSerial.h"
BluetoothSerial ESP_BT;

//For infrared part
int infrared_input = 2;
int analogValue=0;
String receive;

//For motor/opening part
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 150;


void setup() {
  //Bluetooth part
  ESP_BT.begin("Bluetooth");
  Serial.begin(115200);
  //Infrared part
  pinMode(infrared_input, INPUT);
  //Motor part
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcWrite(pwmChannel, dutyCycle);
}

void loop() {
  //int value_entrada = digitalRead(infrared_input);

  //Bluetooth part for motor

  if(ESP_BT.available()){
    receive = ESP_BT.readString();
    Serial.println(receive);
    delay(10);
  }
  if(Serial.available()){
    receive = Serial.readString();
    ESP_BT.println(receive);
    delay(10);
  } 
  if (receive == "Open"){
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    delay(2000);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }
  else if (receive == "Close"){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    delay(2000); 
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }


  dutyCycle = 150;
  ledcWrite(pwmChannel, dutyCycle);

  //Reading signal part

  // Read the analog value from the sensor
  int analogValue = analogRead(infrared_input) ;

  String message;


  // Send the voltage data over Bluetooth
  if (analogValue > 1){
    message = "NO In position";
    delay(1000);
  }else{
    message= "In position";
    delay(1000);
  }

  ESP_BT.print(message);
  //ESP_BT.println(analogValue);

  receive ="";


}
