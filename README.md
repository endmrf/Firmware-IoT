# Firmware IoT

Este repositório contém o código-fonte para o firmware IoT desenvolvido para o dispositivo WGM160P. O firmware é projetado para fornecer uma solução robusta e eficiente para aplicações IoT, permitindo comunicação segura e gerenciamento dos dispositivos de tomada, interruptor e dimmer inteligentes.

## Conteúdo

- `src/`: Código-fonte principal do firmware.
- `docs/`: Documentação detalhada do firmware.
- `examples/`: Exemplos de utilização do firmware.
- `tests/`: Scripts de teste para validar a funcionalidade do firmware.

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


## Utilização
Para utilizar o firmware, siga os exemplos fornecidos na pasta examples/. Cada exemplo contém um README detalhado sobre como configurar e executar o código.

## Documentação
A documentação completa pode ser encontrada na pasta docs/. Certifique-se de ler a documentação para entender completamente as capacidades e limitações do firmware.

## Testes
Para executar os testes, use o comando:
  make test
  Os resultados dos testes serão exibidos no terminal. Para mais detalhes sobre os testes, consulte a pasta tests/.
