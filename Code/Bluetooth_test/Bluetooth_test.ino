/*
 * Dokumentasi pengerjaan code untuk proyek Pangandaran. 
 * 
 * Versi 1.01
 * 
 * Pengerjaan board menggunakan arduino Nano
 * 
 * Sensor yang digunakan adalah :
 *    1. pH
 *    2. Moistuer
 *    3. Fertility
 *    4. Cahaya
 *    
 *Definisi Port untuk sensor
 *    1. pH ada di A2
 *    2. Moisture A1
 *    3. Fertility Perpauan A1 dan A0
 *    4. Cahaya A3
 *    
 *    
 *Penambahan LCD pada tanggal 16 Juli 2018
 *
 * LCD yang digunakan adalah LCD 128*64 I2C 0X3C
 * 
 *Penambahan Prosedur bahwa data yang dikirim adalah satu string dengan format pH, Moisture, Fertility, Cahaya dengan format "pH123#Mo123#Fe123#Li123"
 *
 *  26 Juli 2018
 *
 *Perbahuran kode untuk pengambilan data pH. Pembaruan pengambilan data dilakukan karena pengambilan data pH tidak begitu akurat dan memiliki jangka nilai yang jauh.
 * 
 *    
 */


#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>       // Lbrary Untuk Grapics LCD
#include <Adafruit_SSD1306.h>   // Library For LCD

/*
 * Define Port Untuk sensor
 */
#define pH A2
#define Moisture A1
#define Fertility A0
#define Cahaya A3
#define LCD 9
int DatapH;
int KalpH;
int DataMoisture;
int KalMos;
int DataFertility;
int KalFer;
int DataCahaya;
int KalLi;
String readFromBLE = "";
int looppH = 0;
int signpH;
int loopFer = 0;
int signFer;
int loopMos = 0;
int signMos;
int loopLi = 0;
int signLi;


/*
 * Define untuk LCD
 */
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

static const unsigned char PROGMEM Logo_Light[] = // Logo Australia Global Alumni
{ B10000000, B00000001,
  B10000001, B10000001,
  B11000001, B10000011,
  B01100001, B10000110,
  B00000000, B00000000,
  B00000011, B11100000,
  B00000111, B11110000,
  B11011111, B11111011,
  B11011111, B11111011,
  B00001111, B11110000,
  B00000111, B11100000,
  B00000000, B00000000,
  B01100001, B10000110,
  B11000001, B10000011,
  B10000001, B10000001,
  B10000001, B10000001 };
static const unsigned char PROGMEM Logo_Moisture[] = // Logo Australia Global Alumni
{ B00000100, B00000000,
  B00000011, B00000000,
  B00000011, B1000000,
  B00000001, B11100000,
  B00000001, B11110000,
  B00000001, B11111000,
  B00000111, B11111100,
  B00011111, B11111110,
  B00111111, B11111111,
  B01111111, B11111111,
  B01111111, B11111111,
  B01111111, B11111111,
  B00111111, B11111110,
  B00011111, B11111110,
  B00001111, B11111000,
  B00000001, B11000000 };

static const unsigned char PROGMEM Logo_pH[] = // Logo Australia Global Alumni
{ B00000000, B00000000,
  B00000011, B00001100,
  B00000011, B00001100,
  B00000011, B00001100,
  B00000011, B11111100,
  B00000011, B00001100,
  B00000011, B00001100,
  B00000011, B11001100,
  B00111111, B11001100,
  B01100011, B1100000,
  B01100011, B11000000,
  B01100000, B11000000,
  B01111111, B11000000,
  B01100000, B00000000,
  B01100000, B00000000,
  B01100000, B00000000 };

static const unsigned char PROGMEM Logo_Fertility[] = // Logo Australia Global Alumni
{ B00000000, B00011111,
  B00000000, B00111111,
  B11000000, B01111111,
  B11111100, B01111110,
  B01111110, B00011100,
  B01111110, B01110000,
  B01111111, B11100000,
  B00000011, B00000000,
  B00001111, B00000000,
  B00011100, B00000000,
  B01111000, B00000000,
  B01110000, B00000000,
  B01110000, B00000000,
  B11110000, B00000000,
  B11100000, B00000000,
  B11100000, B00000000 };


