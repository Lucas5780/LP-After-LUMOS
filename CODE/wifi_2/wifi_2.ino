/*Fonctionnement d'un capteur ultrason HC-SR04, vous pouvez retrouver ce composant sur notre site www.ihm3d.fr */
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#ifndef APSSID
#define APSSID "Lumoss"
#define APPSK  "o"
#endif

Servo myservo;
int trig = 15;
int echo = 12;
long lecture_echo;
long cm;
int v;
int v1;
int v2;
bool etat = 0;
bool ultrason;
bool site_on =0;
const char *ssid = APSSID;
const char *password = APPSK;
int pos;

//page html
const char index_html[]PROGMEM = R"=====(
  <!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Coucou</title>
  </head>
  
<body>
 <a href="/On">
<label class="switch"><input type="checkbox" id="togBtn" onclick="myFunction()" class ="check">
  <div class="slider round"><!--ADDED HTML -->
  <span class="on">ON</span>
  <span class="off">OFF</span>
  <!--END-->
</div></label>
</a>
<style>

 
body{
  margin-top : 20%;
  text-align: center;
  background-color : #072e3a

}
.switch {
  
  position: relative;
  display: inline-block;
  width: 190px;
  height: 134px;
}

.switch input {display:none;}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ca2222;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 126px;
  width: 126px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider {
  background-color: #2ab934;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(55px);
  -ms-transform: translateX(55px);
  transform: translateX(55px);
}

/*------ ADDED CSS ---------*/
.on
{
  display: none;
}

.on, .off
{
  color: white;
  position: absolute;
  transform: translate(-50%,-50%);
  top: 50%;
  left: 50%;
  font-size: 10px;
  font-family: Verdana, sans-serif;
}

input:checked+ .slider .on
{display: block;}

input:checked + .slider .off
{display: none;}

/*--------- END --------*/

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;}

</style>


<script>
function myFunction(){
  var result = document.getElementsByClassName("check")[0].checked ? 'yes' : 'no'
  console.log(result);

 
    // Get the checkbox
  var checkBox = document.getElementById("togBtn");
  // Get the output text
  var text = document.getElementById("text");

  // If the checkbox is checked, display the output text
  if (checkBox.checked == true){
    text.style.display = "block";
  } else {
    text.style.display = "none";
  }

  if (result == "yes"){
    console.log("on");
     window.open("http://192.168.4.1/On","_blank"); 
    

 
      
  }

  if (result == "no"){
    console.log("off");
    
  }
}
  
</script>
<p id="text" style="display:none">Vous avez allumé la lumière !</p>

</body>
</html>
)=====";

const char index_html_checked[]PROGMEM = R"=====(
  <!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Coucou</title>
  </head>
  
<body>
 <a href="/On">
<label class="switch"><input type="checkbox" id="togBtn" onclick="myFunction()" class ="check" checked =1>
  <div class="slider round"><!--ADDED HTML -->
  <span class="on">ON</span>
  <span class="off">OFF</span>
  <!--END-->
</div></label>
</a>
<style>

 
body{
  margin-top : 20%;
  text-align: center;
  
}
.switch {
  
  position: relative;
  display: inline-block;
  width: 190px;
  height: 134px;
}

.switch input {display:none;}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ca2222;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 126px;
  width: 126px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider {
  background-color: #2ab934;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(55px);
  -ms-transform: translateX(55px);
  transform: translateX(55px);
}

/*------ ADDED CSS ---------*/
.on
{
  display: none;
}

.on, .off
{
  color: white;
  position: absolute;
  transform: translate(-50%,-50%);
  top: 50%;
  left: 50%;
  font-size: 10px;
  font-family: Verdana, sans-serif;
}

input:checked+ .slider .on
{display: block;}

input:checked + .slider .off
{display: none;}

/*--------- END --------*/

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;}

</style>


<script>
function myFunction(){
  var result = document.getElementsByClassName("check")[0].checked ? 'yes' : 'no'
  console.log(result);

 
    // Get the checkbox
  var checkBox = document.getElementById("togBtn");
  // Get the output text
  var text = document.getElementById("text");

  // If the checkbox is checked, display the output text
  if (checkBox.checked == true){
    text.style.display = "block";
  } else {
    text.style.display = "none";
  }

  if (result == "yes"){
    console.log("on");
    window.open("http://192.168.4.1/On","_blank"); 
    

 
      
  }

  if (result == "no"){
    console.log("off");
    window.open("http://192.168.4.1/Off","_blank");
   }
}
  
</script>
<p id="text" style="display:none">Vous avez allumé la lumière !</p>

</body>
</html>
)=====";




ESP8266WebServer server(80);

 void handleRoot() {
  server.send(200, "text/html", index_html);
  Serial.println("bravo");
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH4
 
}

  void SwitchOn() {
  myservo.attach(16);
  site_on = 1; 
  Serial.println("Allumé");
  server.send(200, "text/html", index_html_checked);
  
  for (pos = 25; pos <= 145; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  myservo.detach();
   
  }

  void SwitchOff() {
  myservo.attach(16);0.
  site_on = 1;
  Serial.println("éteint");
  server.send(200, "text/html", index_html);
  
  for (pos = 145; pos >= 25; pos -= 1) { // goes from 180 degrees to 0 degrees
  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  delay(15);                       // waits 15ms for the servo to reach the position
  }
  myservo.detach();

  }

  
void setup(){
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(echo, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
   delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/On", SwitchOn);
  server.on("/Off", SwitchOff);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}



void loop(){

int angle;


digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);
lecture_echo = pulseIn(echo,HIGH);
cm = lecture_echo /58;
delay(500);

v = cm;
v1 = v;
v2 = v1;


Serial.print ("Servo position :");
Serial.println(pos);
 

if (cm > 0 && v < 20 && v1 < 20 && v2 < 20 && pos >= 0) {  
  ultrason = 1;
}

else {
  ultrason = 0;
}

if (ultrason && etat == 0) { 
   etat = 1;
   myservo.attach(16);
  site_on = 0;
  delay(1000);
 }

else if (ultrason && etat == 1) {   
  etat = 0;
  myservo.attach(16);

  site_on = 0;
  delay(1000);
  
}

if (etat == 0 && !site_on) {
  
  for (pos = 25; pos <= 145; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  myservo.detach();
 
 }

if (etat ==1 && !site_on) {
  
  for (pos = 145; pos >= 25; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

   myservo.detach();

  
}

Serial.println(etat);

server.handleClient();

}
/***********************FIN DU PROGRAMMES**************************/
