#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
 
// Mude para 434.0 ou outra frequência, deve corresponder ao RX's frequencia!
#define RF95_FREQ 868.0
 
// Instância singleton do driver de radio
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Pisca no recebimento
#define LED 13
 
void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  Serial.println("Teste Arduino LoRa RX!");
  
 // reset manual 
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("Falha na inicialização do rádio LoRa");
    while (1);
  }
  Serial.println("Iniciar radio LoRa  OK!");
 
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
 
void loop()
{
  if (rf95.available())
  {
    // Deve ser uma mensagem para nós agora    
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Recebido: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
       Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      
      // Send a reply
      uint8_t data[] = "E olá de volta Cliente";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Enviou uma resposta");
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Falha no recebimento");
    }
  }
}