#include <ESP8266WiFi.h>

/* Set to preferred SSID and password */
const char *ssid = "Projeto Girassol";
const char *password = "12345678";

WiFiServer server(80); // server port 80

int valor=120;

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println("Starting ESP-01 Access Point...");
  
  WiFi.softAP(ssid, password);

  IPAddress espIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(espIP);
  
  server.begin();
  Serial.println("ESP-01 Web Server ready...");
  Serial.println();
}


void loop() {
  
  // Check if a client has connected
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }   
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  String webpage = "";

      webpage += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
      webpage += "<head> ";
      webpage += "<meta charset='UTF-8'> ";
      webpage += "<meta http-equiv='cache-control' content='max-age=0' /> ";
      webpage += "<meta http-equiv='cache-control' content='no-cache' /> ";
      webpage += "<meta http-equiv='expires' content='0' /> ";
      webpage += "<meta http-equiv='expires' content='Tue, 31 Ago 2023 1:00:00 GMT' /> ";
      webpage += "<meta http-equiv='pragma' content='no-cache' /> ";
      webpage += "<title>Projeto Girassol</title> ";
      webpage += "<style> ";
      webpage += "body{font-family:Open Sans; color:00047a;} ";
      webpage += "h1{font-size:24px; font-weight:normal; margin:0.4em 0;} ";
      webpage += ".container { width: 100%; margin: 0 auto; } ";
      webpage += ".container .row { float: left; clear: both; width: 100%; } ";
      webpage += ".container .col { float: left; margin: 0 0 1.2em; padding-right: 1.2em; padding-left: 1.2em; } ";
      webpage += ".container .col.four, .container .col.twelve { width: 100%; } ";
      webpage += "@media screen and (min-width: 767px) { ";
      webpage += ".container{width: 100%; max-width: 1080px; margin: 0 auto;} ";
      webpage += ".container .row{width:100%; float:left; clear:both;} ";
      webpage += ".container .col{float: left; margin: 0 0 1em; padding-right: .5em; padding-left: .5em;} ";
      webpage += ".container .col.four { width: 50%; } ";
      webpage += ".container .col.tweleve { width: 100%; } ";
      webpage += "} ";
      webpage += "* {-moz-box-sizing: border-box; -webkit-box-sizing: border-box; box-sizing: border-box;} ";
      webpage += "a{text-decoration:none;} ";
      webpage += ".btn {font-size: 18px; white-space:nowrap; width:100%; padding:.8em 1.5em; font-family: Open Sans, Helvetica,Arial,sans-serif; ";
      webpage += "line-height:18px; display: inline-block;zoom: 1; color: #fff; text-align: center; position:relative; ";
      webpage += "-webkit-transition: border .25s linear, color .25s linear, background-color .25s linear; ";
      webpage += "transition: border .25s linear, color .25s linear, background-color .25s linear;} ";
      webpage += ".btn.btn-sea{background-color: #08bc9a; border-color: #08bc9a; -webkit-box-shadow: 0 3px 0 #088d74; box-shadow: 0 3px 0 #088d74;} ";
      webpage += ".btn.btn-sea:hover{background-color:#01a183;} ";
      webpage += ".btn.btn-sea:active{ top: 3px; outline: none; -webkit-box-shadow: none; box-shadow: none;} ";
      webpage += "</style> ";
      webpage += "</head> ";
      webpage += "<body> ";
      webpage += "<p align='center'><font size='20'>Projeto Girassol</font></p>";
      webpage += "<div class='container'> ";
      webpage += "<div class='row'> ";
      webpage += "<div class='col twelve'> ";
      webpage += "<p align='center'><font size='5'>Essa pagina pertence ao Projeto Girassol da equipe WolfPower do IEEE do Cefet RJ onde vai ser informado os valores de corrente gerados pelas placas solares. </font></p> ";
      webpage += "</div> ";
      webpage += "</div> ";
      webpage += "<p align='left'><font size='5'>Valor da corrente: </font></p> ";
      webpage += valor ;
      webpage += "</body> ";
      webpage += "</html> ";
  
    //Enviando para o browser a 'pagina' criada.
    client.print(webpage);

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}
