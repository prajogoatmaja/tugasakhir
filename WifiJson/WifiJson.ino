#include <ArduinoJson.h>
#include <WiFi.h>
#include <SPI.h>

/* WiFi network name and password */
const char* ssid = "Tenda_2E4718";
const char* pwd = "0987654321";

/* Put IP Address details */
IPAddress local_ip(192, 168, 17, 254);
IPAddress gateway(192, 168, 17, 1);
IPAddress subnet(255, 255, 255, 0);

//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
WiFiServer server(80);

// Variable to store the HTTP request
String header;

const size_t capacity = JSON_ARRAY_SIZE(6) + JSON_OBJECT_SIZE(1) + 6*JSON_OBJECT_SIZE(2) + 140;


void setup() {
  // Initialize serial port
  Serial.begin(115200);

  //Connect to the WiFi network
  WiFi.begin(ssid, pwd);
  WiFi.config(local_ip, gateway, subnet);
  Serial.println("");

  while (!Serial) continue;

  // Start to listen
  server.begin();

  Serial.println(F("Server is ready."));
  Serial.print(F("Please connect to http://"));
  Serial.println(WiFi.localIP());
}

void loop() {
  // Wait for an incomming connection
  WiFiClient client = server.available();   // Listen for incoming clients
  // Do we have a client?
  if (client) {                             // If a new client connects,
    Serial.println(F("New client"));        // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // Allocate a temporary JsonDocument
            // Use arduinojson.org/v6/assistant to compute the capacity.
            DynamicJsonDocument doc(capacity);        
            if (header.indexOf("GET /posts") >= 0) {
              JsonArray posts = doc.createNestedArray("posts");
            
              JsonObject posts_0 = posts.createNestedObject();
              posts_0["id"] = 1;
              posts_0["time"] = "00:04:128";
            
              JsonObject posts_1 = posts.createNestedObject();
              posts_1["id"] = 2;
              posts_1["time"] = "00:07:321";
            
              JsonObject posts_2 = posts.createNestedObject();
              posts_2["id"] = 3;
              posts_2["time"] = "00:10:913";
            
              JsonObject posts_3 = posts.createNestedObject();
              posts_3["id"] = 4;
              posts_3["time"] = "00:12:856";
            
              JsonObject posts_4 = posts.createNestedObject();
              posts_4["id"] = 5;
              posts_4["time"] = "00:15:015";
            
              JsonObject posts_5 = posts.createNestedObject();
              posts_5["id"] = 6;
              posts_5["time"] = "00:17:843";

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            // Write response headers
            client.println(F("HTTP/1.0 200 OK"));
            client.println(F("Content-Type: application/json"));
            client.println(F("Connection: close"));
            client.print(F("Content-Length: "));
            client.println(measureJsonPretty(doc));
            client.println();
            // Write JSON document
            serializeJsonPretty(doc, client);
            }
          }
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
          }
      }
    }
  // Clear the header variable
  header = "";
  // Disconnect
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  }
}
