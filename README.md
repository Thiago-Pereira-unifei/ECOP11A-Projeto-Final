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

## Diagrama em blocos 

![image](https://github.com/user-attachments/assets/a7ed25e5-d98b-4da0-a22d-dfd27c382ade)
<div align="center">
