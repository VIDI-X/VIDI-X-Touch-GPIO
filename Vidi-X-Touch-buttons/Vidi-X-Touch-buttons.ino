#define OFF "\033[0m"
#define BOLD "\033[1m"
#define BOLDGREEN "\033[1;32m"

const int LED_PIN_NUMBER = 3;

// Definiraj niz Array za Touch pinove
const int TOUCH_PINS[] = { T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 };

// Definiraj dozvoljeno odstupanje u očitanjima za Touch pinove
const int MAX_DEVIATION = 20;  // dozvoljena devijacija između očitanja

// Varijabla za brojanje koliko puta su gumbi pritisnuti
int buttonPressCount[10] = { 0 };

// Definiraj niz pragova osjetljivosti za svaki gumb zasebno
int VALUE_THRESHOLDS[] = { 35, 25, 20, 30, 40, 25, 30, 25, 20, 30 };
int deviation[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 };

const int READ_COUNT = 10;  // broj očitanja po touch pinu

void deviation_function() {
  for (int i = 0; i < 10; i++) {
    int readings[READ_COUNT];
    int sum = 0;
    int minReading = 1024;
    int maxReading = 0;
    int validCount = 0;

    // pročitaj 10 puta vrijednost s trenutnog touch pina
    for (int j = 0; j < READ_COUNT; j++) {
      readings[j] = touchRead(TOUCH_PINS[i]);
      sum += readings[j];
      delay(10);
    }

    // Pronađi indeks člana niza koji najviše odstupa od ostalih očitanja
    int maxDiffIndex = 0;  // Indeks člana niza s najvećom razlikom
    int maxDiff = 0;       // Najveća razlika između člana niza i ostalih očitanja
    for (int ii = 0; ii < READ_COUNT; ii++) {
      int diff = 0;
      for (int j = 0; j < READ_COUNT; j++) {
        diff += abs(readings[ii] - readings[j]);  // Izračunaj razliku između dva člana niza
      }
      if (diff > maxDiff) {
        maxDiffIndex = ii;
        maxDiff = diff;
      }
    }

    // Eliminiraj član niza koji najviše odstupa od ostalih očitanja
    for (int j = 0; j < READ_COUNT; j++) {
      if (j == maxDiffIndex) {
        readings[j] = 0;  // Ako je indeks jednak indeksu člana s najvećom razlikom, postavi vrijednost na nulu
      }
    }

    // izračunaj minimalnu i maksimalnu vrijednost te devijaciju između očitanja
    for (int j = 0; j < READ_COUNT; j++) {
      if (readings[j] != 0) {
        if (readings[j] < minReading) {
          minReading = readings[j];
        }
        if (readings[j] > maxReading) {
          maxReading = readings[j];
        }
      }
    }

    // izračunaj odstupanja u očitanju
    deviation[i] = maxReading - minReading;
    
    // za poptpuno stabilne pinove postavit vrijednosti odstipanja na MAX_DEVIATION vrijednsot
    // ostale pinove ostavi na izračunatim vrijednostima
    if (deviation[i] < 20) {
      deviation[i] = MAX_DEVIATION;
    }

    // provjeri koja očitanja su u granicama maksimalne dozvoljene devijacije i od tih uzmi najniže očitanje
    for (int j = 0; j < READ_COUNT; j++) {
      if (abs(readings[j] - sum / READ_COUNT) <= MAX_DEVIATION) {
        if (readings[j] < minReading) {
          minReading = readings[j];
        }
        validCount++;
      }
    }

    // spremanje pragova osjetljivosti za svaki touch pin
    if (validCount > 0) {
      VALUE_THRESHOLDS[i] = minReading;
      Serial.print("Touch pin ");
      Serial.print(TOUCH_PINS[i]);
      Serial.print(": THRESHOLD = ");
      Serial.print(VALUE_THRESHOLDS[i]);
      Serial.print(": deviation = ");
      Serial.println(deviation[i]);
    }
  }
  // pauza prije ponovnog očitavanja
  delay(200);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Touch sensor tutorial ESP32");
  delay(1000);
  pinMode(LED_PIN_NUMBER, OUTPUT);

  Serial.println("Na mikroprekidačima oznake S3 pomakni gumb VOLUME u USE EXP. položaj");
  // petlja koja čeka promjenu vrijednosti T0
  while (touchRead(0) == 1) {
    delay(100);  // kratki delay da se smanji opterećenje procesora
  }
  Serial.println("Čekam 10 sekundi da pustite VIDI X kako nebiste dotaknuli neki touch GPIO!");
  delay(10000);

  deviation_function();

  Serial.println("Čekam 10 sekundi...");
  delay(10000);
}

void loop() {
  // Prolazak kroz sve Touch pinove u nizu
  for (int i = 0; i < 10; i++) {
    int TOUCH_SENSOR_VALUE = touchRead(TOUCH_PINS[i]);

    // Koristi odgovarajuću vrijednost praga za svaki gumb
    if (TOUCH_SENSOR_VALUE < ( VALUE_THRESHOLDS[i] - deviation[i] ) ) {
      digitalWrite(LED_PIN_NUMBER, HIGH);
      buttonPressCount[i]++;  // Povećaj brojač ako je gumb pritisnut
    } else {
      digitalWrite(LED_PIN_NUMBER, LOW);
    }

    Serial.print("Touch pin ");
    Serial.print(TOUCH_PINS[i]);
    Serial.print(": THRESHOLD = ");
    if (TOUCH_SENSOR_VALUE < ( VALUE_THRESHOLDS[i] - deviation[i] )) {
      Serial.print(BOLDGREEN);
    }
    Serial.print(( VALUE_THRESHOLDS[i] - deviation[i] ));
    Serial.print(" : ");
    Serial.print(TOUCH_SENSOR_VALUE);
    if (TOUCH_SENSOR_VALUE < ( VALUE_THRESHOLDS[i] - deviation[i] )) {
      Serial.print(OFF);
    }
    Serial.print(", Button press count: ");
    Serial.print(BOLD);
    Serial.println(buttonPressCount[i]);
    Serial.print(OFF);
  }
  //delay(1000);
}
