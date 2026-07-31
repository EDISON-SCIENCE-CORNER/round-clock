
//EDISON SCIENCE CORNER
//EDISON SCIENCE CORNER

#include <DS3232RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include <FastLED.h>

DS3232RTC myRTC(false);

#define NUM_LEDS 30
#define COLOR_ORDER RGB  // Define color order for your strip
#define LED_PIN 6 // Data pin for led comunication
#define DST_PIN 5  // Define DST adjust button pin
#define MIN_PIN 3  // Define Minutes adjust button pin

#define HUR_PIN 2  // Define Hours adjust button pin
#define BRI_PIN 4 // Define Light sensor pin

CRGB leds[NUM_LEDS];
byte digits[12][7] = {
  {1,0,1,1,1,1,1},
  {1,0,1,0,0,0,0},
  {0,1,1,1,0,1,1},
  {1,1,1,1,0,0,1},
  {1,1,1,0,1,0,0},
  {1,1,0,1,1,0,1},
  {1,1,0,1,1,1,1},
  {1,0,1,1,0,0,0},
  {1,1,1,1,1,1,1},
  {1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1},
  {0,1,1,1,1,0,0}}; 
                       
                       
bool Dot = true;  //Dot state
bool DST = true; //DST state// false to true
bool TempShow = false;
int last_digit = 0;

// int ledColor = 0x0000FF; // Color used (in hex)
long ledColor = CRGB::DarkOrchid; // Color used (in hex)
//long ledColor = CRGB::MediumVioletRed;
//Random colors i picked up
long ColorTable[21] = {
  CRGB::Amethyst,  //white
  CRGB::Aqua,    //pink
  CRGB::Blue,     //Blue
  CRGB::Chartreuse,// Gold
  CRGB::DarkGreen, //Red
  CRGB::DarkMagenta,//Aqua
  CRGB::DarkOrange,  // yellow green
  CRGB::DeepPink,  //Aqua
  CRGB::Fuchsia,  //Sea blue
  CRGB::Gold,    //Gold
  CRGB::GreenYellow,//off white
  CRGB::LightCoral,//white
  CRGB::Tomato,//white
  CRGB::Salmon,//Pure white
  CRGB::Red,// Drak Green
  CRGB::Orchid,//blue white
  CRGB::Sienna,//yellow white
  CRGB::Purple,// aqua
  CRGB::DarkOrange,//yellow green
  CRGB::FloralWhite,//white
  CRGB::Yellow  //yellow
};


void setup(){ 
  Serial.begin(9600); 
  Wire.begin();
  myRTC.begin();
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(75); // Set initial brightness
  pinMode(DST_PIN, INPUT_PULLUP); // Define DST adjust button pin
  pinMode(MIN_PIN, INPUT_PULLUP); // Define Minutes adjust button pin
  pinMode(HUR_PIN, INPUT_PULLUP); // Define Hours adjust button pin
  pinMode(BRI_PIN, INPUT_PULLUP); // Define bright adjust 
  TempShow = false; // do not show temperature 
} 

// Get time in a single number, if hours will be a single digit then time will be displayed 155 instead of 0155
int GetTime(){
  tmElements_t Now;
  myRTC.read(Now);
  int hour=Now.Hour;
  int minute=Now.Minute;
  int second =Now.Second;
  if (second % 2==0)
   {Dot = false;}
    else {Dot = true;};
 
  // Apply DST adjust before 12-hour conversion
  if (DST){   
    hour = (hour + 1) % 24;
  };

  if (hour >= 12) {
    hour -= 12;
  }
  if (hour == 0) {
    hour = 12;
  }
  return (hour*100+minute);
};

 // Check Light sensor and set brightness accordingly
void BrightnessCheck(){
  const byte sensorPin = BRI_PIN; // light sensor pin
  const byte brightnessLow = 20; // Low brightness value
  const byte brightnessHigh = 75; // High brightness value
  int sensorValue = digitalRead(sensorPin); // Read sensor
  if (sensorValue == 0) {FastLED.setBrightness(brightnessHigh);}
  else {FastLED.setBrightness(brightnessLow);} 
  }; 