SoftwareSerial BlueT(10, 11); // RX | TX

void setup()   {                
  BlueT.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  pinMode(LCD,INPUT);
  delay(1000);
  testscrolltext();
  //Serial.begin(9600);
}
  
void loop() {

  //if(false)
  //if (bit_is_clear( PIND, PD2))
  //Serial.println(digitalRead(9));
  if(digitalRead(LCD))
  {
    /*
     * Get Data Moisture
     */
    DataMoisture = analogRead(Moisture);
    while(loopMos < 10 ){
      if(loopMos == 0){
        signMos = DataMoisture; 
        loopMos++;
      }
      else if(abs(DataMoisture - signMos) < 150){
        loopMos ++; 
      }
      else{
        loopMos = 0;
      }
      delay(50);
      DataMoisture = analogRead(Moisture);
    }
    loopMos = 0;
    KalMos = DataMoisture / 102.337 ;
    display.clearDisplay();
    /* 
     *  Get Data Fertility
     */
    DataFertility = analogRead(Fertility);
    while(loopFer < 10 ){
      if(loopFer == 0){
        signFer = DataFertility; 
        loopFer++;
      }
      else if(abs(DataFertility - signFer) < 340){
        loopFer ++; 
      }
      else{
        loopFer = 0;
      }
      delay(50);
      DataFertility = analogRead(Fertility);
    }
    loopFer = 0;
    DataFertility = DataFertility + DataMoisture;
    KalFer = (DataFertility / 321.13)*1.428571429;
    /*
     * Get Data Light
     */
    int DataCahaya = analogRead(Cahaya);
    KalLi = DataCahaya /102.3;
    /*
     * Get Data pH
     */
    DatapH = analogRead(pH);
    while(looppH < 10 ){
      if(looppH == 0){
        signpH = DatapH; 
        looppH++;
      }
      else if(abs(DatapH - signpH) < 120){
        looppH ++; 
      }
      else{
        looppH = 0;
      }
      delay(50);
      DatapH = analogRead(pH);
    }
    looppH = 0;
    if(DatapH < 650){
      KalpH = (1023-DatapH)/99.198;
    }
    else{
      KalpH = DatapH / 99.198;
    }
    /*
     * Show to LCD
    */
    display.clearDisplay();
    display.drawBitmap(64, 0,  Logo_Moisture, 16, 16, 1);
    display.drawBitmap(0, 16,  Logo_Fertility, 16, 16, 1);
    display.drawBitmap(0, 0,  Logo_pH, 16, 16, 1);
    display.drawBitmap(64, 16,  Logo_Light, 16, 16, 1);
    display.display();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20,0);
    display.println(KalpH);
    display.setCursor(20,16);
    display.println(KalFer);
    display.setCursor(84,0);
    display.println(KalMos);
    display.setCursor(84,16);
    display.println(KalLi);
    display.display();
    delay(10);
   
  }
  else{
    display.clearDisplay();
    display.display();
    if(BlueT.available()){
      display.clearDisplay();
      while(char(BlueT.read()) != '\r' or char(BlueT.read()) != '\n'){
        readFromBLE = readFromBLE + (char(BlueT.read()));
        delay(4);
      }
      if(readFromBLE == "6171"){
        delay(4500);
        DatapH = analogRead(pH);
        while(looppH < 10 ){
          if(looppH == 0){
            signpH = DatapH;
            looppH++;
          }
          else if(abs(DatapH - signpH) < 120){
            looppH ++; 
          }
          else{
            looppH = 0;
          }
          delay(50);
          DatapH = analogRead(pH);
        }
        looppH = 0;
        delay(10);
        BlueT.println("pH");
        delay(50);
        BlueT.println(DatapH);
        delay(50);
        BlueT.println("#");
      }
      if(readFromBLE == "9024"){
        delay(4500);
        DataMoisture = analogRead(Moisture);
        while(loopMos < 10 ){
          if(loopMos == 0){
            signMos = DataMoisture; 
            loopMos++;
          }
          else if(abs(DataMoisture - signMos) < 150){
            loopMos ++; 
          }
          else{
            loopMos = 0;
          }
          delay(50);
          DataMoisture = analogRead(Moisture);
        }
        loopMos = 0;
        delay(10);
        BlueT.println("Mo");
        delay(50);
        BlueT.println(DataMoisture);
        delay(50);
        BlueT.println("#");
      }
      if(readFromBLE == "3024"){
        delay(4500);
        DataFertility = analogRead(Fertility);
        while(loopFer < 10 ){
          if(loopFer == 0){
            signFer = DataFertility; 
            loopFer++;
          }
          else if(abs(DataFertility - signFer) < 340){
            loopFer ++; 
          }
          else{
            loopFer = 0;
          }
          delay(50);
          DataFertility = analogRead(Fertility);
        }
        loopFer = 0;
        DataMoisture = analogRead(Moisture);
        while(loopMos < 10 ){
          if(loopMos == 0){
            signMos = DataMoisture;
            loopMos ++;
          }
          else if(abs(DataMoisture - signMos) < 150){
            loopMos ++; 
          }
          else{
            loopMos = 0;
          }
          delay(50);
          DataMoisture = analogRead(Moisture);
        }
        loopMos = 0;
        DataFertility = DataFertility + DataMoisture;
        delay(10);
        BlueT.println("Fe");
        delay(50);
        BlueT.println(DataFertility);
        delay(50);
        BlueT.println("#");
      }
      if(readFromBLE == "8024"){
        delay(4500);
        DataCahaya = analogRead(Cahaya);
        delay(10);
        BlueT.println("Li");
        delay(50);
        BlueT.println(DataCahaya);
        delay(50);
        BlueT.println("#");
      }
      if(readFromBLE == "8029"){
        delay(4500);
        GetAllDataSensor();
      }
    }
    readFromBLE = "";
  }
}

