#include <WiFi.h>
#include <ArduinoMqttClient.h>
char ssid[]="VNPC_IoT";
char pass[]="*VNPC_IoT*";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]="miot.ditf.rtu.lv";
int port =1883;
//const char user[]="ntnu";
//const char passMQTT[]="ntnuais2103";

const char topic1[] ="NTNU/Ard2/out/LightSen";

const char topic2[]="NTNU/Web/LED";
const char topic3[] ="NTNU/Ard2/out/LedState";
const char topic4[] ="NTNU/Ard2/out/ButtonState";


void setup() {
pinMode(13,OUTPUT);//LED
#define LedON digitalWrite(13,HIGH)
#define LedOFF digitalWrite(13,LOW)
pinMode(A5,INPUT_PULLUP);
#define button !digitalRead(A5)
pinMode(A3,INPUT);
#define lightSen analogRead(A3)
Serial.begin(9600);

Serial.print("Connecting to Wifi: ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while(WiFi.status()!=WL_CONNECTED)
{
  Serial.print("Waiting for connection.");
  delay(5000);
}
Serial.println("You have connectet to the network");
//MQTT

Serial.print("Connecting to MQTT: ");
Serial.println(broker);
//mqttClient.setUsernamePassword(user,passMQTT);
while(!mqttClient.connect(broker,port))
{
  Serial.print("MQtt connection failed! Error=:.");
  Serial.println(mqttClient.connectError());
  while(1);
}
Serial.println("You have connectet to the MQTT");
mqttClient.subscribe(topic2);
mqttClient.onMessage(onMqttMessage);


}

bool ledState=0;
void loop() {
  mqttClient.poll();
// LedON;
// Serial.print("Button: ");
// Serial.println(button);
// Serial.print("LightSensor: ");
// Serial.println(lightSen);
// LedOFF;
// delay(200);
Serial.print("Publish to topic:");
Serial.println(topic1);
Serial.println(lightSen);
mqttClient.beginMessage(topic1);
mqttClient.print(lightSen);
mqttClient.endMessage();
Serial.print("Publish to topic:");
Serial.println(topic3);
Serial.println(ledState);
mqttClient.beginMessage(topic3);
mqttClient.print(ledState);
mqttClient.endMessage();

mqttClient.beginMessage(topic4);
mqttClient.print(button);
mqttClient.endMessage();

delay(500);


}

void onMqttMessage(int messageSize){
Serial.println("Recived message. Topic: ");
String topic =mqttClient.messageTopic();
Serial.print(topic);
Serial.print(" ,lenght ");
Serial.print(messageSize);
Serial.println (" bytes:");
String message;
while(mqttClient.available()){
message +=(char)mqttClient.read();
}
Serial.println(message);
if (topic==topic2){
  if (message=="on")
  {
    LedON;
    ledState=1;
  }
  else if (message=="off")
  {
  LedOFF;
  ledState=0;

  }
  else{
    Serial.println("Wrong message!!!!!");
  }


}


}
// https://www.tinkercad.com/things/5QzSM3VjRIi-stunning-gogo/editel?tenant=circuits
