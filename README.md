# Ticket to Ride

## Introdução
Esse é um trabalho da disciplina de Teoria de Grafos (ECOM02), ministrada pelo
professor Edmilson Marmo Moreira em 2019, e consiste na implementação do jogo de tabuleiro “Ticket to
Ride” na linguagem C++.

## Princípios de funcionamento
O primeiro passo do algoritmo é ler os dados das rotas e cartas por meio de arquivos.
Em seguida, são distribuídas as cartas aos jogadores e então as rodadas começam. Em cada
rodada, o jogador tem três opções de execução:
1. Comprar cartas vagão
2. Comprar cartas destino
3. Conquistar uma rota

Quando algum jogador estiver com apenas 3 ou menos cartas vagão, o jogo continua
por mais uma rodada e então é calculado a pontuação final dos jogadores.

## Análise do projeto

Em geral, tudo está funcionando muito bem, foi implementado todas as funcionalidades
e particularidades do jogo original. Com apenas um breve conhecimento sobre as regras,
qualquer pessoa consegue ter uma boa experiência com o jogo pelo programa desenvolvido.

Entretanto, como o projeto foi desenvolvido para uma plataforma console, a interface é
extremamente simples e não muito intuitiva, para isso seria necessário trabalhar com uma
plataforma que permitisse uma melhor manipulação dos recursos gráficos e visuais.

Como é a implementação é um pouco complexa e o tempo para trabalho foi
relativamente curto, não foi possível fazer todos os testes necessários para averiguar a total
funcionalidade do projeto e encontrar possíveis bugs.

## Descrição das funções

- **void Ler_Dados():** Leitura dos dados das cartas e rotas.
- **void Introducao():** Introduz ao jogo e a algumas considerações iniciais.
- **void Distribuir_Cartas():** Distribui todas as cartas aos jogadores e aos montes de forma aleatória
- **bool Pegar_Vagao(int Jogador_Num):** Algum jogador compra uma carta do topo do
monte ou uma virada na mesa
- **bool Conquistar_Rota(int Jogador_Num):** Algum jogador conquista uma determinada
rota
- **bool Tentar_Conquistar(int Jogador_Num, int c1, int c2, int cor, int vagao):** Faz as
necessárias verificações para conquistar uma determinada rota
- **int Encontrar_Aresta(int saida, int destino, int cor):** Procura determinada aresta
- **bool Alcancar_Cidade(int Jogador_Num, int s, int procura):** Verifica se um determinado
consegue alcançar uma determinada cidade
- **void Maior_Caminho():** Encontra o jogador que conquistou o maior caminho contínuo
- **void Rodadas():** Funcionamento geral de cada rodada
- **void Verificador_Vencedor():** Verifica quem venceu o jogo
- **void Cabecalho(int vez):** Cabeçalho das rodadas
- **void Limpar_Tela():** Limpa a tela do console (Funciona tanto para Windows quanto para Linux)

## Informações adicionais
- [Regras e funcionamentos](http://galapagosjogos1.hospedagemdesites.ws/PDFs/Jogo-Ticket-to-Ride_regras.pdf)
- [Mapa das rotas](https://images-na.ssl-images-amazon.com/images/I/B19d%%2BVcYwWS.png)