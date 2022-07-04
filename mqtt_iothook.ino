#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "****";
const char* password =  "****";
const char* mqttServer = "iothook.com";
const int mqttPort = 1883;
const char* mqttUser = "publicmqttbroker";
const char* mqttPassword = "publicmqttbroker";

 
WiFiClient espClient;
PubSubClient client(espClient);

 
void setup() {
 
  Serial.begin(115200);

  /* WiFi BAĞLANTISI */
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");


  /* MQTT BAĞLANTISI */
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("testtopic", "test");
  client.subscribe("testtopic");
 
}


 
void callback(char* topic, byte* payload, unsigned int length) {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.println((char)payload[i]);
  }
  payload[length]= '\0';

  char * charPointer = (char *)payload;
  double f=0;
  int in = 0;
  String s="";
  if(isDigit(charPointer[0])){
    f = atof(charPointer);
    in = atoi(charPointer);
    Serial.printf("Float = %0.1f, int = %d\n", f,in);
  }else{
    s =charPointer;
    Serial.printf("String = %s, ",charPointer);
    Serial.print("String objesi = ");
    Serial.println(s);
  }
  float hesap = (60/f);
  float delay_hesap = (hesap/2);
  Serial.println();
  Serial.print("Dakikada ");
  Serial.print(in);
  Serial.println(" kere tur atacak");
  Serial.println();
  Serial.print(delay_hesap);
  Serial.println(" saniye yanıp sönecek");
  Serial.println();

  for(int i = 0; i < in; i++){
    digitalWrite(LED_BUILTIN,LOW);
    delay(delay_hesap*1000);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(delay_hesap*1000);
    Serial.print(i+1);
    Serial.println(". tur bitti");
    Serial.println();
  }
  

  Serial.println();
  Serial.println("-----------------------");
  digitalWrite(LED_BUILTIN,HIGH);

}



void loop() {
  client.loop();
}
