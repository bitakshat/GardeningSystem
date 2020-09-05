#include<ESP8266WiFi.h>

//NodeMcu
const char* ssid = "YOUR WIFI SSID";
const char* password = "YOUR WIFI PASSWORD";
WiFiServer server(80);

//Reciving data from arduino
const int recv_temp = D5;
const int recv_humid = D6;
const int recv_moisture = D7;
const int recv_light = D8;

void setup(){
  
  pinMode(recv_temp, INPUT);
  pinMode(recv_humid, INPUT);
  pinMode(recv_moisture, INPUT);
  pinMode(recv_light, INPUT);

  Serial.begin(115200);
  //ModeMcu
  Serial.print("Hello world");
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("*");
    delay(500);
  }  

  Serial.println("WiFi Connected! ");
  server.begin(); ///Initializes server///
  Serial.println("Server started");
  Serial.print("Use this IP to connect: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  //NodeMcu Main 
  WiFiClient client = server.available();
  if(!client){
    return;
  }

  while(client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  unsigned int temp_val = analogRead(recv_temp);
  unsigned int humid_val = analogRead(recv_humid);
  unsigned int soil_moist_val = analogRead(recv_moisture);
  unsigned int light_val = analogRead(recv_light);

  /**************Return response*************************/
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCEYPE html>");
  client.println("<html style='background-color: white'>");
  client.println("<head><title>Automatic Garden Control</title></head>");
  client.println("<body>");
  client.println("<h1 align='center'>Automatic Garden Control</h1>");
  /**************Displaying Information*********************/
  client.println("<p style='color: blue'>Temperature: </p>");
  client.println(temp_val);
  client.println("<p style='color: blue'>Humidity: </p>");
  client.println(humid_val);
  client.println("<p style='color: blue'>Soil Moisture: </p>");
  client.println(soil_moist_val);
  client.println("<p style='color: blue'>Light Intensity: </p>");
  client.println(light_val);
  client.println("</body>");
  client.println("</html>");
}
