#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <String.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define led 2
#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_CS1   2  // chon slave
#define HSPI_CS2   15

#define BT1 19 // bt gui slave2
#define BT2 5

#define BT3 23 // bt gui slave 1
#define BT4 16 //

#define BT5 17

long last = 0;
int nhietdo = 0; int doam = 0;
int TB1 = 0;int TB2 = 0;
int TB3 = 0;int TB4 = 0;
void XulyJson(String JsonDATA);

String data_uart = "";
char rx_data_SPI[100];

void setup() {
  Serial.begin(115200); 
  pinMode(HSPI_CS1, OUTPUT); //HSPI SS
  digitalWrite(HSPI_CS1, HIGH) ;

  pinMode(HSPI_CS2, OUTPUT); //HSPI SS
  digitalWrite(HSPI_CS2, HIGH) ;
  
  pinMode(led, OUTPUT);
  pinMode(BT1, INPUT_PULLUP);
  pinMode(BT2, INPUT_PULLUP);

  pinMode(BT3, INPUT_PULLUP);
  pinMode(BT4, INPUT_PULLUP);
  //pinMode(BT5, INPUT_PULLUP);
  digitalWrite(led, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("TBNV & KTGN");
}


void DataJson(unsigned int ND ,unsigned int DA, unsigned int TB1, unsigned int TB2, unsigned int TB3,  unsigned int TB4){
  char JSON[100] = "";
  char Str_ND[10] = ""; 
  char Str_DA[10] = "";
  char Str_TB1[10] = "";
  char Str_TB2[10] = "";
  char Str_TB3[10] = "";
  char Str_TB4[10] = "";


  sprintf(Str_ND, "%d", ND); 
  sprintf(Str_DA, "%d", DA);
  sprintf(Str_TB1, "%d", TB1);
  sprintf(Str_TB2, "%d", TB2);
  sprintf(Str_TB3, "%d", TB3);
  sprintf(Str_TB4, "%d", TB4);

  strcat(JSON, "{\"ND\":\"");
  strcat(JSON, Str_ND); strcat(JSON, "\",");
  strcat(JSON, "\"DA\":\"");
  strcat(JSON, Str_DA); strcat(JSON, "\",");

  strcat(JSON, "\"TB1\":\"");
  strcat(JSON, Str_TB1); strcat(JSON, "\",");
  strcat(JSON, "\"TB2\":\"");
  strcat(JSON, Str_TB2); strcat(JSON, "\",");
  strcat(JSON, "\"TB3\":\"");
  strcat(JSON, Str_TB3); strcat(JSON, "\",");
  strcat(JSON, "\"TB4\":\"");
  strcat(JSON, Str_TB4); strcat(JSON, "\"}\n");

 
  for (int i = 0; i < sizeof JSON; i++ )
  {
       rx_data_SPI[i] = SPI.transfer(JSON[i]); 
       delay(2);
  }
  SPI.transfer('\n'); 
   Serial.printf("%s", JSON); // send to PC
   for(int i = 0; i<sizeof JSON; i++ ){
      JSON[i] = 0;
      rx_data_SPI[i] = 0;
    }
    XulyJson(rx_data_SPI);
}

void button(){
  if (digitalRead(BT1) == 0)
    { 
      delay(20);
      if (digitalRead(BT1) == 0)
      {  
        if(TB1 == 0)
        {
          TB1 = 1;
        }
        else 
        {
          TB1 = 0;
        }
        SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS1);
        digitalWrite(HSPI_CS1, LOW) ;
        DataJson(nhietdo, doam, TB1, TB2, TB3, TB4); 
        digitalWrite(HSPI_CS1, HIGH) ; 
        while(!digitalRead(BT1));
        digitalWrite(led, !digitalRead(led));
      }
    }
    if (digitalRead(BT2) == 0)
    { 
      delay(20);
      if (digitalRead(BT2) == 0)
      {   
        if(TB2 == 0)
        {
          TB2 = 1;
        }
        else 
        {
          TB2 = 0;
        }
        SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS1);
        digitalWrite(HSPI_CS1, LOW) ;
        DataJson(nhietdo, doam, TB1, TB2, TB3, TB4); 
        digitalWrite(HSPI_CS1, HIGH) ; 
        while(!digitalRead(BT2));
        digitalWrite(led, !digitalRead(led));
      }
    }
    ///////////////////////////////////////
    if (digitalRead(BT3) == 0)
    { 
      delay(20);
      if (digitalRead(BT3) == 0)
      {   
        if(TB3 == 0)
        {
          TB3 = 1;
        }
        else 
        {
          TB3 = 0;
        }
        SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS2);
        digitalWrite(HSPI_CS2, LOW) ;
        DataJson(nhietdo, doam, TB1, TB2, TB3, TB4); 
        digitalWrite(HSPI_CS2, HIGH) ; 
        while(!digitalRead(BT3));
        digitalWrite(led, !digitalRead(led));
      }
    }
    if (digitalRead(BT4) == 0)
    { 
      delay(20);
      if (digitalRead(BT4) == 0)
      {   
        if(TB4 == 0)
        {
          TB4 = 1;
        }
        else 
        {
          TB4 = 0;
        }
        SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS2);
        digitalWrite(HSPI_CS2, LOW) ;
        DataJson(nhietdo, doam, TB1, TB2, TB3, TB4); 
        digitalWrite(HSPI_CS2, HIGH) ;  
        while(!digitalRead(BT4));
        digitalWrite(led, !digitalRead(led));
      }
    }
}

