#include <SoftwareSerial.h>

#define ESP8266_rx 6
#define ESP8266_tx 7

SoftwareSerial ESP8266(ESP8266_rx, ESP8266_tx); //rx tx

const char keyword_OK[] = "OK";
const char keyword_ARROW[] = ">";
const char keyword_SEND_OK[] = "SEND OK";
const char keyword_CURLY[] = "{";
const char keyword_CURLY_END[] = "}";
const char get_url[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=NISVZ2HHBDL8OQ3X HTTP/1.0\r\n\r\n";
const char post_url1[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=BNVI2YTP11SBYAVA HTTP/1.0\r\n\r\n";

byte payloadsize = 0;
byte counter = 0;
char payload[150];
char save_data[223];
String json_data = "";


//DEFINE FUNCTIONS

void setupESP(){
  ESP8266.print("AT\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("AT ERROR");
  }
  else{
    Serial.println("AT OK");
  }
  clear_serial(3000);
  
  ESP8266.print("AT+RST\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("AT+RST ERROR");
  }
  else{
    Serial.println("AT+RST OK");
  }
  clear_serial(3000);
  
  ESP8266.print("AT+CWMODE=1\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("CWMODE ERROR");
  }
  else{
    Serial.println("CWMODE SET TO 1");
  }
  clear_serial(3000);

  ESP8266.print("AT+CWJAP=\"");
  ESP8266.print("Capstone");
  ESP8266.print("\",\"");
  ESP8266.print("secure123");
  ESP8266.print("\"\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("ERROR");
  }
  else{
    Serial.println("WIFI CONNECTED");
  }
  clear_serial(3000);

  ESP8266.print("AT+CIPMUX=1\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("CIPMUX ERROR");
  }
  else{
    Serial.println("CIPMUX SET TO 1");
  }
  clear_serial(3000);
  Serial.println("SETUP COMPLETE");

};

void send_get(){
  ESP8266.print("AT+CIPSTART=0,\"");
  ESP8266.print("TCP");
  ESP8266.print("\",\"");
  ESP8266.print("api.thingspeak.com");
  ESP8266.print("\",80");
  ESP8266.print("\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("CIPSTART ERROR");
  }
  else{
    Serial.println("CIPSTART OK");
  }
  clear_serial(3000);

  counter = 0;
  for(int i=0;i<sizeof(get_url)-1;i++){
    payload[i] = get_url[i];
    counter++;
  }
  Serial.print(counter);
  payloadsize = counter;

  ESP8266.print("AT+CIPSEND=0,");
  ESP8266.print(payloadsize);
  ESP8266.print("\r\n");
  if(!find_key(keyword_ARROW,1,0)){
    Serial.println("CIPSEND ERROR");
  }
  else{
    Serial.println("READY TO SEND DATA");
    for(int i=0;i<payloadsize;i++){
      ESP8266.print(payload[i]);
      //Serial.print(payload[i]);
    }
    //store_data();
    
    if(!find_key(keyword_CURLY,1,0)){
      Serial.println("FAILED TO SEND");
    }
    else{
      Serial.println("SENT GET");
      // add functionality so save JSON data
      if(!find_key(keyword_CURLY_END,1,1)){
        Serial.println("FAILED TO SAVE");
      }
      else{
        Serial.println("DATA SAVED");
      }
    }
    clear_serial(3000);
    
    
  }
  clear_serial(3000);
}

void send_post(){
  ESP8266.print("AT+CIPSTART=0,\"");
  ESP8266.print("TCP");
  ESP8266.print("\",\"");
  ESP8266.print("api.thingspeak.com");
  ESP8266.print("\",80");
  ESP8266.print("\r\n");
  if(!find_key(keyword_OK,2,0)){
    Serial.println("CIPSTART ERROR");
  }
  else{
    Serial.println("CIPSTART OK");
  }
  clear_serial(3000);

  counter = 0;
  for(int i=0;i<sizeof(post_url1)-1;i++){
    payload[i] = post_url1[i];
    counter++;
  }
  Serial.print(counter);
  payloadsize = counter;

  ESP8266.print("AT+CIPSEND=0,");
  ESP8266.print(payloadsize);
  ESP8266.print("\r\n");
  if(!find_key(keyword_ARROW,1,0)){
    Serial.println("CIPSEND ERROR");
  }
  else{
    Serial.println("READY TO SEND DATA");
    for(int i=0;i<payloadsize;i++){
      ESP8266.print(payload[i]);
      //Serial.print(payload[i]);
    }
    if(!find_key(keyword_SEND_OK,7,0)){
      Serial.println("FAILED TO SEND");
    }
    else{
      Serial.println("SENT POST");
    }
    clear_serial(3000);
    
  }
  clear_serial(3000);
}

void clear_serial(int wait){
  char temp;
  while(1){
    while(ESP8266.available()>0){
        temp = ESP8266.read();
    }
    delay(wait);
    if(ESP8266.available()>0){
      continue;
    }
    else break;
  }
}

boolean find_key(const char keyword[], int keysize, int mode){
  char data_in[20];
  int timeout_start_val;
  int pos = 0;
  
  for(byte i=0;i<keysize;i++){
    timeout_start_val = millis();
    while(!ESP8266.available()){
      //do nothing
      
    }
    data_in[i]=ESP8266.read();
    if(mode==1){
        save_data[pos] = data_in[i];
        pos++;
    } 
  }
  while(1){
    for(byte i=0;i<keysize;i++){
      Serial.print(data_in[i]);
      if(keyword[i] != data_in[i]){
        break;
      }
      if(i==keysize-1){
        return 1;
      }
    }
    
    for(byte i=0;i<keysize-1;i++){
      data_in[i] = data_in[i+1];
    }
    timeout_start_val = millis();
    while(!ESP8266.available()){
      //do nothing
    }
    data_in[keysize-1] = ESP8266.read(); 
    if(mode==1){
        save_data[pos] = data_in[keysize-1];
        pos++;
    }
  }
  return 1;
}

void store_data(){
  char json_array[500];
  counter = 0;
  
  for(byte i=0;i<500;i++){
    while(!ESP8266.available()){
      //do nothing
    }
    json_array[i]=ESP8266.read();
    
    Serial.print(json_array[i]); 
   
  } 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ESP8266_rx,INPUT);
  pinMode(ESP8266_tx,OUTPUT);
  ESP8266.begin(9600);
  ESP8266.listen();
  Serial.begin(9600);
  delay(5000);
  setupESP();
  send_get();
  send_post();

  for(byte i=0;i<223;i++){
    Serial.print(save_data[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
}
