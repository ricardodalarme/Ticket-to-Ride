using namespace std;
#include <bits/stdc++.h>
#define num_Vagoes 110
#define num_Cores 9
#define Num_Cidades 36
#define INF 0x3F3F3F3F

string Cor[] = {"Cinza", "Roxo", "Azul", "Laranja", "Branco", "Verde", "Amarelo", "Preto", "Vermelho"}; // Representação dos vagões
string Cidade[] = {"Vancouver", "Calgary", "Winnipeg", "Sault Ste. Marie", "Montreal", "Seattle", "Portland", "Helena", "Duluth", "Toronto", "Boston", "Salt Lake City", "San Francisco", "Denver", "Omaha", "Chicago", "Pittsburgh", "New York City", "Las Vegas", "Los Angeles", "Santa Fe", "Oklahoma City", "Kansas City", "Saint Louis", "Nashville", "Washington", "Raleigh", "Little Rock", "Atlanta", "Charleston", "Phoenix", "El Paso", "Dallas", "Houston", "New Orleans", "Miami"};

struct Rota_Estrutura { // estrutura das rotas do mapa
    int Destino;
    int Jogador;
    int Comprimento;
    int Cor;
};

struct Destino_Estrutura{ // estrutura das cartas de destino
    int Cidade_A;
    int Cidade_B;
    int Pontos;
};

struct Jogador_Estrutura{ // estrutura do jogador
    vector<Destino_Estrutura> Destino;
    int Vagao[9];
    int Pontos;
};

int rodada = 1, jogador_final = -1;
int num_Rotas, num_Destinos, num_Jogadores;
int Viradas_Vagoes[5]; // cartas de vagões viradas
vector<int> Cartas_Vagoes; // monte das cartas de vagões (N=110)
deque<Destino_Estrutura> Cartas_Destino; // monte das cartas de destino
vector<Jogador_Estrutura> Jogador; 
vector<Rota_Estrutura> Mapa[Num_Cidades]; // mapa do jogo representado por um vetor de adjacências
int dist[Num_Cidades];

void Ler_Dados(); // Leitura dos dados das cartas e rotas
void Introducao(); // Introdução ao jogo
void Distribuir_Cartas(); // Distribui todas as cartas aos jogadores e aos montes de forma aleatória
bool Pegar_Destino(int Jogador_Num, int min);  // compra uma carta destino
bool Pegar_Vagao(int Jogador_Num); // compra uma carta do topo do monte ou uma virada
bool Conquistar_Rota(int Jogador_Num); // conquista uma rota da cidade A até a cidade B
bool Tentar_Conquistar(int Jogador_Num, int c1, int c2, int cor, int vagao); // faz as necessárias verificações para a conquista da rota
int Encontrar_Aresta(int saida, int destino, int cor); // procura determinada arestra
bool Alcancar_Cidade(int Jogador_Num, int s, int procura); // verifica se o jogador consegue alcançar determinada cidade
void Maior_Caminho(); // encontra o jogador que conquistou o maior caminho contínuo
int Maior_Caminho(int Jogador_Num, int s); // encontra o maior caminho contínuo que determinado jogador conquistou
void Rodadas(); // Funcionamento geral de cada rodada
void Verificador_Vencedor(); // Verifica quem venceu o jogo
void Cabecalho(int vez); // cabeçalho das rodadas
void Limpar_Tela(); // limpa a tela do console (Funciona tanto para Windows quanto para Linux)

int main(){
    // lê os dados do jogo
    Ler_Dados();

    // inicia o jogo
    Introducao();
}

void Ler_Dados(){
    int i, d1, d2;
    Rota_Estrutura rota;
    Destino_Estrutura destino;
    FILE *f_rotas = fopen("rotas.txt", "r"), *f_destinos = fopen("destinos.txt", "r");

    // verifica se os arquivos de dados existem 
    if (f_rotas == NULL || f_destinos == NULL) {
        printf("Os arquivos de dados não conseguiram ser lidos.");
        exit(-1);
    }

    // lê os dados das rotas
    fscanf(f_rotas, "%d", &num_Rotas);
    rota.Jogador = 0;
    for (i = 0 ; i < num_Rotas ; i++) {
        fscanf(f_rotas, "%d %d %d %d", &d1, &d2, &rota.Comprimento, &rota.Cor);
        rota.Jogador = -1;
        rota.Destino = d2, Mapa[d1].push_back(rota);
        rota.Destino = d1, Mapa[d2].push_back(rota);
    }

    // lê os dados das cartas destino
    fscanf(f_destinos, "%d", &num_Destinos);
    for (i = 0 ; i < num_Destinos; i++){
        fscanf(f_destinos, "%d %d %d", &destino.Cidade_A, &destino.Cidade_B, &destino.Pontos);
        Cartas_Destino.push_back(destino);
    }

    // termina a leitura dos arquivos
    fclose(f_rotas);
    fclose(f_destinos);
}

