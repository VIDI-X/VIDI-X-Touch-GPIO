#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// ILI9341 TFT LCD deklaracija spajanja zaslona
#define TFT_CS 5
#define TFT_DC 21

// varijable za dimenzije zaslona
int myWidth;
int myHeight;
int j = 0;

// Stvori objekt zaslona koji zovemo TFT
Adafruit_ILI9341 TFT = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define TS_CS 4  // deklaracija pina za senzor dodira ekrana

// stvaranje objekta za senzor dodira koji će se zvati ts
XPT2046_Touchscreen ts(TS_CS);

#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000
#define Z_THRESHOLD 400

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// definicija slidera
#define SLIDER_WIDTH 280
#define SLIDER_HEIGHT 50
#define SLIDER_X 20
#define SLIDER_Y1 60
#define SLIDER_Y2 120
#define SLIDER_Y3 180
#define SLIDER_COLOR 0xE79C

// varijable za vrijednosti slidera
int sliderValue1 = 125; // trajanje tona
int sliderValue2 = 100; // glasnoća tona (za mali zvučnik oko 200, za slušalice postaviti na 100 ili manje)
int sliderValue3 = 0; // za pomak frekvencije tona

int volume = sliderValue2 * 3; //varijabla glasnoće ovisna o slideru 2

const int speakerPin = 25;  // pin 25 za zvučnik
int duration = 125;

int threshold = 20;  //povećavanjem threshold varijable možete postići slučajno generiranje zvukova
bool touch0detected = false;
bool touch1detected = false;
bool touch2detected = false;
bool touch3detected = false;
bool touch4detected = false;
bool touch5detected = false;
bool touch6detected = false;
bool touch7detected = false;
bool touch8detected = false;
bool touch9detected = false;

void setup() {
  Serial.begin(115200);

  // inicijalizacija zaslona
  TFT.begin();
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1)
      ;
  }
  TFT.setRotation(3);  // okreni zaslon za 90 stupnjeva
  ts.setRotation(1);   // postavi orijentaciju
  myWidth = TFT.width();
  myHeight = TFT.height();
  TFT.fillScreen(BLACK);
  TFT.setTextColor(ILI9341_RED); // postavljamo boju teksta u zelenu
  TFT.setTextSize(2);
  TFT.setCursor(0, 0);
  TFT.println("           VIDI X");
  TFT.println("         SYNTHESIZER");
  TFT.setTextSize(1);
  TFT.setTextColor(ILI9341_BLUE);
  TFT.print("==##====#=#====##==##====#=#====##==##====#=#====##==");
  TFT.setCursor(20, 52);
  TFT.print("tone duration");
  TFT.setCursor(20, 112);
  TFT.print("volume");
  TFT.setCursor(20, 172);
  TFT.print("tone frequency");

  // nacrtaj slider 1
  TFT.fillRect(SLIDER_X, SLIDER_Y1, SLIDER_WIDTH, SLIDER_HEIGHT, SLIDER_COLOR);
  TFT.drawFastHLine(SLIDER_X, SLIDER_Y1 + SLIDER_HEIGHT / 2, SLIDER_WIDTH, BLUE);
  updateSliderValue(1);

  // nacrtaj slider 2
  TFT.fillRect(SLIDER_X, SLIDER_Y2, SLIDER_WIDTH, SLIDER_HEIGHT, SLIDER_COLOR);
  TFT.drawFastHLine(SLIDER_X, SLIDER_Y2 + SLIDER_HEIGHT / 2, SLIDER_WIDTH, BLUE);
  updateSliderValue(2);

  // nacrtaj slider 3
  TFT.fillRect(SLIDER_X, SLIDER_Y3, SLIDER_WIDTH, SLIDER_HEIGHT, SLIDER_COLOR);
  TFT.drawFastHLine(SLIDER_X, SLIDER_Y3 + SLIDER_HEIGHT / 2, SLIDER_WIDTH, BLUE);
  updateSliderValue(3);

  delay(1000);  // give me time to bring up serial monitor
  Serial.println("VIDI X Touch Interrupt Test");

  ledcSetup(0, 1000, 8);
  ledcAttachPin(speakerPin, 0); //
  ledcWrite(0, sliderValue2); // postavljamo glasnoću tona

  //touchAttachInterrupt(T0, gotTouch0, threshold); //GPIO 4
  //touchAttachInterrupt(T1, gotTouch1, threshold); //GPIO 0
  touchAttachInterrupt(T2, gotTouch2, threshold);  //GPIO 2
  touchAttachInterrupt(T3, gotTouch3, threshold);  //GPIO 15
  touchAttachInterrupt(T4, gotTouch4, threshold);  //GPIO 13
  touchAttachInterrupt(T5, gotTouch5, threshold);  //GPIO 12
  touchAttachInterrupt(T6, gotTouch6, threshold);  //GPIO 14
  touchAttachInterrupt(T7, gotTouch7, threshold);  //GPIO 27
  touchAttachInterrupt(T8, gotTouch8, threshold);  //GPIO 33
  touchAttachInterrupt(T9, gotTouch9, threshold);  //GPIO 32
}

//void gotTouch0() {
//  touch0detected = true;
//}
//void gotTouch1() {
//  touch1detected = true;
//}
void gotTouch2() {
  touch2detected = true;
}
void gotTouch3() {
  touch3detected = true;
}
void gotTouch4() {
  touch4detected = true;
}
void gotTouch5() {
  touch5detected = true;
}
void gotTouch6() {
  touch6detected = true;
}
void gotTouch7() {
  touch7detected = true;
}
void gotTouch8() {
  touch8detected = true;
}
void gotTouch9() {
  touch9detected = true;
}

