# ECOP11A - Projeto Final Introducao a Engenharia
Introdução a Programacao: Projeto final com introducao a engenharia e metodo cientifico

## Equipe

-Higor Ribeiro  
-Joao kinjo  
-Thiago Pereira  
-Tulio franciscon  
-Weister Oliveira  

## Objetivo

O objetivo principal deste projeto é criar um sistema automatizado para classificar peças em três categorias de tamanho (pequena, média e grande) e direcioná-las usando um servo motor.  

Para alcançar esse objetivo, o sistema utiliza os seguintes componentes e lógicas:  

Detecção de Presença: Um sensor ultrassônico é usado para detectar a presença de uma peça quando ela se aproxima a menos de 10 cm do sensor.  
Classificação de Tamanho: Três sensores infravermelhos são estrategicamente posicionados para identificar o tamanho da peça com base em quantos sensores são ativados simultaneamente:  
Peça Pequena: Ativa apenas um sensor IR.  
Peça Média: Ativa dois sensores IR.  
Peça Grande: Ativa todos os três sensores IR.  
Indicação Visual: Três LEDs indicam visualmente a categoria de tamanho da peça detectada.  
Direcionamento Automatizado: Um servo motor (cancela) atua como um mecanismo de direcionamento, movendo-se para uma posição específica correspondente ao tamanho da peça detectada, presumivelmente para direcioná-la para diferentes compartimentos ou esteiras.  
Controle e Log: O código do ESP32 gerencia a leitura dos sensores, a lógica de classificação, o controle do servo e a comunicação serial para depuração e monitoramento.  
Em resumo, o projeto simula um sistema de triagem simples que pode ser aplicado em linhas de produção para automatizar a separação de objetos com base em seu tamanho.  

##Hipótese

Se um sistema automatizado for implementado utilizando sensores ultrassônicos e infravermelhos conectados a um microcontrolador ESP32, então será possível detectar a presença de peças em uma esteira, classificá-las de acordo com o seu tamanho (pequena, média ou grande) e direcioná-las adequadamente por meio de um servo motor, garantindo maior eficiência e precisão no processo de triagem automatizada em linhas de produção.

##Conclusão

O sistema desenvolvido atendeu com sucesso ao objetivo proposto, demonstrando ser capaz de identificar a presença de peças utilizando um sensor ultrassônico e classificá-las corretamente com base no número de sensores infravermelhos ativados. A lógica implementada permitiu controlar LEDs indicativos e um servo motor que atuou como uma cancela direcional, redirecionando as peças conforme seu tamanho. Além disso, o acionamento automático da esteira garantiu o fluxo contínuo das peças durante o processo. Portanto, conclui-se que a hipótese foi validada: é viável utilizar sensores simples e um microcontrolador ESP32 para criar um sistema eficiente de triagem de peças, com potencial aplicação em ambientes industriais e educacionais.

## Diagrama da montagem na ESP32

<p align="center">
  <img src="https://github.com/user-attachments/assets/a7ed25e5-d98b-4da0-a22d-dfd27c382ade" alt="Imagem centralizada" width="400">
</p>

## Diagrama em blocos

<p align="center">
  <img src="https://github.com/user-attachments/assets/ca7d7462-ce95-42d5-a282-6a5ed5bb6f97" alt="Imagem centralizada" width="400">
</p>
Para melhor visualizacao do diagrama, segue o link: https://miro.com/welcomeonboard/Zjh4YUNFWVlRbGUwYThXUnJKWkRTdEx2UE9zZlhVSllwTVlyWmlpZlc4aXdXNG8xM0xQSTAvV0ZYUHJWUU5SWEZzRFdWYkdYcndqZC96emRGYjlpbDRTMytnLzVBTWdVZGs2ckIyY211ZzFHTGRaazVxNnBoU3U5dG5wdXpuTUJhWWluRVAxeXRuUUgwWDl3Mk1qRGVRPT0hdjE=?share_link_id=345214892846

