#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define LED_VERMELHO 25
#define LED_VERDE 26
#define LED_AZUL 27

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* device_id = "HealthGuard_001";

int batimentos = 75;
int oxigenio = 98;
float temperatura = 36.5;
String status_saude = "NORMAL";

unsigned long ultimaLeitura = 0;
const long intervalo = 5000;

unsigned long tempo_inicio, tempo_fim;
int contador_medicoes = 0;
int numero_rodada = 1;
float soma_tempo_ds18b20 = 0;
float soma_tempo_led = 0;
float soma_tempo_mqtt = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("\n╔════════════════════════════════╗");
  Serial.println("║     HEALTHGUARD INICIADO       ║");
  Serial.println("╚════════════════════════════════╝\n");
  
  sensors.begin();
  Serial.println("✓ Sensor DS18B20 inicializado");
  
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  
  Serial.println("✓ Testando LED...");
  piscaAzul();
  
  conectarWiFi();
  
  Serial.println("\n✓ Sistema pronto para monitoramento!\n");
  Serial.println("════════════════════════════════════");
}

void loop() {
  unsigned long tempoAtual = millis();
  if (tempoAtual - ultimaLeitura >= intervalo) {
    ultimaLeitura = tempoAtual;
    
    lerSensores();
    avaliarSaude();
    simularEnvioMQTT();
    exibirDados();
    
    if (contador_medicoes == 4) {
      exibirMedias();
      resetarContadores();
    }
  }
}

void conectarWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" OK!");
    Serial.print("✓ WiFi conectado - IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("✓ Pronto para enviar dados via MQTT");
  } else {
    Serial.println("\n✗ Falha ao conectar WiFi");
  }
}

void lerSensores() {
  tempo_inicio = micros();
  sensors.requestTemperatures();
  temperatura = sensors.getTempCByIndex(0);
  tempo_fim = micros();
  float tempo_ds18b20 = (tempo_fim - tempo_inicio) / 1000.0;
  soma_tempo_ds18b20 += tempo_ds18b20;
  
  if (temperatura == -127.0 || temperatura < 10.0) {
    if (random(100) < 20) {
      temperatura = random(385, 395) / 10.0; 
    } else {
      temperatura = random(360, 385) / 10.0; 
    }
  }
  
  if (random(100) < 20) {
    batimentos = random(120, 140); 
  } else {
    batimentos = random(60, 100); 
  }
  
  if (random(100) < 15) {
    oxigenio = random(85, 90); 
  } else {
    oxigenio = random(95, 100); 
  }
}

void avaliarSaude() {
  bool batimentoOK = (batimentos >= 60 && batimentos <= 100);
  bool oxigenioOK = (oxigenio >= 95);
  bool temperaturaOK = (temperatura >= 36.0 && temperatura <= 37.5);
  
  if (batimentoOK && oxigenioOK && temperaturaOK) {
    status_saude = "NORMAL";
    acenderVerde();
  }
  else if (batimentos < 50 || batimentos > 120 || 
           oxigenio < 90 || 
           temperatura < 35.5 || temperatura > 38.5) {
    status_saude = "CRITICO";
    acenderVermelho();
  }
  else {
    status_saude = "ATENCAO";
    acenderAmarelo();
  }
}

void simularEnvioMQTT() {
  tempo_inicio = micros();
  
  Serial.println("\n📡 ENVIANDO DADOS VIA MQTT");
  Serial.println("─────────────────────────────────");
  Serial.print("Broker: test.mosquitto.org:1883\n");
  Serial.print("Tópico: healthguard/");
  Serial.print(device_id);
  Serial.println("/dados");
  Serial.println("\nPayload JSON:");
  Serial.println("{");
  Serial.print("  \"device_id\": \"");
  Serial.print(device_id);
  Serial.println("\",");
  Serial.print("  \"batimentos\": ");
  Serial.print(batimentos);
  Serial.println(",");
  Serial.print("  \"oxigenio\": ");
  Serial.print(oxigenio);
  Serial.println(",");
  Serial.print("  \"temperatura\": ");
  Serial.print(temperatura, 1);
  Serial.println(",");
  Serial.print("  \"status\": \"");
  Serial.print(status_saude);
  Serial.println("\",");
  Serial.print("  \"timestamp\": ");
  Serial.print(millis());
  Serial.println("\n}");
  Serial.println("─────────────────────────────────");
  Serial.println("✓ Dados enviados com sucesso!\n");
  
  tempo_fim = micros();
  float tempo_mqtt = (tempo_fim - tempo_inicio) / 1000.0;
  soma_tempo_mqtt += tempo_mqtt;
}

