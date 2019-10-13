#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>

const char* wifiName = "Fiware Test";
const char* wifiPass = "Fiware12345";     // The password of the Wi-Fi network

const char* host= "http://192.168.0.32:1026/v2/entities";
int cont = 001;

void setup() {

  Serial.begin(115200); 
  delay(10);
  
  Serial.print("Connecting to >>> ");
  Serial.print(wifiName); 
  
  WiFi.begin(wifiName, wifiPass); 
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Aguarde o Wi-Fi se conectar
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println("");
  Serial.println("Wi-Fi connected");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Envie o endereço IP do ESP8266 para o computador
}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED){ //Check WiFi connection status
    
//#####################################################################################################   
    //JSON
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& root = JSONbuffer.createObject(); 
    
    root["id"] = "urn:ngsi-ld:Temperatura:"+cont;
    cont++;
    root["type"] = "Temperatura";
    
    JsonObject& name = root.createNestedObject("name");
    name["type"] = "Float";
    name["value"] = "1.0";
    
    JsonObject& refSensor = root.createNestedObject("refSensor");
    refSensor["type"] = "Relationship";
    refSensor["value"] = "urn:ngsi-ld:Sensor:001";
    
    root.printTo(Serial);
//#####################################################################################################

    char JSONmessageBuffer[300];
    root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    
    HTTPClient http;    //Declare object of class HTTPClient 
   
    Serial.print("Request Link:"); 
    Serial.println(host); 
   
    http.begin(host);     //Specify request destination 
    http.addHeader("Content-Type", "application/json"); //Specify content-type header
    
    int httpCode = http.POST(JSONmessageBuffer); //Send the request
    String payload = http.getString(); //Get the response payload
    
    Serial.println(httpCode); //Print HTTP return code
    Serial.println(payload); //Print request response payload
    
    http.end(); //Close connection
    
  }else{
    
    Serial.println("Error in WiFi connection");
    
  }
 
  delay(30000);  //Send a request every 30 seconds
 
}  



/*{
        "id":"urn:ngsi-ld:Temperatura:001", 
        "type":"Temperatura",
        "name":{
            "type":"Text", 
            "value":"Temperatura"
        },
        "refSensor": {
            "type": "Relationship",
            "value": "urn:ngsi-ld:Sensor:001"
        }
  }*/
   
