# Lora-RaspberryPi
Esse repositório vai descrever o passo a passo para fazer funcionar um shield dragino lora hat/gps com um RaspberryPi 3 b+, utilizando uma biblioteca escrita em python

<h1 align="center">  
 
  ![WhatsApp Image 2023-02-25 at 19 42 11](https://user-images.githubusercontent.com/60014649/221384638-3e3794de-e2e3-4bc7-9471-b9b713468cd2.jpeg)


</h1>

<p align="center">
 
 
> **Essa biblioteca é uma alternativa a radioHead em c++, possui as mesmas funções, e a usa como base, porém, em python.**
[RadioHead rfm95](https://github.com/hallard/RadioHead)

## Introdução

Este é um tutorial para explicar como funciona a conexão do shield dragino lora com o raspberry pi com um código em python.

A biblioteca com código que iremos utilizar é chamada de “pyLoraRFM9x”.

- **Para começar**

> O link para o projeto é esse:
https://pypi.org/project/pyLoraRFM9x/
voce irá encontrar uma breve documentação sobre como essa biblioteca foi desenvolvida.
>Antes de iniciar verifique se o python está instalado em sua maquina com o comando:
```bash
python3
```
caso não tenha instalado, instale com o seguinte comando:
```bash
apt-get install python3
```
> 
- **Configuração do raspberry**

> Após esse passo temos que habilitar o SPI do Raspberry Pi.
Esse passo é importante pois o usamos para coletar e enviar as informações pela placa lora.

o primeiro passo é:

no terminal digite:

```bash
sudo raspi-config
```
Deve abrir uma série de opções. 
Vá até a opção de número 5, “Interfacing Options”
Depois vá até a opção número 4, “SPI” e confirme.
Pronto, o SPI do raspberry está habilitado.

>Agora vamos instalar a biblioteca em python para utilizar o módulo dragino diretamente com o raspberry.
Execute o seguinte comando no terminal:

```bash
pip install --upgrade pyLoraRFM9x
```

após a instalação verifique se ele foi instalado corretamente com o seguinte comando:

```bash
ls -l /dev/spidev*
```

você deverá ver 2 progamas nessa aba.




## Montagem



>Primeiramente vamos precisar verificar o diagrama de pinos do rasperry pi b+ e o da placa dragino loraHat, que irá fazer o papel de gateway em conjunto com o raspberry. A seguir temos a pinagem da placa lora que iremos utilizar: 

![LoraHat](https://user-images.githubusercontent.com/60014649/221385006-6effad9c-bec1-4ca4-83a6-e69257884f74.png)
[LoraHat Site do projeto](https://wiki1.dragino.com/index.php/Lora/GPS_HAT)


>A seguir temos o diagrama de pinos do Raspberry b+

![gpio Raspberry](https://user-images.githubusercontent.com/60014649/221385103-09c5baa0-b14f-45cc-a5ec-c4b103cfc7fa.png)


# Conectando as duas placas

>com a ajuda do diagrama de pinos agora nós podemos conectar a placa dragino com o raspberry, pegue seus fios jumpers!

![Captura de tela de 2023-02-25 20-04-53](https://user-images.githubusercontent.com/60014649/221385369-5730ace0-0ecd-44f1-beab-0aa05fdd9457.png)
 >Conecte os fios de acordo com a descrição na tabela acima


 sua conexão deverá ficar dessa forma:
 ![WhatsApp Image 2023-02-25 at 19 42 11](https://user-images.githubusercontent.com/60014649/221385412-7e2c28e9-6498-4251-9f13-ed24ccdb295f.jpeg)
![WhatsApp Image 2023-02-25 at 19 42 10](https://user-images.githubusercontent.com/60014649/221385423-2178eccc-3ba9-4754-9dfd-d65e644f8e6d.jpeg)

## Após as conexões 

>Após a conexão ser feita podemos começar a testar o código de envio:

```python
from pyLoraRFM9x import LoRa, ModemConfig

# This is our callback function that runs when a message is received
def on_recv(payload):
    print("From:", payload.header_from)
    print("Received:", payload.message)
    print("RSSI: {}; SNR: {}".format(payload.rssi, payload.snr))

# Use chip select 1. GPIO pin 5 will be used for interrupts and set reset pin to 25
# The address of this device will be set to 2
lora = LoRa(1, 5, 2, reset_pin = 25, modem_config=ModemConfig.Bw125Cr45Sf128, tx_power=14, acks=True)
lora.on_recv = on_recv

# Send a message to a recipient device with address 10
# Retry sending the message twice if we don't get an  acknowledgment from the recipient
message = "Hello there!"
status = lora.send_to_wait(message, 10, retries=2)
if status is True:
    print("Message sent!")
else:
    print("No acknowledgment from recipient")
lora.close()

```

>crie um arquivo em python e coloque o código acima.


baixe os arquivos no repositório abaixo e coloque o código criado anteriormente na mesma pasta:
https://github.com/epeters13/pyLoraRFM9x



depois disso, vá para pasta onde está o seu arquivo python e execute com o seguinte comando:


```
~$ python3 <nome do arquivo>
```


>seu progama irá enviar 1 mensagem para o lora que tiver o enrdereço “10”, se quiser mudar e enviar para qualquer lora disponivel, mude o 10 em:
status = lora.send_to_wait(message, 10, retries=2), para 255.


>Do modo que está o módulo lora irá enviar 1 mensagem e encerrar o progama, para enviar mensagens contínuas será preciso colocar um while(true) ou algo similar.


>Para enviar mensagens de forma efetiva um lora precisa está recebendo essas mensagens, para isso, em um arduino, conecte um shield lora e use a biblioteca radiohead de preferência, pois é a mesma que é usada na biblioteca python.



