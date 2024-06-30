#include <IRremote.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int irRecvPin = 5;
const int buttonPlayPin = 3;

IRrecv irrecv(irRecvPin);
IRsend irsend;

decode_results results;

void setup() {
  pinMode(buttonPlayPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 init hiba"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  
  irrecv.enableIRIn(); // IR vevő inicializálása
}

void loop() {
  if (irrecv.decode(&results)) {
    displayIR(results.value);
    irrecv.resume(); // Következő IR kód fogadása
  }

  if (digitalRead(buttonPlayPin) == LOW) {
    displayPlaying();
    irsend.sendNEC(results.value, 32); // Elmentett IR kód lejátszása

    // Várunk, amíg le nem nyomjuk újra a gombot
    while (digitalRead(buttonPlayPin) == LOW) {
      delay(100);
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Lejatszas vege");
    display.display();
    delay(2000);
    display.clearDisplay();
  }
}

void displayIR(unsigned long irCode) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("IR kod fogadva:");
  display.setCursor(0, 10);
  display.print(irCode, HEX);
  display.display();

  delay(2000);
  display.clearDisplay();
}

void displayPlaying() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("IR kod lejatszas...");
  display.display();
}
