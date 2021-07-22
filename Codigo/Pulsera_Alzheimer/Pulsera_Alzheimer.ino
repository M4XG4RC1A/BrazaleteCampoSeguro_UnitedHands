#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

String Ubicacion;
//Ejemplos de Ubicacion segura
float LatMin = 0;
float LatMax = 100;
float LngMin = 0;
float LngMax = 100;

//Numero de destino
#define CONFIG_GSM_SMS_DESTINATION "+52155XXXXXXXX"

SoftwareSerial swseri = SoftwareSerial(8, 9);
dafruit_FONA fona = Adafruit_FONA(10);

TinyGPSPlus gps;
SoftwareSerial ss(3, 4);

void setup() {
  // put your setup code here, to run once:
  pinMode(5, INPUT_PULLUP);
  swseri.begin(9600);
  ss.begin(4800);
  pinMode(7,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gps.location.isValid())
  {
    Ubicacion = "Lat " + gps.location.lat() + ", Long"+ gps.location.lng();
    
    if(Condicion(gps.location.lat(), gps.location.lng(), LatMin, LatMax, LngMin, LngMax)){
      MandarMensaje("Ya me voy",Ubicacion);
      delay(1000);
    }
    
    delay(100);
    
    if(Serial.available()){
      String Cadena = "";
      char lec;
      while(Serial.available()){
      lec = Serial.read();
      Cadena += lec;
      }
      Serial.println(Cadena);
      if(Cadena=="¿Donde estas?" || Cadena=="Ya Llegue" || Cadena=="Ubicacion" || lec=='2' || lec=='M'){
        MandarMensaje("Estoy en ",Ubicacion);
      }
    }
    
  }
}

bool Condicion(float Lat, float Lng, float Lm, float LM, float Nm, float NM){
  bool Send = False;
  if (Lat > LM || Lat < Lm){
    Send = true;
  }
  if (Lng > NM || Lng < Nm){
    Send = true;
  }
  return Send;
}

void MandarMensaje(String Men, String Ub){
  String Cadena = Men+Ub;
  if (!fona.sendSMS(CONFIG_GSM_SMS_DESTINATION, Cadena)) {
    Serial.println(F("ERROR"));
  } else {
    Serial.println(F("ENVIADO"));
  }
}
