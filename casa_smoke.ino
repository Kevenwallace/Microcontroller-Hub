#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- CASA (bitmap 32x32) ----------
#define CASA_W 32
#define CASA_H 32

const unsigned char casaBitmap[] PROGMEM = {
  0x00,0x00,0x00,0x00,
  0x03,0xC0,0x00,0x00,
  0x0F,0xF0,0x00,0x00,
  0x3C,0x3C,0x00,0x00,
  0x30,0x0C,0x00,0x00,
  0x3F,0xFC,0x00,0x00,
  0x30,0x0C,0x00,0x00,
  0x30,0x0C,0x00,0x00,
  0x3F,0xFC,0x00,0x00,
  0x30,0x0C,0x00,0x00,
  0x30,0x0C,0x00,0x00,
  0x3F,0xFC,0x00,0x00,
};

// ---------- FUMAÇA (procedural) ----------
struct Fumaca {
  int x;
  int y;
  int raio;
};

Fumaca fumacas[3];

// Inicializa fumaça
void initFumaca() {
  for (int i = 0; i < 3; i++) {
    fumacas[i].x = 70;
    fumacas[i].y = 30 + i * 6;
    fumacas[i].raio = 2;
  }
}

// Atualiza fumaça
void updateFumaca() {
  for (int i = 0; i < 3; i++) {
    fumacas[i].y -= 1;
    fumacas[i].x += random(-1, 2);

    if (fumacas[i].y < 20) {
      fumacas[i].y = 30;
    }
  }
}

// Desenha fumaça
void drawFumaca() {
  for (int i = 0; i < 3; i++) {
    display.drawCircle(
      fumacas[i].x,
      fumacas[i].y,
      fumacas[i].raio,
      WHITE
    );
  }
}

// ---------- SETUP ----------
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  initFumaca();
}

// ---------- LOOP ----------
void loop() {
  display.clearDisplay();

  // Desenha casa
  display.drawBitmap(40, 32, casaBitmap, CASA_W, CASA_H, WHITE);

  // Desenha fumaça
  updateFumaca();
  drawFumaca();

  display.display();
  delay(80);
}
