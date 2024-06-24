# Firmware IoT

Este repositório contém o código-fonte para o firmware IoT desenvolvido para o dispositivo WGM160P. O firmware é projetado para fornecer uma solução robusta e eficiente para aplicações IoT, permitindo comunicação segura e gerenciamento dos dispositivos de tomada, interruptor e dimmer inteligentes.

## Conteúdo

- `FirmwareDIM/`: Código-fonte do firmware do dimmer.
- `FirmwareINT/`: Código-fonte do firmware do interruptor.
- `FirmwareTOM/`: Código-fonte do firmware da tomada.

## Requisitos

Antes de começar, certifique-se de que você possui os seguintes requisitos instalados:

- [GCC](https://gcc.gnu.org/) (GNU Compiler Collection)
- [CMake](https://cmake.org/)
- [Make](https://www.gnu.org/software/make/)
- [SDK-WGM160P](#sdk-wgm160p) (Silicon Labs)

## Instalação

1. Clone o repositório para o seu ambiente local:
  git clone https://github.com/endmrf/Firmware-IoT.git
  cd Firmware-IoT
   
2. Compile o Firmware:
  mkdir build
  cd build
  cmake ..
  make

3. Flash o firmware para o dispositivo:
   make flash


## Testes
Para executar os testes, use o comando:
  make test
