# 🧮 Precificação Dinâmica de Corridas — Surge Pricing com Matriz de Distâncias (C)

Ferramenta de linha de comando em **C** que calcula o preço final de uma corrida de transporte (táxi por aplicativo) combinando um **Custo Base Operacional** (distância × tempo) com um **Fator Dinâmico** — surge pricing — ajustado por condições de mercado e modulado pela **distância da rota A→B**, obtida de uma **matriz de distâncias**.

🧪 Desenvolvido para a cadeira de **Análise de Algoritmos** do **ISPTEC**.

## 📑 Índice

- [Objetivo](#objetivo)
- [Contexto académico](#contexto-académico)
- [Funcionalidades](#funcionalidades)
- [Como o preço é calculado](#como-o-preço-é-calculado)
- [Matriz de distâncias A→B](#matriz-de-distâncias-a→b)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Formatos dos dados de entrada](#formatos-dos-dados-de-entrada)
- [Requisitos](#requisitos)
- [Como executar](#como-executar)
- [Exemplo de execução](#exemplo-de-execução)
- [Próximas evoluções](#próximas-evoluções)
- [Integrantes](#integrantes)
- [Licença](#licença)

## 🎯 Objetivo

Demonstrar, na prática, um sistema de precificação dinâmica inspirado nas plataformas de transporte por aplicativo:

- 💰 um **Custo Base Operacional** determinado por distância e tempo;
- ⚡ um **Fator Dinâmico (surge pricing)** que reage a condições de mercado: relação demanda/oferta, horário de pico, clima, trânsito, eventos e zona geográfica;
- 🗺️ a **distância da rota A→B** calculada por uma **matriz de distâncias** entre zonas, que também influencia o próprio fator de surge (rotas mais longas reduzem suavemente o multiplicador).

O projeto serve de base para demonstrar conceitos da cadeira:

- 📥 leitura de dados estruturados a partir de arquivos externos (`data.txt`, `zonas.txt`, `matriz.txt`) sem recompilar;
- 🔍 busca de valores em uma matriz (matriz de distâncias);
- 🧩 modularização do código em C (`main.c` / `functions.c` / `functions.h`);
- 🧠 o impacto da parametrização externa no comportamento do algoritmo.

## 🏫 Contexto académico

| Campo | Valor |
|---|---|
| Instituição | Instituto Superior Politécnico de Tecnologias e Ciências (ISPTEC) |
| Departamento | Engenharia e Tecnologias |
| Curso | Licenciatura em Engenharia Informática |
| Cadeira | Análise de Algoritmos |
| Professor | Judson Paiva |
| Ano letivo | 2025/2026 |

## ⚙️ Funcionalidades

| Funcionalidade | Descrição |
|---|---|
| 📥 Entrada 100% parametrizada | Cenários definidos em arquivos de texto, sem alterar código |
| 🗺️ Matriz de distâncias A→B | Distância da rota lida de uma matriz entre zonas, com fallback para valor direto |
| ⚡ Fator dinâmico (surge) | Combina demanda/oferta, pico, clima, combustível, evento, tráfego e zona |
| 🛡️ Teto anti-abuso | Fator dinâmico limitado a **x3.0** |
| 📉 Modulação por distância | Rotas longas reduzem suavemente o multiplicador (taper até x0.75) |
| 💵 Preço mínimo | Preço final nunca abaixo de **450 AOA** |

## 🧮 Como o preço é calculado

O preço final é determinado em duas etapas principais.

### 1. Cálculo do Custo Base

O Custo Base é a soma de custos fixos e variáveis, usando valores predefinidos em Kwanza Angolano (AOA):

| Constante | Valor | Descrição |
| :--- | :--- | :--- |
| **Taxa Fixa** | 300.0 AOA | Preço inicial. |
| **Custo por KM** | 100.0 AOA | Custo base por quilômetro. |
| **Custo por Minuto** | 50.0 AOA | Custo base por minuto. |

$$\text{Custo Base} = 300 + (\text{KM} \times 100) + (\text{Minutos} \times 50)$$

### 2. Aplicação do Fator Dinâmico (Surge Pricing)

O Fator Dinâmico é um multiplicador que ajusta o preço com base nas condições atuais:

| Fator | Lógica de Cálculo | Efeito no Preço |
| :--- | :--- | :--- |
| **Mercado** | Relação **Demanda / Oferta** (se oferta ≤ 0, fator = 3.0). | Base do surge; sobe com a escassez de oferta. |
| **Pico** | Horários de 7h–9h e 17h–20h. | ×1.3 |
| **Clima** | Índice de severidade: `<0.5`, `0.5–0.75`, `>0.75`. | ×1.0 / ×1.2 / ×1.5 |
| **Combustível** | Preço do combustível igual a 300 ou 400 AOA. | ×1.2 |
| **Evento** | Fator de evento/feriado maior que 0. | ×1.5 |
| **Tráfego** | Fator de congestionamento maior que 0. | ×1.3 |
| **Zona** | Fator geográfico lido de `zonas.txt`. | Multiplica conforme a área. |
| **Distância** | Rota acima de 30 km (até 150 km). | Taper linear de ×1.0 a ×0.75. |
| **Limite** | O fator final é limitado a **x3.0**. | Impede preços abusivos. |

### 💰 Preço Final

$$\text{Preço Final} = \text{Custo Base} \times \text{Fator Dinâmico}$$

Garantindo sempre um **Preço Mínimo de 450.0 AOA**.

## 🗺️ Matriz de distâncias A→B

A distância da rota não é apenas um valor fixo: ela é derivada de uma **matriz de distâncias** entre as zonas de `zonas.txt`.

- A matriz é **simétrica** — `matriz[i][j]` = distância em km entre a zona `i` e a zona `j`.
- O arquivo `data.txt` informa `originZoneID` e `destinationZoneID`; o programa busca a distância na matriz.
- Se a rota não existir na matriz (IDs inválidos), o programa usa `distanceInKM` de `data.txt` como **fallback**.
- A distância resultante também **modula o surge**: rotas acima de 30 km reduzem linearmente o multiplicador, de ×1.0 até ×0.75 em 150 km, evitando preços exorbitantes em viagens longas.

## 📁 Estrutura do repositório

```
.
├── Arquivos/
│   ├── data.txt          # Parâmetros do cenário (entrada)
│   ├── zonas.txt         # Fatores por zona geográfica (entrada)
│   └── matriz.txt        # Matriz de distâncias A→B (entrada)
├── main.c                # Ponto de entrada
├── functions.c           # Implementação do algoritmo
├── functions.h           # Cabeçalho / definição da struct RideData
├── Makefile              # Build por linha de comando
├── README.md
├── LICENSE
├── .gitignore
└── .gitattributes
```

## 📄 Formatos dos dados de entrada

### `Arquivos/data.txt`

```
distanceInKM = 20            # Fallback se a rota não estiver na matriz
timeInMinutes = 30.0
demand = 10000.0
offer = 8000.0
geographicZoneID = 4         # Zona usada para o fator geográfico
originZoneID = 4             # Origem da rota (matriz A→B)
destinationZoneID = 5        # Destino da rota (matriz A→B)
peakHourFactor = 8.0         # Hora do dia (0–24)
eventOrHolidayFactor = 0.0   # > 0 = há evento/feriado
weatherFactor = 0.6          # Severidade do clima (0–1+)
trafficFactor = 1.3          # > 0 = há congestionamento
fuelPrice = 300
```

### `Arquivos/zonas.txt`

```
1  1.0   Zona Central
2  1.1   Zona Comercial
3  1.2   Zona Turística
4  0.9   Zona Residencial
5  1.3   Zona Aeroporto
```

Formato por linha: `ID` · `fator` · `nome`.

### `Arquivos/matriz.txt`

```
5
0 4 10 15 22
4 0 7 12 18
10 7 0 9 16
15 12 9 0 20
22 18 16 20 0
```

Formato: primeira linha = número de zonas `n`; depois `n` linhas com `n` valores — `matriz[i][j]` = distância em km entre as zonas `i` e `j` (1-indexada, mesma numeração de `zonas.txt`).

## 🧰 Requisitos

- Compilador **C (gcc)** — MinGW embutido numa IDE (Code::Blocks, Dev-C++, VS Code + MinGW, CLion) **ou** gcc via WSL / Git Bash / MSYS2.
- **make** (opcional, apenas para usar o `Makefile`).

## 🚀 Como executar

### Pela IDE

1. Crie/abra um projeto C com `main.c` e `functions.c` no mesmo diretório.
2. Compile e execute a partir da **raiz do projeto**.

### Por linha de comando

A partir da raiz do repositório:

```bash
make
make run
```

Ou manualmente:

```bash
gcc -Wall -Wextra -std=c11 main.c functions.c -o main.exe
./main.exe
```

> ⚠️ **Importante:** os arquivos de dados são lidos por caminho relativo (`Arquivos/...`). Execute sempre a partir da **raiz do projeto**, ou o programa não encontrará os dados.

## 🖥️ Exemplo de execução

Cenário de `data.txt` (rota Zona Residencial → Zona Aeroporto, 20 km; demanda 10 000 vs oferta 8 000; pico das 8h; chuva; tráfego):

```
Distance: 20.00 km
Time: 30.00 min
Demand: 10000.00
Fuel price: 300.00 AOA

Total price: 10403.64 AOA
```

O preço final de **10403.64 AOA** resulta de um Custo Base de 3800 AOA (300 + 20×100 + 30×50) multiplicado por um fator dinâmico de **2.7378** (demanda/oferta 1.25, pico ×1.3, clima ×1.2, combustível ×1.2, tráfego ×1.3, zona 0.9), abaixo do teto de x3.0.

## 🚧 Próximas evoluções

- **Piso para o fator dinâmico** — hoje o fator pode cair bem abaixo de 1 (descontos agressivos com oferta muito maior que a demanda); um piso evitaria precificação abaixo do custo.
- **Validação de leitura** — verificar o retorno de cada `fscanf` para tratar arquivos malformados.
- **Matriz em memória** — carregar `matriz.txt` uma única vez, em vez de reler a cada chamada.
- **Caminhos configuráveis** — permitir indicar a pasta de dados em vez de depender do diretório de execução.

## 👥 Integrantes

| Nome | Número de estudante |
|---|---|
| Isabel Marques | 20231832 |
| Jussana Paim | 20230132 |
| Norberto Cassoma | 20230873 |
| Oldmar Filindo | 20231359 |

## 📄 Licença

Distribuído sob a licença **MIT**. Veja o arquivo [LICENSE](LICENSE).