void Introducao(){
    // mensagem introdutória 2ao jogo
    printf("\n[TICKET TO RIDE]\n-----------------------------------------\n\nTicket to Ride e um jogo de aventura ferroviaria pelo mapa dos Estados Unidos. Os jogadores competem para conectar diferentes cidades conquistando rotas de trem pelo mapa.\n\nRegras: http://galapagosjogos1.hospedagemdesites.ws/PDFs/Jogo-Ticket-to-Ride_regras.pdf\nMapa: https://images-na.ssl-images-amazon.com/images/I/B19d%%2BVcYwWS.png\nObs.: Como nao ha uma representacao grafica do jogo, a representacao do mapa deve ser feito externamente pelo jogador.\nObs.: Para ficar mais fácil o entedimento, considere as cartas locomotivas como as cartas de cor cinza.\n\n");

    // vê quantos jogadores vão participar
    printf("[INTRODUCAO]\nQuantos jogadores participarao do jogo? (Min: 2 e Max: 5) ");
    do{  
        scanf("%d", &num_Jogadores);
    } while ((num_Jogadores < 2 || num_Jogadores > 5));

    Jogador.resize(num_Jogadores);
    Limpar_Tela();

    // distribui as cartas e inicia as rodadas
    Distribuir_Cartas();
    Rodadas();
}

void Distribuir_Cartas(){
    int i, j;

    // distribui as cartas dos vagões
    Cartas_Vagoes.resize(num_Vagoes);
    for (i = 14 ; i < num_Vagoes ; i++) Cartas_Vagoes[i] = ((i- 14) / 12) + 1;

    // embaralha as cartas
    random_shuffle(Cartas_Vagoes.begin(), Cartas_Vagoes.end());
    random_shuffle(Cartas_Destino.begin(), Cartas_Destino.end());
    
    // distribui as cartas destino aos jogadores
    for (i = 0 ; i < num_Jogadores ; i++){
        printf("\n[DISTRIBUICAO]\n---------------------------------------\n\n[Jogador %d]\n", i + 1);
        Pegar_Destino(i, 2);
        Limpar_Tela();
    }

    // distribui as cartas vagões aos jogadores
    for (i = 0 ; i < num_Jogadores; i++)
        for (j = 0 ; j < 3 ; j++){
            Jogador[i].Vagao[Cartas_Vagoes[Cartas_Vagoes.size() - 1]] += 1;
            Cartas_Vagoes.pop_back();
        }

    // vira as cartas vagões
    for (i = 0 ; i < 5 ; i++) Viradas_Vagoes[i] = Cartas_Vagoes[Cartas_Vagoes.size() - 1], Cartas_Vagoes.pop_back();
}

bool Pegar_Destino(int Jogador_Num, int min){
    int i, j, n, max = 3, total = Cartas_Destino.size();

    // determina o limite maximo de cartas que podem ser pegas
    if (total == 0) {
        printf("Não e possivel realizar essa acao\n");
        return false;
    }
    else if (total < 3) max = n;

    // demonstra três cartas destino para o jogador escolher
    pair<int, Destino_Estrutura> escolha[max];
    printf("Cartas de destino disponiveis para escolha:\n");
    for (i = 0 ; i < max ; i++){
        j = total - i - 1;
        cout << "   [" << i + 1 << "] " << Cidade[Cartas_Destino[j].Cidade_A] << " -> " << Cidade[Cartas_Destino[j].Cidade_B] << " (" << Cartas_Destino[j].Pontos << "pts)\n";
        escolha[i] = {0, Cartas_Destino[j]};
        Cartas_Destino.pop_back();
    }

    // permite que o jogador escolha um minimo de cartas
    printf("Quantas cartas voce deseja escolher? (No minimo %d) ", min);
    do {
        scanf("%d", &n);
    } while (n < min || n > max);

    printf("Quais? ");
    for (i = 0 ; i < n ; i++){
        do {
            scanf("%d", &j);
        } while (j < 1 || j > max || escolha[j - 1].first);
        escolha[j - 1].first = 1;
    }

    // distribui as cartas escolhidas ao jogador e retorna as restantes para o fundo do baralho
    for (i = 0 ; i < max ; i++)
        if (escolha[i].first) Jogador[Jogador_Num].Destino.push_back(escolha[i].second);
        else Cartas_Destino.push_front(escolha[i].second);

    return true;
}

