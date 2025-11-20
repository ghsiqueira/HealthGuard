# 🏥 HealthGuard - Sistema de Monitoramento de Sinais Vitais

![Status](https://img.shields.io/badge/status-active-success.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

## 📋 Sobre o Projeto

O **HealthGuard** é um sistema para monitoramento contínuo de sinais vitais, desenvolvido como parte das atividades de Objetos Inteligentes Conectados. O projeto utiliza ESP32, sensores de temperatura, frequência cardíaca e oxigenação, além de comunicação MQTT para envio de dados em tempo real.

### 🎯 Objetivos

- Monitorar frequência cardíaca, temperatura corporal e níveis de oxigênio
- Enviar dados em tempo real via protocolo MQTT
- Indicação visual de status através de LED RGB
- Sistema de alertas para situações críticas

## 🚀 Tecnologias Utilizadas

- **Microcontrolador:** ESP32 DevKit V1
- **Sensores:**
  - DS18B20 (Temperatura)
  - MAX30102 (Frequência cardíaca e SpO₂ - simulado)
- **Comunicação:** WiFi + MQTT
- **Plataforma:** Wokwi (Simulador)
- **Linguagem:** C++ (Arduino)

## 🔧 Componentes do Hardware

| Componente | Modelo | Função |
|------------|--------|--------|
| Microcontrolador | ESP32 DevKit V1 | Processamento e WiFi |
| Sensor de Temperatura | DS18B20 | Medir temperatura corporal |
| LED RGB | Comum | Indicação visual de status |
| Sensor de FC/SpO₂ | MAX30102 (simulado) | Batimentos e oxigenação |

## 📊 Arquitetura do Sistema

```
┌─────────────────┐
│   Sensores      │
│  - DS18B20      │
│  - MAX30102     │
└────────┬────────┘
         │
         ↓
┌─────────────────┐
│     ESP32       │
│  (Processamento)│
└────────┬────────┘
         │
         ↓
┌─────────────────┐
│   WiFi + MQTT   │
│  (Comunicação)  │
└────────┬────────┘
         │
         ↓
┌─────────────────┐
│      Nuvem      │
│   (Servidor)    │
└─────────────────┘
```

## 📈 Resultados

### Tempos de Resposta Medidos

| Componente | Tempo Médio (ms) |
|------------|------------------|
| DS18B20    | 33.02           |
| LED RGB    | 0.04            |
| Prep. MQTT | 33.67           |

### Estados de Saúde

| Status | Condições | LED |
|--------|-----------|-----|
| **NORMAL** | Todos os parâmetros OK | 🟢 Verde |
| **ATENÇÃO** | Algum parâmetro alterado | 🟡 Amarelo |
| **CRÍTICO** | Situação grave | 🔴 Vermelho |

#### Parâmetros Normais:
- **Frequência Cardíaca:** 60-100 bpm
- **Oxigênio (SpO₂):** ≥ 95%
- **Temperatura:** 36.0-37.5°C

## 🎬 Demonstração

🔗 **Simulação no Wokwi:** [https://wokwi.com/projects/443812439152820225](https://wokwi.com/projects/443812439152820225)

🎥 **Vídeo no YouTube:** [INSERIR LINK DO VÍDEO AQUI]

## 💻 Como Usar

### 1. Executar no Wokwi (Recomendado)

1. Acesse: [https://wokwi.com/projects/443812439152820225](https://wokwi.com/projects/443812439152820225)
2. Clique em "Start Simulation"
3. Observe os dados no Serial Monitor

### 2. Implementação Física

1. Monte o circuito conforme o diagrama
2. Instale as bibliotecas necessárias:
   ```
   - WiFi.h
   - OneWire.h
   - DallasTemperature.h
   ```
3. Configure seu WiFi no código:
   ```cpp
   const char* ssid = "SEU_WIFI";
   const char* password = "SUA_SENHA";
   ```
4. Compile e faça upload para o ESP32

## 📁 Estrutura do Repositório

```
HealthGuard/
│
├── HealthGuard.ino          # Código principal do Arduino
├── README.md                # Este arquivo
├── DOCUMENTACAO.md          # Documentação técnica completa
├── circuit-diagram.png      # Diagrama do circuito
└── docs/                    # Documentação adicional
```

## 🔌 Diagrama de Conexões

### Pinout ESP32

| Pino ESP32 | Componente | Função |
|------------|------------|--------|
| GPIO 15 | DS18B20 | One-Wire Data |
| GPIO 25 | LED Vermelho | Saída Digital |
| GPIO 26 | LED Verde | Saída Digital |
| GPIO 27 | LED Azul | Saída Digital |
| 3.3V | DS18B20 VDD | Alimentação |
| GND | Comum | Terra |

## 📡 Protocolo MQTT

### Configuração

- **Broker:** test.mosquitto.org
- **Porta:** 1883
- **Tópico:** `healthguard/[device_id]/dados`

### Payload JSON

```json
{
  "device_id": "HealthGuard_001",
  "batimentos": 75,
  "oxigenio": 98,
  "temperatura": 36.5,
  "status": "NORMAL",
  "timestamp": 12345
}
```

## 👥 Equipe

- Camily Sousa Lucio - camily.slucio@gmail.com
- Gabriel Henrique Siqueira de Oliveira - gabrielh2805@gmail.com
- Guilherme da Silva Souza - guilherme_souza17@hotmail.com

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.

## 🎓 Contexto Acadêmico

**Instituição:** Universidade Presbiteriana Mackenzie | 
**Curso:** Análise e Desenvolvimento de Sistemas | 
**Disciplina:** Objetos Inteligentes Conectados | 
**Atividade:** A4 - Aplicando Conhecimento | 
**Semestre:** 5º

---

⭐ Se este projeto foi útil para você, considere dar uma estrela!

📧 Para dúvidas ou sugestões, entre em contato através das issues do GitHub.
