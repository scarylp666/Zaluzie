#include <WiFi.h> 
#include <Servo.h>

Servo ObjServo;

static const int ServoGPIO = 27;
const char* ssid = "MenoWifi";
const char* password = "HesloWifi";


WiFiServer server(80);

String header;

String valueString = String(0);
int positon1 = 0;
int positon2 = 0;

void setup() 
{
WiFi.setSleep(false);
Serial.begin(115200);
ObjServo.attach(ServoGPIO);
Serial.print("Making connection to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin(); 
}

void loop(){
WiFiClient client = server.available();
if (client)
{
  
String header = client.readStringUntil('\r');
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");

client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
client.println(".slider { width: 300px; }</style>");
client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

client.println("</head><body><h1>ESP32 with Servo</h1>");
client.println("<p>Position: <span id=\"servoPos\"></span></p>"); 
client.println("<input type=\"range\" min=\"0\" max=\"140\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");

client.println("<script>var slider = document.getElementById(\"servoSlider\");");
client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

client.println("</body></html>"); 

if(header.indexOf("GET /?value=")>=0) 
{
positon1 = header.indexOf('=');
positon2 = header.indexOf('&');
valueString = header.substring(positon1+1, positon2);

ObjServo.attach(ServoGPIO);
ObjServo.write(valueString.toInt());
if (valueString.toInt() == 0){
  delay(200);
  ObjServo.detach();
  }
else if (valueString.toInt() == 140){
delay(200);
ObjServo.detach();
  }

Serial.println(valueString); 
} 

header = "";
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}