bool Pegar_Vagao(int Jogador_Num){
    int pontos = 2, modo, topo, i, check = 0;
    bool check1 = false, check2 = false;

    // verifica se é possível fazer essa jogada
    for (i = 0 ; i < 5; i++) check += Viradas_Vagoes[i];
    if (check < 0 && !Cartas_Vagoes.size()) {
        printf("Nao e possivel realizar essa acao\n");
        return false;
    }

    while (pontos){
        // demonstra o cabeçalho
        Limpar_Tela();
        Cabecalho(Jogador_Num);

        // recebe de qual forma o jogador vai querer pegar uma carta
        printf("Acoes:\n   [1] Pegar uma carta das cartas viradas\n   [2] Pegar uma carta do topo do monte\nEscolha uma acao: ");
        do {
            scanf("%d", &modo);
        }
        while (modo < 1 || modo > 2);
        
        // pegar uma carta virada
        if (modo == 1){
            // verifica se é possível pegar uma carta das viradas
            check = 3; // 3 significa que não há nenhuma carta no monte
            for (i = 0 ; i < 5; i++) {
                if (Viradas_Vagoes[i] == 0) min(check, 2);
                else if (Viradas_Vagoes[i] == 1) check = 1;
            }
            if (pontos - check < 0 || check == 3){
                printf("Voce nao consegue realizar essa acao.\n");
                check1 = true;
                continue;
            }

            // escolha de uma das cartas viradas
            printf("Escolha uma carta virada: ");
            do {
                scanf("%d", &topo);
                topo--;
            }
            while (topo < 0 || topo > 4 || Viradas_Vagoes[topo] == -1 || (Viradas_Vagoes[topo] == 0 && pontos == 1));
            Jogador[Jogador_Num].Vagao[Viradas_Vagoes[topo]]++;
            if (Viradas_Vagoes[topo] == 0) pontos -=2;
            else pontos -= 1;
            cout << "Carta pega e da cor " << Cor[Viradas_Vagoes[topo]] << "\n\n";

            // coloca uma carta no lugar
            if (Cartas_Vagoes.size()){
                Viradas_Vagoes[topo] = Cartas_Vagoes[Cartas_Vagoes.size() - 1];
                Cartas_Vagoes.pop_back();
            }
            else Viradas_Vagoes[topo] = -1;
        }
        // pegar uma carta do topo monte
        else {
            // verifica se é possível pegar uma carta do monte
            if (!Cartas_Vagoes.size()){
                printf("Nao ha cartas disponiveis no monte\n\n");
                check2 = true;
                continue;
            }
            
            topo = Cartas_Vagoes[Cartas_Vagoes.size() - 1];
            Jogador[Jogador_Num].Vagao[topo] += 1;
            pontos -= 1;
            Cartas_Vagoes.pop_back();
            cout << "Carta pega e da cor " << Cor[topo] << "\n\n";
        }

        // caso o jogador não consiga realizar mais nenhuma ação, termina a jogada dele
        if (check1 && check2) break;
    }

    return true;
}

bool Conquistar_Rota(int Jogador_Num){
    int i, c1, c2, cor, vagao;
    Jogador[0].Vagao[0] = 10000;
    // demonstra a indexação das cidades e das cores
    printf("Cidades:\n");
    for (i = 0 ; i < Num_Cidades; i++) cout << "   [" << i + 1 << "] " << Cidade[i] << endl;
    printf("Cores:\n");
    for (i = 0 ; i < num_Cores; i++) cout << "   [" << i + 1 << "] " << Cor[i] << endl;
    
    // recebe qual caminho o jogador quer conquistar
    printf("Qual e a cidade de saida, a de destino, a cor da rota e a cor da peca que voce quer usar? (caso voce nao tenha pecas suficientes para conquistar sera tentando utilizar as pecas de locomotias) ");
    do {
        scanf("%d %d %d %d", &c1, &c2, &cor, &vagao);
    }
    while (!Tentar_Conquistar(Jogador_Num, c1, c2, cor, vagao));
    
    // verifica se completou algum das missões
    for (auto i = Jogador[Jogador_Num].Destino.begin(); i != Jogador[Jogador_Num].Destino.end() ; i++) 
        if (Alcancar_Cidade(Jogador_Num, i->Cidade_A, i->Cidade_B)){
            Jogador[Jogador_Num].Pontos += i->Pontos;
            Jogador[Jogador_Num].Destino.erase(i);
        }

    return true;
}

