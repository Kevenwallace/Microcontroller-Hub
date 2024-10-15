#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializando o LCD (endereço, colunas, linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);


int hora = 12;  
int minuto = 22;
int segundo = 0;

unsigned long tempoAnterior = 0;

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  int dia = 15;
  int mes = 10;
  int ano = 2024;
  unsigned long tempoAtual = millis();
  Serial.print(tempoAtual);

  if (tempoAtual - tempoAnterior >= 1000) {
    tempoAnterior = tempoAtual;
    segundo++;

    if (segundo >= 60) {
      segundo = 0;
      minuto++;
    }
    if (minuto >= 60) {
      minuto = 0;
      hora++;
    }
    if (hora >= 24) {
      hora = 0;
    }

    // Exibe a hora no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hora: "); 
    lcd.print(hora);
    lcd.print(":");
    if (minuto < 10) lcd.print("0");
    lcd.print(minuto);
    lcd.print(":"); 
    if (segundo < 10) lcd.print("0");
    lcd.print(segundo);
    lcd.setCursor(0,1);
    lcd.print("DATA");
    lcd.print(":");
    lcd.print(dia);
    lcd.print("/");
    lcd.print(mes);
    lcd.print("/");
    lcd.print(ano);
    
  }
}

