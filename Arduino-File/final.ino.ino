#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Lee";
const char* password = "";

ESP8266WebServer server(80);

// // Function to blink the LED
void blinkLED(int numTimes, int delayTime) {
  for (int i = 0; i < numTimes; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayTime);
  }

  digitalWrite(LED_BUILTIN, LOW);
}

void handleRoot() {
  if (server.method() == HTTP_GET) {
    String message = "Hello from NodeMCU!";
    server.send(200, "text/plain", message);
  }
}

void handlePost() {
    String payload = server.arg("plain");
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);

    if (doc.containsKey("prediction")) {
        String message = doc["prediction"].as<String>();
        Serial.println("Message : "+message);

        if((message=="ambulance"))
        {
          blinkLED(3,2000);
        }
        else
        {
          digitalWrite(LED_BUILTIN,LOW);
          
        }
        
        server.send(200, "text/plain", "Received message: " + message);
    } else {
      server.send(400, "text/plain", "Bad Request: 'message' field is missing");
    }

    // server.send(200, "text/plain", "Received: " + server.arg("plain"));
    // Serial.println("Received Data"+server.arg("prediction"));
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize Serial communication
  digitalWrite(LED_BUILTIN,LOW);
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  Serial.print("NodeMCU IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize the web server
  server.on("/", handleRoot);
  server.on("/post", handlePost);

  // Start the server
  server.begin();
  Serial.println("Server started!");
}

void loop() {
  // Handle client requests
  server.handleClient();
}

