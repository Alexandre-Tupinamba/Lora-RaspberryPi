// LoRa 9x_TX 
// -*- modo: C++ -*- 
// Esboço de exemplo mostrando como criar um cliente de mensagens simples (transmissor) 
// com a classe RH_RF95. A classe RH_RF95 não fornece endereçamento ou 
// confiabilidade, então você só deve usar RH_RF95 se não precisar das  habilidades de mensagens de nível // mais alto. 
// Ele foi projetado para funcionar com o outro exemplo LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Mude para 434.0 ou outra frequência, deve corresponder a RX's frequencia! 
#define RF95_FREQ 868.0

// Instância singleton do driver de  radio  
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Teste Arduino LoRa TX!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("Falha na inicialização do rádio LoRa");
    while (1);
  }
  Serial.println("Iniciar  radio init OK!");

// Os padrões após a inicialização são 434.0MHz, modulação GFSK_Rb250Fd250, +13dbM 
if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Definir Freq para: "); Serial.println(RF95_FREQ);
  
// Os padrões após a inicialização são 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on 

  // A potência padrão do transmissor é 13dBm, usando PA_BOOST. 
  // Se você estiver usando módulos RFM95/96/97/98 que usam o pino do transmissor PA_BOOST, 
  // você pode definir as potências do transmissor de 5 a 23 dBm: 
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // contador de pacotes, incrementamos por xmission

void loop()
{
  Serial.println(" Enviando para server");
// Envia uma mensagem para  server 
  
  char radiopacket[20] = "OLÁ SERVER #      ";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Enviando "); Serial.println(radiopacket);
  radiopacket[19] = 0;
  
  Serial.println("Enviando..."); delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Aguardando a conclusão do pacote..."); delay(10);
  rf95.waitPacketSent();
  // Agora aguarde uma resposta 
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Aguardando resposta..."); delay(10);
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Deve ser uma mensagem de resposta para nós agor  
    if (rf95.recv(buf, &len))
   {
      Serial.print("Resposta: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Falha no recebimento");
    }
  }
  else
  {
    Serial.println("Nenhuma resposta, há um ouvinte por perto?");
  }
  delay(1000);
}