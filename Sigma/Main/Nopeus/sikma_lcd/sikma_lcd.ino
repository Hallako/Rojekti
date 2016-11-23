#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <EEPROM.h>
#include <MsTimer2.h>

#define cs   10
#define dc   9
#define rst  8

TFT TFTscreen = TFT(cs, dc, rst);
unsigned long t,s,m,h,seconds,secondsoff,oldseconds;
int sensorPin = 5, v=1,del=0,mph = 0,fi,tk=22,f,kierrokset = 0,ekierrokset;
float start, matka = 0, revs, elapsed,ematka, time,matkat,matkar,matkaoff,matkaold,kmh,huippu=0,matkav,vert1,vert2;
char oldsensor[6], Matka[6], sensorPrintout[6], secc[4], mnc[4], hrc[4],MatkaT[6],Huippu[6]={0};
String oldVal,sensorVal,matkaVal,sec,minn,hou,matkaT,hUippu;
char minuutit[10], sekunnit[10], tunnit[10];

void setup() {
  TFTscreen.begin();
  tftSetup();
  Serial.begin(9600);
  ADCSRB = 0;
  ACSR =						//Analog comparaattorin alustus
   (0 << ACD) |				
   (0 << ACBG) |   
   (0 << ACO) |    
   (1 << ACI) |    
   (1 << ACIE) |   
   (0 << ACIC) |   
   (1 << ACIS1) | 
   (1 << ACIS0);   
  start=millis();
  ekierrokset = sizeof(int);
  EEPROM.get(100, ekierrokset);
  ematka = ekierrokset * (tk*2.54*3.1459)/100000;
  matkat=ematka;
  MsTimer2::set(3000, nollaus); 
  MsTimer2::start();
}

ISR(ANALOG_COMP_vect) {
  fi=1;
  elapsed=millis()-start;
  start=millis();
    float revs = 60000/elapsed;
    kmh = ((tk*2.54*3.1459)*revs*60/100000);
  kierrokset += 1;
  ekierrokset += 1;
  matka = kierrokset * (tk*2.54*3.1459)/100000;
  ematka = ekierrokset * (tk*2.54*3.1459)/100000;
 if(mph==1)
  {
    kmh = kmh * 0.621371;
    matka = matka * 0.621371;
  }
  sensorVal = String(kmh);
  ACSR=(0 << ACIE);
}


void reset()
{
	secondsoff = millis() / 1000;
	matkaoff = matka;
}

