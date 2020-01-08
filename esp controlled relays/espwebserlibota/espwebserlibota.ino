

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
const char* ssid = "ssid";
const char* password = "password";

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
ESP8266HTTPUpdateServer httpUpdater;
// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output16State = "off";
String output14State = "off";
String output12State = "off";
String output13State = "off";

// Assign output variables to GPIO pins
const int output16 = 16;
const int output14 = 14;
const int output12 = 12;
const int output13 = 13;
const int led = 0;
unsigned int handleRoot_counter=0; //for rebooting after 50 root pages

// for rebooting after all variables low
//int period_reb = 45000;
//unsigned long time_last = 0;
unsigned int i=0;

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 19800, 60000);


void setup(void) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  // Initialize the output variables as outputs
  pinMode(output16, OUTPUT);
  pinMode(output14, OUTPUT);
  pinMode(output12, OUTPUT);
  pinMode(output13, OUTPUT);
  pinMode(led, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output16, LOW);
  digitalWrite(output14, LOW);
  digitalWrite(output12, LOW);
  digitalWrite(output13, LOW);
  digitalWrite(led, LOW);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  digitalWrite(led, HIGH);

  server.on("/", handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  //server.on("/inline", []() {
  // server.send(200, "text/plain", "this works as well");
  //});
  server.on("/16/on", []() {
    Serial.println("GPIO 16 on");
    output16State = "on";
    digitalWrite(output16, HIGH);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");
  });
  server.on("/16/off", []() {
    Serial.println("GPIO 16 off");
    output16State = "off";
    digitalWrite(output16, LOW);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/14/on", []() {
    Serial.println("GPIO 14 on");
    output14State = "on";
    digitalWrite(output14, HIGH);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/14/off", []() {
    Serial.println("GPIO 14 off");
    output14State = "off";
    digitalWrite(output14, LOW);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/12/on", []() {
    Serial.println("GPIO 12 on");
    output12State = "on";
    digitalWrite(output12, HIGH);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/12/off", []() {
    Serial.println("GPIO 12 off");
    output12State = "off";
    digitalWrite(output12, LOW);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/13/on", []() {
    Serial.println("GPIO 13 on");
    output13State = "on";
    digitalWrite(output13, HIGH);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/13/off", []() {
    Serial.println("GPIO 13 off");
    output13State = "off";
    digitalWrite(output13, LOW);
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");

  });
  server.on("/reboot", []() {
    Serial.println("Rebooting....");
    ESP.restart();

  });
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  httpUpdater.setup(&server);
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
  timeClient.begin();
  Serial.println("TimeClient started");
//  time_last=millis();
}

void loop(void) {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(led, HIGH);
    }else{
      digitalWrite(led, LOW);
      }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    i=i+1;
    if (i>=120){
      WiFi.reconnect();
      i=0;
      break;
      }
  }
//  if ((millis() > time_last + period_reb)&&(output16State=="off")&&(output14State=="off")&&(output12State=="off")&&(output13State=="off")){
//    ESP.restart();
//    }
  
  server.handleClient();                    // Listen for HTTP requests from clients
  
}

void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  handleRoot_counter=handleRoot_counter+1;
  if (handleRoot_counter>=50){
    Serial.println("restarting esp root more than 50");
    ESP.restart();
    }
  String content = "<!DOCTYPE html><html>";
  content += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  content += "<link rel=\"icon\" href=\"data:,\">";
  content += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  content += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
  content += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  content += ".button2 {background-color: #77878A;}</style></head>";

  content += "<body><h1>Outside Buttons</h1>";
  
  // Display current state, and ON/OFF buttons for GPIO 16
  content += "<p>Button 1:GPIO 16 - State " + output16State + "</p>";
  // If the output5State is off, it displays the ON button
  if (output16State == "off") {
    content += "<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    content += "<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  // Display current state, and ON/OFF buttons for GPIO 14
  content += "<p>Button 2:GPIO 14 - State " + output14State + "</p>";
  // If the output14State is off, it displays the ON button
  if (output14State == "off") {
    content += "<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    content += "<p><a href=\"/14/off\"><button class=\"button button2\">OFF</button></a></p>";
  }
  // Display current state, and ON/OFF buttons for GPIO 12
  content += "<p>Button 3:GPIO 12 - State " + output12State + "</p>";
  // If the output12State is off, it displays the ON button
  if (output12State == "off") {
    content += "<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    content += "<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  // Display current state, and ON/OFF buttons for GPIO 13
  content += "<p>Button 4:GPIO 13 - State " + output13State + "</p>";
  // If the output13State is off, it displays the ON button
  if (output13State == "off") {
    content += "<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    content += "<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>";
  }
  content += "<p>Free Heap =  " + String(ESP.getFreeHeap()) + "</p>";
  content += "<p>Heap Frag =  " + String(ESP.getHeapFragmentation()) + "</p>";
  
  content += "<p>HandleRoot_counter =  " + String(handleRoot_counter) + "</p>";
  content += "<p>Signal =  " + String(WiFi.RSSI()) + "</p>";
  content += "</body></html>";
  content += "";
  server.send(200, "text/html", content);

}

//void handleLED() {                          // If a POST request is made to URI /LED
//  digitalWrite(led,!digitalRead(led));      // Change the state of the LED
//server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
//  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
//}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