void sendData_BLE(int pH, int Mos, int Fer, int lig){
  BlueT.println("pH");
  delay(50);
  BlueT.println(pH);
  delay(50);
  BlueT.println("#");
  delay(50);
  BlueT.println("Mo");
  delay(50);
  BlueT.println(Mos);
  delay(50);
  BlueT.println("#");
  delay(50);
  BlueT.println("Fe");
  delay(50);
  BlueT.println(Fer);
  delay(50);
  BlueT.println("#");
  delay(50);
  BlueT.println("Li");
  delay(50);
  BlueT.println(lig);
  delay(50);
  BlueT.println("#");
}

void testscrolltext(void) {
  display.setTextSize(1.8);
  display.setTextColor(WHITE);
  display.setCursor(30,0);
  display.println("Smart");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,8);
  display.println("Coconut");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,24);
  display.println("Plantation");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.display();
  delay(500);
 
  display.startscrollright(0x00, 0xFF);
  delay(1000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0xFF);
  delay(1000);
  display.stopscroll();
}

void GetAllDataSensor(){
  DataMoisture = analogRead(Moisture);
    while(loopMos < 10 ){
      if(loopMos == 0){
        signMos = DataMoisture; 
      }
      else if(abs(DataMoisture - signMos) < 150){
        loopMos ++; 
      }
      else{
        loopMos = 0;
      }
      delay(150);
      DataMoisture = analogRead(Moisture);
    }
    loopMos = 0;
    
    DataFertility = analogRead(Fertility);
        while(loopFer < 10 ){
          if(loopFer == 0){
            signFer = DataFertility; 
          }
          else if(abs(DataFertility - signFer) < 340){
            loopFer ++; 
          }
          else{
            loopFer = 0;
          }
          delay(150);
          DataFertility = analogRead(Fertility);
        }
        loopFer = 0;

    DatapH = analogRead(pH);
        while(looppH < 10 ){
          if(looppH == 0){
            signpH = DatapH; 
          }
          else if(abs(DatapH - signpH) < 120){
            looppH ++; 
          }
          else{
            looppH = 0;
          }
          delay(150);
          DatapH = analogRead(pH);
        }
        looppH = 0;
        
    DataCahaya = analogRead(Cahaya);
    sendData_BLE(DatapH,DataMoisture,DataFertility,DataCahaya);
}