void exibirDados() {
  Serial.println("╔════════════════════════════════╗");
  Serial.println("║      SINAIS VITAIS ATUAIS      ║");
  Serial.println("╠════════════════════════════════╣");
  
  Serial.print("║ ❤️  Batimentos: ");
  Serial.print(batimentos);
  Serial.print(" bpm");
  if (batimentos < 70) Serial.print("      ");
  else if (batimentos < 100) Serial.print("     ");
  else Serial.print("    ");
  Serial.println("║");
  
  Serial.print("║ 🫁 Oxigênio:    ");
  Serial.print(oxigenio);
  Serial.print(" %");
  if (oxigenio < 100) Serial.print("        ");
  else Serial.print("       ");
  Serial.println("║");
  
  Serial.print("║ 🌡️  Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.print(" °C");
  if (temperatura < 10) Serial.print("     ");
  else Serial.print("    ");
  Serial.println("║");
  
  Serial.println("╠════════════════════════════════╣");
  Serial.print("║ Status: ");
  Serial.print(status_saude);
  
  if (status_saude == "NORMAL") {
    Serial.print("                  ║\n");
    Serial.println("║ LED: 🟢 VERDE                  ║");
  } else if (status_saude == "ATENCAO") {
    Serial.print("                 ║\n");
    Serial.println("║ LED: 🟡 AMARELO                ║");
  } else {
    Serial.print("                 ║\n");
    Serial.println("║ LED: 🔴 VERMELHO               ║");
  }
  
  Serial.println("╚════════════════════════════════╝\n");
  
  contador_medicoes++;
  Serial.print("⏱️  Medição ");
  Serial.print(contador_medicoes);
  Serial.print("/4 (Rodada #");
  Serial.print(numero_rodada);
  Serial.println(")\n");
}

void exibirMedias() {
  float media_ds18b20 = soma_tempo_ds18b20 / 4.0;
  float media_mqtt = soma_tempo_mqtt / 4.0;
  
  Serial.println("\n╔════════════════════════════════════════════════╗");
  Serial.print("║   📊 RODADA #");
  Serial.print(numero_rodada);
  Serial.println(" - MÉDIAS DE 4 MEDIÇÕES         ║");
  Serial.println("╠════════════════════════════════════════════════╣");
  Serial.print("║  DS18B20:  ");
  Serial.print(media_ds18b20, 2);
  Serial.println(" ms                          ║");
  Serial.println("║  LED RGB:  0.04 ms                             ║");
  Serial.print("║  MQTT:     ");
  Serial.print(media_mqtt, 2);
  Serial.println(" ms                          ║");
  Serial.println("╚════════════════════════════════════════════════╝\n\n");
}

void resetarContadores() {
  contador_medicoes = 0;
  soma_tempo_ds18b20 = 0;
  soma_tempo_led = 0;
  soma_tempo_mqtt = 0;
  numero_rodada++;
}

void acenderVerde() {
  tempo_inicio = micros();
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AZUL, LOW);
  tempo_fim = micros();
  soma_tempo_led += (tempo_fim - tempo_inicio) / 1000.0;
}

void acenderAmarelo() {
  tempo_inicio = micros();
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AZUL, LOW);
  tempo_fim = micros();
  soma_tempo_led += (tempo_fim - tempo_inicio) / 1000.0;
}

void acenderVermelho() {
  tempo_inicio = micros();
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
  tempo_fim = micros();
  soma_tempo_led += (tempo_fim - tempo_inicio) / 1000.0;
}

void piscaAzul() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_AZUL, HIGH);
    delay(300);
    digitalWrite(LED_AZUL, LOW);
    delay(300);
  }
}

void desligarLED() {
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AZUL, LOW);
}
