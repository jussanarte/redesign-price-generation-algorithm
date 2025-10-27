# 🚀 Algoritmo Simples de Precificação Dinâmica em C

Este projeto implementa um algoritmo de precificação dinâmica para simular como plataformas de transporte (como táxis por aplicativo) calculam o preço final de uma corrida.

O objetivo é combinar um **Custo Fixo Operacional** (baseado em distância e tempo) com um **Fator Dinâmico** que se ajusta às condições do mercado.

## 🎯 Como o Preço é Calculado

O preço final é determinado em duas etapas principais:

### 1. Cálculo do Custo Base

O Custo Base é a soma de custos fixos e variáveis, usando valores predefinidos em Kwanza Angolano (AOA):

| Constante | Valor | Descrição |
| :--- | :--- | :--- |
| **Taxa Fixa** | 300.0 AOA | Preço inicial. |
| **Custo por KM** | 100.0 AOA | Custo base por quilômetro. |
| **Custo por Minuto**| 50.0 AOA | Custo base por minuto. |

$$\text{Custo Base} = 300 + (\text{KM} \times 100) + (\text{Minutos} \times 50)$$

### 2. Aplicação do Fator Dinâmico

O Fator Dinâmico (`finalFactor`) é um multiplicador que ajusta o preço com base nas condições atuais.

| Fator | Lógica de Cálculo | Efeito no Preço |
| :--- | :--- | :--- |
| **Mercado** | Relação **Demanda / Oferta**. | Aumenta drasticamente se houver pouca oferta. |
| **Geografia** | Fator lido do arquivo `zonas.txt`. | Ajusta o preço com base na área (ex: aeroporto, zona residencial). |
| **Pico** | Horários de 7h-9h e 17h-20h. | Multiplica o preço por **x1.3**. |
| **Condições** | Clima (chuva), Tráfego e Eventos. | Aplicam múltiplos ajustes (ex: x1.5 para Evento; x1.3 para Tráfego). |
| **Limite** | O fator final é limitado a **x3.0**. | Impede preços abusivos. |

## 💰 Preço Final

O preço final é o **Custo Base multiplicado pelo Fator Dinâmico**, garantindo sempre um **Preço Mínimo de 450.0 AOA**.

---

## 🛠️ Para Rodar o Projeto

O programa lê todos os dados de entrada de arquivos de texto (`data.txt` e `zonas.txt`) e os armazena na estrutura `RideData` para calcular o valor.

1. Compile o código-fonte C.
2. Certifique-se de que os arquivos de dados estejam na pasta **`Arquivos/`**.
3. Execute o programa para obter o preço final.

*Este projeto é modular: a lógica da Zona Geográfica é lida de um arquivo, separando-a do algoritmo de precificação principal.*