// Convert time to array needed for display 
void TimeToArray(){
  int Now = GetTime();  // Get time
  
  int cursor = 30; // last led number
  
  // Serial.print("Time is: ");Serial.println(Now);
  // DST is now handled in GetTime()
  if (Dot){leds[28]=ledColor;
           leds[29]=ledColor;}   
   else  {leds[28]=0x000000;
           leds[29]=0x000000;
    };
    
  for(int i=1;i<=4;i++){
    int digit = Now % 10; // get last digit in time
    if (i==1){
    //  Serial.print("Digit 4 is : ");Serial.print(digit);Serial.print(" ");

      cursor = 21;
      
      for(int k=0; k<=6;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();

      if (digit != last_digit)
      {
        cylon();
        ledColor =  ColorTable[random(21)];
      }
      last_digit = digit;
      
      }
    else if (i==2){
      // Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");

      cursor =14;
      
      for(int k=0; k<=6;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();
      }
     else if (i==3){
      // Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
      cursor =7;
      for(int k=0; k<=6;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      // Serial.println();
      }
    else if (i==4){
      // Serial.print("Digit 1 is : ");Serial.print(digit);Serial.print(" ");
      cursor =0;
      if(digit !=0){
         for(int k=0; k<=6;k++){ 
        // Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      }
      if(digit ==0){
      for(int k=0; k<7;k++){ 
        leds[k]=0x000000;
        };
      
      // Serial.println();
      }
    }
     Now /= 10;
  }; 
};

// Convert temp to array needed for display 
void TempToArray(){
  tmElements_t tm;
  myRTC.read(tm);

  if (tm.Second != 27) {
    TempShow = false;
    return;
  }
  TempShow = true;
  int t = myRTC.temperature();
  int celsius = t * 25; // Equivalent to (t / 4.0) * 100 using integer math
  bool isNegative = (celsius < 0);
  celsius = abs(celsius);

  Serial.print("Temp is: ");Serial.println(celsius);
  
  int cursor = 30; // last led number
    
  leds[28]=0x000000;
  leds[29]=0x000000;
      
  for(int i=1;i<=4;i++){
    int digit = celsius % 10; // get last digit in temp
    if (i==1){
      Serial.print("Digit 4 is : ");Serial.print(digit);Serial.print(" ");

      cursor = 21;
      
      for(int k=0; k<=6;k++){ 
        Serial.print(digits[10][k]);
        if (digits[10][k]== 1){leds[cursor]=ledColor;}
         else if (digits[10][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
    }
    else if (i==2){
      Serial.print("Digit 3 is : ");Serial.print(digit);Serial.print(" ");

      cursor =14;
      
      for(int k=0; k<=6;k++){ 
        Serial.print(digits[11][k]);
        if (digits[11][k]== 1){leds[cursor]=ledColor;}
         else if (digits[11][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
      }
    else if (i==3){
      Serial.print("Digit 2 is : ");Serial.print(digit);Serial.print(" ");
      cursor =7;
      for(int k=0; k<=6;k++){ 
        Serial.print(digits[digit][k]);
        if (digits[digit][k]== 1){leds[cursor]=ledColor;}
         else if (digits[digit][k]==0){leds[cursor]=0x000000;};
         cursor ++;
        };
      Serial.println();
      }
    else if (i==4){
      Serial.print("Digit 1 is : ");Serial.print(digit);Serial.print(" ");
      cursor =0;
      if (isNegative) {
        // Display minus sign (only segment G / index 1 is ON)
        for(int k=0; k<=6; k++){ 
          if (k == 1) { leds[cursor] = ledColor; }
          else { leds[cursor] = 0x000000; }
          cursor++;
        }
      } else {
        // Normal leading zero blanking
        if(digit != 0){
          for(int k=0; k<=6;k++){ 
            if (digits[digit][k]== 1){leds[cursor]=ledColor;}
             else if (digits[digit][k]==0){leds[cursor]=0x000000;};
             cursor ++;
            };
        }
        else if(digit ==0){
          for(int k=0; k<7;k++){ 
            leds[k]=0x000000;
          };
          Serial.println();
        }
      }
    }
    celsius /= 10;
  }; 
};


void DSTcheck(){
   int buttonDST = digitalRead(DST_PIN);
   // Serial.print("DST is: ");Serial.println(DST);
   if (buttonDST == LOW){
    if (DST){
      DST=false;
      // Serial.print("Switching DST to: ");Serial.println(DST);
      }
      else if (!DST){
        DST=true;
        // Serial.print("Switching DST to: ");Serial.println(DST);
      };
   delay(500);   
   };
  }

void TimeAdjust(){
  int buttonH = digitalRead(HUR_PIN);
  int buttonM = digitalRead(MIN_PIN);
  if (buttonH == LOW || buttonM == LOW){
    delay(500);
    tmElements_t Now;
    myRTC.read(Now);
    int hour=Now.Hour;
    int minutes=Now.Minute;
    int second =Now.Second;
      if (buttonH == LOW){
        if (Now.Hour== 23){Now.Hour=0;}
          else {Now.Hour += 1;};
        }else {
          if (Now.Minute== 59){Now.Minute=0;}
          else {Now.Minute += 1;};
         // if (Now.Hour > 12){Now.Hour = Now.Hour -12;}  // 24 to 12 hr  
         // else {Now.Hour = Now.Hour ;}; 
          };
    myRTC.write(Now); 
    }
  }

/* coool effect function*/
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }


void cylon () {
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}
  
void loop()  // Main loop
{
  BrightnessCheck(); // Check brightness
  DSTcheck(); // Check DST
  TimeAdjust(); // Check to se if time is geting modified
  TimeToArray(); // Get leds array with required configuration
  TempToArray();
  FastLED.show(); // Display leds array
  if (TempShow == true)
  delay (8000);
 
}