void nollaus()
{
	vert1=kmh;
	if(vert1==vert2){
	kmh=0;	
	sensorVal = String(kmh);
	}
	vert2=kmh;
} 
void loop() 
{
  if(fi==1){
  delay(40);
  fi=0;
  delay(40);
  ACSR = (1 << ACI);
  ACSR = (1 << ACIE);
  }
  
  matkar=matka-matkaoff;
  del=0;
  f = 0;
  while(digitalRead(5) == HIGH)
  {
    del=del+1;
	delay(1);
 f=1;
  }
  if(del<500 && f==1){
	  v+=1;
	if(v>4){
	v=1;
	
  }
  }
  if(del>500 && f==1){
	secondsoff = millis() / 1000;
	matkaoff = matka;
	huippu=0;
  }
  if(del>5000 && f==1){
	for(;;){
	while(digitalRead(5) == HIGH)
    {
 
    }
    if(del<500 && f==1){
	
		
	}
  }
  }
  seconds = (millis() / 1000)-secondsoff;  	//secondsoff = offset resetistä.
 
  if(sensorVal != oldVal)
  {
  sensorVal.toCharArray(sensorPrintout, 6);
  
  TFTscreen.setTextSize(4);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(oldsensor, 0, 20);

  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(sensorPrintout, 0, 20);
   
//ottaa vanhan arvon talteen näytön tyhjennystä varten
   oldVal = String (sensorPrintout);
   oldVal.toCharArray(oldsensor, 6);


 
 Serial.print(sensorPrintout[0]);
 Serial.print(sensorPrintout[1]);
 Serial.print(sensorPrintout[2]);
 Serial.print(sensorPrintout[3]);
 Serial.print(sensorPrintout[4]);
 Serial.println(sensorPrintout[5]);
 if(mph == 1)
 {
	 TFTscreen.setTextSize(1);
    TFTscreen.stroke(1000, 1000, 1000);
  TFTscreen.text("MP/h", 130, 41);
 }
 else
 {
  TFTscreen.setTextSize(1);
    TFTscreen.stroke(1000, 1000, 1000);
  TFTscreen.text("KM/h", 130, 41);
 }
  }
  
  if(huippu < kmh && huippu <1000)
  {
    TFTscreen.setTextSize(1);
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(Huippu, 130, 0); 
    huippu = kmh;
    hUippu = String (huippu);
    hUippu.toCharArray(Huippu, 6);
    TFTscreen.stroke(1000, 1000, 1000);
    TFTscreen.text(Huippu, 130, 0);
  } 
  if(roundf((matkat+0.2) * 100) < roundf(ematka * 100)){ //tallennetaan arvo eepromiin 200m välein
	EEPROM.put(100, ekierrokset);
	Serial.print("eeprom.write");
	matkat=ematka;
  }
	switch (v){
	case 1:
	
	
  if(roundf(matkar * 100) / 1 != roundf(matkaold * 100) / 1)
  {
  Serial.print(matkar);
  Serial.print("\t");
  Serial.print(matkaold);
  Serial.print("\t");
	
	TFTscreen.stroke(0, 0, 0);
  TFTscreen.setTextSize(1);
  TFTscreen.text("TOTAL", 60, 65);

  TFTscreen.setTextSize(2);
	TFTscreen.text(Matka, 0, 60);
  TFTscreen.text(tunnit, 0, 60);
  TFTscreen.text(MatkaT, 0, 60);
	//matkar=matka-matkaoff;
	
 // Serial.print(matkaold);
	matkaVal = String (matkar);
    matkaVal.toCharArray(Matka, 6);
    TFTscreen.stroke(255, 255, 255);

    TFTscreen.setTextSize(2);
    TFTscreen.text(Matka, 0, 60);
    matkaold=matkar;
  }
    if(mph == 1)
    {
      TFTscreen.setTextSize(1);
      TFTscreen.text("miles TRIP", 60, 65);
    }
    else
    {
      TFTscreen.setTextSize(1);
      TFTscreen.text("km TRIP", 60, 65);
    }
 Serial.println("CASE1");
 
	break;
 
	case 2:
	
	if(seconds!=oldseconds){
 Serial.println("CASE2");
 TFTscreen.setTextSize(2);
 Serial.print(secondsoff);
 
	
	oldseconds=seconds;
  	t = seconds;					// h= hours m=minutes s=seconds 
    s = t % 60;
    t = (t - s)/60;
    m = t % 60;
    t = (t - m)/60;
    h = t;
TFTscreen.setTextSize(1);
  TFTscreen.stroke(0, 0, 0);
 TFTscreen.text("miles TRIP", 60, 65);
 TFTscreen.text("km TRIP", 60, 65);
 TFTscreen.setTextSize(2);
 TFTscreen.text(Matka, 0, 60);
 TFTscreen.text(tunnit, 0, 60);
 TFTscreen.text(MatkaT, 0, 60);
  
	ultoa(s,secc,10);						//unsigned long = string
	ultoa(m,mnc,10);
	ultoa(h,hrc,10);
	
	Serial.println(secc);
	Serial.println(mnc);

  strcpy(tunnit, hrc);
  strcat(tunnit, ":");
  strcpy(minuutit, mnc);
  strcat(minuutit, ":");
  strcat(tunnit, minuutit);
  strcpy(sekunnit, secc);
  strcat(tunnit, sekunnit);
  Serial.println(tunnit);
  
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.text(tunnit, 0, 60);
	}
	break;
	
	case 3:
  
  if(roundf(matkav * 100) != roundf(ematka * 100))
  {
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(Matka, 0, 60);
  TFTscreen.text(tunnit, 0, 60);
  TFTscreen.text(MatkaT, 0, 60);
 
  matkav = ematka;
  matkaT = String (ematka);
  matkaT.toCharArray(MatkaT, 6);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text(MatkaT, 0, 60);
  }
  TFTscreen.setTextSize(1);
  TFTscreen.text("TOTAL", 60, 65);
  Serial.println("CASE3");
  break;
  
  case 4:
  
  
  
  
  break;
}

}
  void tftSetup()
  {
  // clear the screen with a black background
  TFTscreen.background(0,0,0);
  TFTscreen.stroke(1000, 200, 200);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("SIKMA ", 0, 0);
  TFTscreen.line(0, 55, 160, 55);    
  }



