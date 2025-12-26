#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- TELA ----------------
#define SCREEN_W 128
#define SCREEN_H 64

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

// ---------------- BONECO ----------------
struct Boneco {
  int x;
  int y;
  int passo;
};

Boneco boneco;

// ---------------- DESENHO ----------------
void desenharBoneco(Boneco b) {
  int fase = b.passo % 20;

  // Movimento alternado
  int swing = (fase < 10) ? fase : 20 - fase; // 0 → 10 → 0
  int direcao = (b.passo % 40 < 20) ? 1 : -1;

  int pernaFrente = swing * direcao;
  int pernaTras   = -pernaFrente;

  int bracoFrente = -pernaFrente;
  int bracoTras   = -bracoFrente;

  // --- Cabeça ---
  display.drawCircle(b.x, b.y - 16, 4, WHITE);

  // --- Corpo ---
  display.drawLine(b.x, b.y - 12, b.x, b.y + 6, WHITE);

  // --- Braços (2 segmentos) ---
  display.drawLine(b.x, b.y - 8, b.x + bracoFrente, b.y + 2, WHITE);
  display.drawLine(b.x, b.y - 8, b.x + bracoTras,   b.y + 2, WHITE);

  // --- Pernas (2 segmentos) ---
  // Coxas
  int joelhoA_x = b.x + pernaFrente / 2;
  int joelhoB_x = b.x + pernaTras / 2;

  int joelho_y = b.y + 12;

  display.drawLine(b.x, b.y + 6, joelhoA_x, joelho_y, WHITE);
  display.drawLine(b.x, b.y + 6, joelhoB_x, joelho_y, WHITE);

  // Canela
  display.drawLine(joelhoA_x, joelho_y, b.x + pernaFrente, b.y + 20, WHITE);
  display.drawLine(joelhoB_x, joelho_y, b.x + pernaTras,   b.y + 20, WHITE);
}

// ---------------- SETUP ----------------
void setup() {
  Wire.begin(8, 9); // Wokwi / ESP32-C3
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  boneco.x = -10;
  boneco.y = 40;
  boneco.passo = 0;
}

// ---------------- LOOP ----------------
void loop() {
  display.clearDisplay();

  desenharBoneco(boneco);
  display.display();

  boneco.x += 1;        // deslocamento horizontal
  boneco.passo += 1;   // animação

  if (boneco.x > SCREEN_W + 10) {
    boneco.x = -10;
  }

  delay(50);
}