bool Tentar_Conquistar(int Jogador_Num, int c1, int c2, int cor, int vagao){
    // verifica se os valores estão dentro dos limites
    if (c1 < 1 || c2 < 1 || c1 > Num_Cidades || c2 > Num_Cidades) return false;
    if (cor < 1 || vagao < 1 || cor > num_Cores || vagao > num_Cores) return false;
    
    // corrige os valores
    --c1, --c2, --cor, --vagao;

    // encontra a arestra que o jogador quer conquistar e verifica se ela já foi conquistada
    int v1 = Encontrar_Aresta(c1, c2, cor), v2 = Encontrar_Aresta(c2, c1, cor);
    if (v1 < 0) return false;

    // quantidade de peças que o jogador tem disponivel para conqusitar
    int total;
    if (vagao == 0) total = Jogador[Jogador_Num].Vagao[vagao];
    else total = Jogador[Jogador_Num].Vagao[vagao] + Jogador[Jogador_Num].Vagao[0];

    // verifica se e possivel conqusitar a rota
    if (Mapa[c1][v2].Cor != 0 && Mapa[c1][v2].Cor != vagao) return false;
    if (total < Mapa[c1][v2].Comprimento) return false;

    if (vagao == 0 || Jogador[Jogador_Num].Vagao[vagao] >= Mapa[c1][v2].Comprimento) Jogador[Jogador_Num].Vagao[vagao] -= Mapa[c1][v2].Comprimento;
    else {
        Jogador[Jogador_Num].Vagao[0] -= Mapa[c1][v2].Comprimento - Jogador[Jogador_Num].Vagao[vagao];
        Jogador[Jogador_Num].Vagao[vagao] = 0;
    }

    // conquista a aresta
    Mapa[c1][v1].Jogador = Jogador_Num;
    Mapa[c2][v2].Jogador = Jogador_Num;

    // pontuação do jogador
    switch (Mapa[c1][v2].Comprimento) {
        case 1: total = 1; break;
        case 2: total = 2; break;
        case 3: total = 4; break;
        case 4: total = 7; break;
        case 5: total = 10; break;
        case 6: total = 15; break;
    }
    Jogador[Jogador_Num].Pontos += total;
    return true;
}

// -2 = não encontrou, -1 encontrou mas ja esta ocupada, != aresta destino
int Encontrar_Aresta(int saida, int destino, int cor){
    int ans = -2, i;

    for (i = 0 ; i < Mapa[saida].size(); i++){
        if (Mapa[saida][i].Destino == destino && Mapa[saida][i].Cor == cor)
            if (Mapa[saida][i].Jogador == -1) return i;
            else ans = -1;
    }
    printf("\nans: %d\n", ans);
    return ans;
}

bool Alcancar_Cidade(int Jogador_Num, int s, int procura) {
    memset(dist, INF, sizeof(dist));
    dist[s] = 0;
    queue<int> q;
    q.push(s);

    while (!q.empty())  {
        int x = q.front();
        int d = dist[x];
        q.pop();

        for (int i = 0; i < Mapa[x].size(); i++)
            if (Mapa[x][i].Jogador == Jogador_Num) {
                int y = Mapa[x][i].Destino;
                if (y == procura) return true;

                if (d + 1 < dist[y]) {
                    dist[y] = d + 1;
                    q.push(y);
                }
            }
    }

    return false;
}

