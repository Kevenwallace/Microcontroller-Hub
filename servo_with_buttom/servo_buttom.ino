#include <Servo.h>

Servo meuServo;

int touch_pin = 2;
int posicao_init;
int aux;
void setup() {
  meuServo.attach(9);  // Conectado no pino digital 9
  pinMode(touch_pin, INPUT);

  // Agora gira no sentido anti-horário até a posição 0
  for (int pos = 180; pos >= 0; pos--) {
    meuServo.write(pos);
    delay(15); // Tempo para o servo se mover
    posicao_init = pos;
  }
  
}

void loop() {
  if (digitalRead(touch_pin) == HIGH) {
        for (int pos = posicao_init; pos <= posicao_init+20; pos++) {
            meuServo.write(pos);
            delay(10);
            aux = pos;
        }
        posicao_init = aux;
    }
}
