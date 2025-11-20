# 📖 Documentação Técnica - HealthGuard (Wokwi)

## Índice

1. [Visão Geral](#visão-geral)
2. [Como Usar no Wokwi](#como-usar-no-wokwi)
3. [Funcionamento Detalhado](#funcionamento-detalhado)
4. [Configuração](#configuração)

---

## 🎯 Visão Geral

O HealthGuard é um sistema embarcado para monitoramento contínuo de sinais vitais, desenvolvido e testado no **simulador Wokwi**.

### Arquitetura do Sistema

```
┌─────────────────┐
│   Sensores      │
│  - DS18B20      │
│  - Simulados    │
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
│   LED RGB       │
│  (Indicação)    │
└────────┬────────┘
         │
         ↓
┌─────────────────┐
│  Serial Monitor │
│   (Saída MQTT)  │
└─────────────────┘
```

### Componentes Utilizados

- **ESP32 DevKit V1:** Microcontrolador principal
- **DS18B20:** Sensor de temperatura digital
- **LED RGB:** Indicação visual de status
- **Sensores simulados:** Frequência cardíaca e SpO₂ (MAX30102)

---

## 🚀 Como Usar no Wokwi

### Acesso Rápido

🔗 **Link do projeto:** [https://wokwi.com/projects/443812439152820225](https://wokwi.com/projects/443812439152820225)

### Passo a Passo

#### 1. Acessar o Projeto
```
1. Clique no link acima
2. Aguarde carregar a página do Wokwi
3. Você verá o circuito montado
```

#### 2. Iniciar a Simulação
```
1. Clique no botão verde "Start Simulation" (▶️)
2. Aguarde a inicialização (~3 segundos)
3. Observe os dados no Serial Monitor (parte inferior)
```

#### 3. Interagir com o Projeto
```
- Os dados são atualizados a cada 5 segundos
- O LED RGB muda de cor conforme o status de saúde
- O Serial Monitor mostra:
  • Sinais vitais coletados
  • Status de saúde (NORMAL/ATENÇÃO/CRÍTICO)
  • Payload JSON simulado para MQTT
```

#### 4. Parar a Simulação
```
1. Clique no botão vermelho "Stop Simulation" (⏹️)
2. Para reiniciar, clique em "Start" novamente
```

### Visualizando os Dados

No **Serial Monitor**, você verá:

```
╔════════════════════════════════╗
║      SINAIS VITAIS ATUAIS      ║
╠════════════════════════════════╣
║ ❤️  Batimentos: 75 bpm         ║
║ 🫁 Oxigênio:    98 %           ║
║ 🌡️  Temperatura: 36.5 °C       ║
╠════════════════════════════════╣
║ Status: NORMAL                  ║
║ LED: 🟢 VERDE                  ║
╚════════════════════════════════╝
```

---

## 🔄 Funcionamento Detalhado

### Fluxo de Execução

```
1. Inicialização (setup)
   ├── Configurar Serial (115200 baud)
   ├── Inicializar sensor DS18B20
   ├── Configurar pinos do LED RGB
   ├── Conectar WiFi (Wokwi-GUEST)
   └── Teste inicial LED (piscar azul 3x)

2. Loop Principal (a cada 5 segundos)
   ├── lerSensores()
   │   ├── Ler temperatura real do DS18B20
   │   ├── Simular batimentos cardíacos (60-140 bpm)
   │   └── Simular oxigênio (85-100%)
   ├── avaliarSaude()
   │   ├── Comparar valores com thresholds
   │   ├── Determinar status (NORMAL/ATENÇÃO/CRÍTICO)
   │   └── Acender LED correspondente
   ├── simularEnvioMQTT()
   │   └── Exibir payload JSON no Serial Monitor
   └── exibirDados()
       └── Mostrar tabela formatada com dados
```

### Estados de Saúde

#### 🟢 NORMAL
```cpp
Condições:
✓ 60 ≤ batimentos ≤ 100 bpm
✓ oxigênio ≥ 95%
✓ 36.0°C ≤ temperatura ≤ 37.5°C

LED: Verde
Ação: Monitoramento contínuo
```

#### 🟡 ATENÇÃO
```cpp
Condições:
⚠️ 50 < batimentos < 60 ou 100 < batimentos ≤ 120
⚠️ 90% ≤ oxigênio < 95%
⚠️ 35.5°C < temperatura < 36.0°C ou 37.5°C < temperatura ≤ 38.5°C

LED: Amarelo (vermelho + verde)
Ação: Observar evolução
```

#### 🔴 CRÍTICO
```cpp
Condições:
❌ batimentos < 50 ou > 120 bpm
❌ oxigênio < 90%
❌ temperatura < 35.5°C ou > 38.5°C

LED: Vermelho
Ação: Intervenção necessária
```

### Simulação de Dados

#### Temperatura (DS18B20)
```cpp
// Sensor real do Wokwi
// Lê temperatura ambiente virtual (~36-38°C)
sensors.requestTemperatures();
temperatura = sensors.getTempCByIndex(0);
```

#### Batimentos Cardíacos (Simulado)
```cpp
// 80% das vezes: valores normais (60-100 bpm)
// 20% das vezes: taquicardia (120-140 bpm)
if (random(100) < 20) {
  batimentos = random(120, 140);
} else {
  batimentos = random(60, 100);
}
```

#### Oxigênio SpO₂ (Simulado)
```cpp
// 85% das vezes: valores normais (95-100%)
// 15% das vezes: baixo oxigênio (85-90%)
if (random(100) < 15) {
  oxigenio = random(85, 90);
} else {
  oxigenio = random(95, 100);
}
```

### Protocolo MQTT (Simulado)

#### Tópico
```
healthguard/HealthGuard_001/dados
```

#### Formato do Payload
```json
{
  "device_id": "HealthGuard_001",
  "batimentos": 75,
  "oxigenio": 98,
  "temperatura": 36.5,
  "status": "NORMAL",
  "timestamp": 47520
}
```

> **Nota:** No Wokwi, o MQTT é apenas simulado (exibido no Serial Monitor).
> Para enviar dados MQTT reais, seria necessário usar a biblioteca PubSubClient.

---

## ⚙️ Configuração

### Parâmetros Ajustáveis no Código

#### 1. Identificação do Dispositivo
```cpp
const char* device_id = "HealthGuard_001";
// Altere para identificar diferentes dispositivos
```

#### 2. Intervalo de Leitura
```cpp
const long intervalo = 5000; // 5 segundos
// Mínimo recomendado: 1000ms (1 segundo)
// Máximo sugerido: 60000ms (1 minuto)
```

#### 3. Thresholds de Saúde

Localize a função `avaliarSaude()` e ajuste:

```cpp
// Valores NORMAIS
bool batimentoOK = (batimentos >= 60 && batimentos <= 100);
bool oxigenioOK = (oxigenio >= 95);
bool temperaturaOK = (temperatura >= 36.0 && temperatura <= 37.5);

// Valores CRÍTICOS
if (batimentos < 50 || batimentos > 120 || 
    oxigenio < 90 || 
    temperatura < 35.5 || temperatura > 38.5)
```

#### 4. Probabilidades de Simulação

Na função `lerSensores()`:

```cpp
// Taquicardia: 20% de chance
if (random(100) < 20) {  // Mude para 30 = 30% de chance
  batimentos = random(120, 140);
}

// Baixo oxigênio: 15% de chance
if (random(100) < 15) {  // Mude para 25 = 25% de chance
  oxigenio = random(85, 90);
}
```

---

## 📊 Especificações Técnicas (Wokwi)

### Desempenho Simulado

| Métrica | Valor |
|---------|-------|
| Tempo de inicialização | ~3 segundos |
| Intervalo entre leituras | 5 segundos |
| Latência de processamento | ~70 ms |
| Precisão DS18B20 | ±0.5°C (simulado) |

### Componentes do Circuito

| Componente | Especificação |
|------------|---------------|
| ESP32 | DevKit V1 (240 MHz, WiFi) |
| DS18B20 | Sensor digital de temperatura |
| LED RGB | Catodo comum, 3 cores |
| Resistores | Internos do Wokwi |

---

## 📞 Suporte

Para dúvidas sobre o projeto:

- **GitHub Issues:** [Criar issue](https://github.com/ghsiqueira/HealthGuard/issues)
- **Wokwi Community:** [Fórum Wokwi](https://wokwi.com/discord)
- **Email:** gabrielh2805@gmail.com

---

**Desenvolvido no Wokwi Simulator**

**Última atualização:** 20/11/2025

**Versão:** 1.0.0
