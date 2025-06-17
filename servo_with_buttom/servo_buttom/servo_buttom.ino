#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Variáveis globais
int16_t accelX = 0, accelY = 0, accelZ = 0;
const float ACCEL_RANGE = 16.0; // ±2g
const uint8_t MAX_RETRIES = 3;
uint8_t retryCount = 0;

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Aguarda conexão serial

  // Inicializa I2C como Mestre
  Wire.begin();
  Wire.setClock(100000); // 100 kHz

  // Inicializa MPU-6050
  if (!mpu.begin()) {
    Serial.println("Erro: MPU-6050 não encontrado. Verifique conexões.");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // ±2g
  Serial.println("MPU-6050 inicializado.");

  Serial.println("Arduino Mestre iniciado.");
  scanI2CDevices();
}

void loop() {
  // Lê aceleração
  delay(1000);
  sensors_event_t a, g, temp;
  if (mpu.getEvent(&a, &g, &temp)) {
    // Converte para valores brutos (mapeia ±2g para ±32767)
    accelX = (int16_t)(a.acceleration.x / ACCEL_RANGE * 32767);
    accelY = (int16_t)(a.acceleration.y / ACCEL_RANGE * 32767);
    accelZ = (int16_t)(a.acceleration.z / ACCEL_RANGE * 32767);

    // Valida dados
    if (abs(accelX) > 32767 || abs(accelY) > 32767 || abs(accelZ) > 32767) {
      Serial.println("Erro: Aceleração fora da faixa.");
      accelX = accelY = accelZ = 0;
    }
  } else {
    Serial.println("Erro ao ler MPU-6050.");
    accelX = accelY = accelZ = 0;
    if (++retryCount >= MAX_RETRIES) {
      Serial.println("Falha persistente no MPU-6050. Verifique o sensor.");
      retryCount = 0;
    }
  }

  // Exibe no Serial
  Serial.print("X=");
  Serial.print(accelX);
  Serial.print(" | Y=");
  Serial.print(accelY);
  Serial.print(" | Z=");
  Serial.println(accelZ);
  if (accelX < 0) { // Corrigido de accelx para accelX
    Serial.println("Aciona o barulho!"); // Adicionado ponto e vírgula e alterado para println
  }

  // Envia para o Escravo (endereço 8)
  Wire.beginTransmission(8);
  Wire.write(0xAA); // Byte de sincronização
  Wire.write((byte*)&accelX, sizeof(int16_t)); // 2 bytes
  Wire.write((byte*)&accelY, sizeof(int16_t)); // 2 bytes
  Wire.write((byte*)&accelZ, sizeof(int16_t)); // 2 bytes
  uint8_t status = Wire.endTransmission();

  if (status == 0) {
    Serial.println("Dados enviados com sucesso.");
  } else {
    Serial.print("Erro I2C: Status=");
    Serial.println(status);
  }

  delay(100); // Intervalo de leitura
}

void scanI2CDevices() {
  uint8_t error, address;
  uint8_t nDevices = 0;

  Serial.println("Procurando dispositivos I2C...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Dispositivo encontrado: 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Erro desconhecido: 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("Nenhum dispositivo I2C encontrado.");
  } else {
    Serial.println("Busca concluída.");
  }
}