void loop() {

  duration = sliderValue1;
  
  if (  //(touch0detected == false) &&
    //(touch1detected == false) &&
    (touch2detected == false) && (touch3detected == false) && (touch4detected == false) && (touch5detected == false) && (touch6detected == false) && (touch7detected == false) && (touch8detected == false) && (touch9detected == false)) {
    ledcWriteTone(0, 0);
  }

  /*
  if (touch0detected) {
    touch0detected = false;
    ledcWriteTone(0, 260 +(sliderValue3 * 5) );  // svira se nota C4
    ledcWrite(0, volume);
    //Serial.println("C4");
    delay(duration);
  }
  
  if (touch1detected) {
    touch1detected = false;
    ledcWriteTone(0, 290 +(sliderValue3 * 5) );  // svira se nota D4
    ledcWrite(0, volume);
    //Serial.println("D4");
    delay(duration);
  }
  */

  if (touch2detected) {
    touch2detected = false;
    ledcWriteTone(0, 330 + (sliderValue3 * 5));  // svira se nota E4
    ledcWrite(0, volume);
    //Serial.println("E4");
    delay(duration);
  }

  if (touch3detected) {
    touch3detected = false;
    ledcWriteTone(0, 350 + (sliderValue3 * 5));  // svira se nota F4
    ledcWrite(0, volume);
    //Serial.println("F4");
    delay(duration);
  }

  if (touch4detected) {
    touch4detected = false;
    ledcWriteTone(0, 390 + (sliderValue3 * 5));  // svira se nota G4
    ledcWrite(0, volume);
    //Serial.println("G4");
    delay(duration);
  }

  if (touch5detected) {
    touch5detected = false;
    ledcWriteTone(0, 440 + (sliderValue3 * 5));  // svira se nota A4
    ledcWrite(0, volume);
    //Serial.println("A4");
    delay(duration);
  }

  if (touch6detected) {
    touch6detected = false;
    ledcWriteTone(0, 490 + (sliderValue3 * 5));  // svira se nota B4
    ledcWrite(0, volume);
    //Serial.println("B4");
    delay(duration);
  }

  if (touch7detected) {
    touch7detected = false;
    ledcWriteTone(0, 550 + (sliderValue3 * 5));  // svira se nota C5
    ledcWrite(0, volume);
    //Serial.println("C5");
    delay(duration);
  }

  if (touch8detected) {
    touch8detected = false;
    ledcWriteTone(0, 600 + (sliderValue3 * 5));  // svira se nota D5
    ledcWrite(0, volume);
    //Serial.println("D5");
    delay(duration);
  }

  if (touch9detected) {
    touch9detected = false;
    ledcWriteTone(0, 660 + (sliderValue3 * 5));  // svira se nota E5
    ledcWrite(0, volume);
    //Serial.println("E5");
    delay(duration);
  }
  // dohvati točku dodira
  TS_Point point = ts.getPoint();
  // ukoliko ima dodira
  if (point.z > 0) {

    // pretvori koordinate
    int x = map(point.x, TS_MINX, TS_MAXX, 0, myWidth);
    int y = map(point.y, TS_MINY, TS_MAXY, 0, myHeight);
    //int z = point.z;
    /*
    Serial.print("sliderValue1, 2, 3 = ");
    Serial.print(sliderValue1);
    Serial.print(", ");
    Serial.print(sliderValue2);
    Serial.print(", ");
    Serial.print(sliderValue3);
    Serial.print(" : x = ");
    Serial.print(x);
    Serial.print(" : y = ");
    Serial.println(y);

    Serial.print(" : z = ");
    Serial.println(z);
    */

    // provjeri dodir na slideru 1
    if (x > SLIDER_X && x < SLIDER_X + SLIDER_WIDTH && y > SLIDER_Y1 && y < SLIDER_Y1 + SLIDER_HEIGHT) {
      sliderValue1 = map(x, SLIDER_X, SLIDER_X + SLIDER_WIDTH, 0, 255);
      updateSliderValue(1);
    }

    // provjeri dodir na slideru 2
    if (x > SLIDER_X && x < SLIDER_X + SLIDER_WIDTH && y > SLIDER_Y2 && y < SLIDER_Y2 + SLIDER_HEIGHT) {
      sliderValue2 = map(x, SLIDER_X, SLIDER_X + SLIDER_WIDTH, 0, 255);
      updateSliderValue(2);
      volume = sliderValue2 * 3;
    }

    // provjeri dodir na slideru 3
    if (x > SLIDER_X && x < SLIDER_X + SLIDER_WIDTH && y > SLIDER_Y3 && y < SLIDER_Y3 + SLIDER_HEIGHT) {
      sliderValue3 = map(x, SLIDER_X, SLIDER_X + SLIDER_WIDTH, 0, 255);
      updateSliderValue(3);
    }
  }
}

// funkcija za ažuriranje vrijednosti slidera
void updateSliderValue(int sliderNum) {
  int sliderY;
  int sliderValue;
  switch (sliderNum) {
    case 1:
      sliderY = SLIDER_Y1;
      sliderValue = sliderValue1;
      break;
    case 2:
      sliderY = SLIDER_Y2;
      sliderValue = sliderValue2;
      break;
    case 3:
      sliderY = SLIDER_Y3;
      sliderValue = sliderValue3;
      break;
    default:
      return;
  }
  TFT.fillRect(SLIDER_X + 1, sliderY + 1, SLIDER_WIDTH - 2, SLIDER_HEIGHT - 2, BLACK);
  TFT.fillRect(SLIDER_X + 1, sliderY + 1, map(sliderValue, 0, 255, 0, SLIDER_WIDTH - 2), SLIDER_HEIGHT - 2, RED);
}