void XulyJson(String JsonDATA) // xu ly data nhan tu PC và SPI
{
  
  const size_t capacity = JSON_OBJECT_SIZE(2) + 256;
  DynamicJsonDocument JSON(capacity);
  DeserializationError error = deserializeJson(JSON, JsonDATA);
  if(JSON["TB1"] == "0"){
    TB1 = 0;
  }else if(JSON["TB1"] == "1"){
    TB1 = 1;
  }
  
   if(JSON["TB2"] == "0"){
    TB2 = 0;
  }else if(JSON["TB2"] == "1"){
    TB2 = 1;
  }
   if(JSON["TB3"] == "0"){
    TB3 = 0;
  }else if(JSON["TB3"] == "1"){
    TB3 = 1;
  }
   if(JSON["TB4"] == "0"){
    TB4 = 0;
  }else if(JSON["TB4"] == "1"){
    TB4 = 1;
  }
   if(JSON["TB4"] == "0"){
    TB4 = 0;
  }else if(JSON["TB4"] == "1"){
    TB4 = 1;
  }
   if(JSON["ND"] != nhietdo){
    nhietdo  = JSON["ND"];
  }else if(JSON["TB4"] != doam){
    doam  = JSON["DA"];
  }
    SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS1);
    DataJson(nhietdo, doam, TB1, TB2, TB3, TB4); 
    SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS2);
    DataJson(nhietdo, doam, TB1, TB2, TB3, TB4);
    JSON.clear();

}
void read_uart(){ // doc uart tu PC
  while(Serial.available()){
    char x =(char)Serial.read();
    if(x != '\n'){
       data_uart += x;
    }
    else 
    {
      XulyJson(data_uart); 
      data_uart = "";
    }
  }
  if(millis() - last >= 1000){
    
    lcd.setCursor(0, 1); 
    lcd.print("DA: ");
    lcd.print(doam);
    lcd.print("%");
    lcd.setCursor(7, 1);
    
    lcd.print(" ");
    lcd.print("ND: ");
    lcd.print(nhietdo);
    lcd.print((char)223);
    lcd.print("C");
    
    DataJson(nhietdo, doam, TB1, TB2, TB3, TB4); 
    last = millis();
  }
}
void loop() {
    button();
    read_uart();
}