void Maior_Caminho() {
    int i, j, maior = 0, maior_jogador[num_Jogadores], total;

    // encontra o maior caminho de cada jogador
    for (j = 0 ; j < num_Jogadores ; j++){
        total = 0;
        for (i = 0 ; i < Num_Cidades; i++){
            memset(dist, false, sizeof(dist));
            total = max(total, Maior_Caminho(j, i));
        }
        maior_jogador[j] = total;
        maior = max(maior, total);
    }

    // dá 10 a mais de pontos para todos os jogadores com o maior caminho
    for (j = 0 ; j < num_Jogadores ; j++)
        if (maior_jogador[j] == maior)
            Jogador[j].Pontos += 10;
}

int Maior_Caminho(int Jogador_Num, int s){
    dist[s] = true;
    for (int i = 0; i < Mapa[s].size(); i++) 
        if (Mapa[s][i].Jogador == Jogador_Num) {
            int y = Mapa[s][i].Destino;
            if (!dist[y])
                return Maior_Caminho(Jogador_Num, y) + 1;
        }
    return 0;
}

void Verificador_Vencedor(){
    int i;

    // limpa a tela do console
    Limpar_Tela();

    // dá um bônus a quem conseguiu o maior caminho
    Maior_Caminho();

    // exibe as classificações
    printf("\n[RESULTADO]\n---------------------------------------\n\n");
    vector<pair <int, int> > resultados;
    for (i = 0 ; i < num_Jogadores; i++) resultados.push_back(make_pair(Jogador[i].Pontos, i + 1));
    sort(resultados.rbegin(), resultados.rend());
    for (i = 0 ; i < num_Jogadores; i++) printf("[%d] Jogador %d (%d)\n", i + 1, resultados[i].second, resultados[i].first);
}

void Rodadas(){
    int vez, i, acao, sum;
    bool continuidade;

    while (rodada){
        for (vez = 0; vez < num_Jogadores; vez++){
            // demonstra o cabeçalho e informações relevantes
            Cabecalho(vez);

            // realiza uma ação
            continuidade = true;
            do {
                printf("\nAcoes:\n   [1] Comprar cartas vagao\n   [2] Comprar cartas destino\n   [3] Conquistar uma rota\nEscolha uma acao: ");
                do {
                    scanf("%d", &acao);
                } while (acao < 1 || acao > 3);
                printf("\n");

                switch (acao) {
                    case 1: continuidade = Pegar_Vagao(vez); break;
                    case 2: continuidade = Pegar_Destino(vez, 1); break;
                    case 3: continuidade = Conquistar_Rota(vez); break;
                    case 4: return;
                }
            } while (!continuidade);

            // verifica se o jogo terminou
            if (jogador_final == vez) {
                rodada = -1;
                break;
            }

            // verifica se o jogo terminrá na próxima rodada
            sum = 0;
            for (i = 0; i < 8; i++) sum += Jogador[vez].Vagao[i];
            if (sum < 3) jogador_final = vez;

            // limpa a tela para os demais jogadores não verem as cartas
            Limpar_Tela();
        }
        rodada++;
    }

    // verifica quem ganhou
    Verificador_Vencedor();
}

void Cabecalho(int vez){
    int i;
    Destino_Estrutura jogador_destino;

    // cabeçalho
    printf("\n[RODADA %d]\n", rodada);
    if (jogador_final != -1) printf("OBS.: O jogo acabara apos a jogado do jogador %d\n", jogador_final + 1);
    printf("-------------------------------------------\n\n[Jogador %d]\n", vez + 1);

    // mostra informações sobre o jogador
    printf("Pontos: %d\n", Jogador[vez].Pontos);
    printf("Cartas vagoes:\n");
    for (i = 0 ; i < 9; i++)
        if (Jogador[vez].Vagao[i])
            cout << "   (" << Jogador[vez].Vagao[i] << ")" << " " << Cor[i] << endl;
            
    printf("Cartas destino:\n");
    for (i = 0 ; i < Jogador[vez].Destino.size() ; i++) {
        jogador_destino = Jogador[vez].Destino[i];
        cout << "   " << Cidade[jogador_destino.Cidade_A] << " -> " << Cidade[jogador_destino.Cidade_B] << " (" << jogador_destino.Pontos << "pts)\n";
    }

    // mostra as cartas viradas
    printf("\nCartas viradas:\n");
    for (i = 0 ; i < 5; i++) 
        if (Viradas_Vagoes[i] != -1) cout << "   [" << i + 1 << "] " << Cor[Viradas_Vagoes[i]] << endl;
}

void Limpar_Tela(){
    system("@cls||clear");
}