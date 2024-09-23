// PROJETO DE APC
// ALUNO: GUSTAVO ALVES DIAS MATRÍCULA: 241020779
// PROFESSORA: CARLA CASTANHO


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

//estrutura que armazena os dados do jogador para atualizar no ranking

typedef struct                                      
{
    char nickname[20];
    int pontuacao;
} dados_jogador;


//variáveis globais: jogador atual, nivel atual e nivel anterior ao atual, escolha do menu, ponteiro para cada arquivo e arquivo do ranking

dados_jogador jogador;
int nivel_atual = 1;
int nivel_anterior = 1;
int escolha;
FILE *ponteiro_nivel_1 = NULL;
FILE *ponteiro_nivel_2 = NULL;
FILE *ponteiro_nivel_3  = NULL;
FILE *ponteiro_ranking = NULL;

char file_ranking[] = "ranking.bin";

//menu principal, função que guarda a opção selecionada pelo jogador no menu e retorna ela no loop da main

int menu_principal()
{

    printf("+++ JOGO DAS SOMAS +++\n");
    printf("\n1 - JOGAR\n");
    printf("\n2 - CONFIGURACOES\n");
    printf("\n3 - INSTRUCOES\n");
    printf("\n4 - RANKING\n");
    printf("\n5 - SAIR\n");
    printf("\nDigite a opcao desejada: "); int opcao; scanf("%d", &opcao);   //recebe a opção do jogador e retorna na main
    system(CLEAR);
    return opcao;

}

//fim do jogo, printa na tela a mensagem e retorna para o menu principal dentro da função jogar

void fim_jogo()
{
    printf("PARABENS! VOCE ZEROU O JOGO! :)\npontuacao final: %d\nespero que tenha se divertido!\n", jogador.pontuacao);
    getchar();
}

//função de atualizar o ranking, a qual é chamada sempre que um jogador ganha uma fase

void atualizar_ranking()
{
    dados_jogador jogador_temporario;
    FILE *fd2;
    fd2 = fopen("ranking.bin", "r+b");                                      // abre o arquivo para leitura ou escrita

    if(fd2 == NULL)
    {
        fd2 = fopen("ranking.bin", "w+b");                                  // se retornar nulo, cria o arquivo para leitura ou escrita
    }

    int verificador = 1;
    while(fread(&jogador_temporario, sizeof(dados_jogador), 1, fd2) == 1)   //enquanto tiver nomes para ler no ranking, ele verifica se o nome do jogador atual já está no ranking
    {
        if(strcmp(jogador_temporario.nickname, jogador.nickname) == 0)
        {
            fseek(fd2, -1*sizeof(dados_jogador), SEEK_CUR);                 // se estiver, ele atualiza a pontuação dele, a qual está armazenada na variável do jogador
            fwrite(&jogador, sizeof(dados_jogador), 1, fd2);
            verificador = 0;
            break;
        }
    }

    if(verificador)
    {
        fseek(fd2, 0, SEEK_END);
        fwrite(&jogador, sizeof(dados_jogador), 1, fd2);                    // se não estiver(ou seja, é um novo jogador), ele escreve o nome e a pontuação do jogador em uma nova linha
    }

    fclose(fd2);
    
}

// função de jogar, a qual é chamada casdo o jogador selecione a opção 1 no menu

void jogar()
{
    int nivel = nivel_atual;                                                // (nível 1: fácil, nível 2: médio, nível 3: difícil)
    FILE *fd;
    if(nivel == nivel_anterior)                                             // se o nível for igual ao anterior, mantém a ordem das fases
    {
        switch(nivel)                                                       // fd vai receber o ponteiro do nivel correspondente para operar com as matrizes
        {                                                                   // caso feof(fd) retorne diferente de 0, ou seja, chegou no final do arquivo, ele troca de nivel automaticamente
            case 1:                                                         // caso feof(fd) retorne diferente de 0 e esteja no último nível, ele chama a função fim_jogo e termina a execução do jogo(escolha = 5);
                fd = ponteiro_nivel_1;                                      
                if(feof(fd) != 0)                                           
                {
                    fd = ponteiro_nivel_2;
                }
                break;
            case 2:
                fd = ponteiro_nivel_2;
                if(feof(fd) != 0)
                {
                    fd = ponteiro_nivel_3;
                }
                break;
            case 3:
                fd = ponteiro_nivel_3;
                if(feof(fd) != 0)
                {
                    fim_jogo();
                    escolha  = 5;
                    return;
                }
                break;
        }
    } else {                                                                // se o nível não for igual ao anterior(ou seja, trocou de nível), volta para o início do novo modo de dificuldade
       
        switch(nivel)
        {
            case 1:
                fd = fopen("iniciante.txt", "r+");
                ponteiro_nivel_1 = fd;
                break;
            case 2:
                fd = fopen("intermediario.txt", "r+");
                ponteiro_nivel_2 = fd;
                break;
            case 3:
                fd = fopen("avancado.txt", "r+");
                ponteiro_nivel_3 = fd;
                break;
        }
    }
    

    int N;                                                                  // N é a variável para operar com o tamanho máximo das matrizes(n = 1, matriz 4x4, N = 4; n = 2, matriz 6x6, N = 6; n = 3, matriz 7x7, N = 7)
    if(nivel == 1)
    {
        N = nivel+3;
    } else {
        N = nivel+4;
    }

    char matriz[N][N];                                                      // a partir daqui, é feita a leitura das matrizes e da soma das colunas e linhas da matriz com o uso do fgets()
                                                                            // as matrizes são lidas como char para printar o espaço em branco ' ' quando um número for eliminado 
    for(int i=0; i<N; i++)
    {
        char linha[14];
        fgets(linha, 14, fd);
        for(int j=0; j<N; j++)
        {
            matriz[i][j] = linha[j];
        }
    }

    char proxima_linha[20];
    fgets(proxima_linha, 20, fd);

    int vetor_coluna[N];                                                    // a soma das linhas e colunas são armazenadas em vetores de int para facilitar a comparação se a soma de uma linha/coluna da matriz resultou na soma necessária daquela linha/coluna
    int index_coluna = 0;

    for(int i=0; i<N*2; i += 2)
    {
        if(proxima_linha[i] == '0')
        {
            vetor_coluna[index_coluna] = (proxima_linha[i+1] - '0');
            index_coluna++;
        } else {
            vetor_coluna[index_coluna] = (proxima_linha[i] - '0')*10 + (proxima_linha[i+1] - '0');
            index_coluna++;
        }
    }

    char proxima_linha1[20];
    fgets(proxima_linha1, 20, fd);

    int vetor_linha[N];
    int index_linha = 0;

    for(int i=0; i<N*2; i += 2)
    {
        if(proxima_linha1[i] == '0')
        {
            vetor_linha[index_linha] = (proxima_linha1[i+1] - '0');
            index_linha++;
        } else {
            vetor_linha[index_linha] = (proxima_linha1[i] - '0')*10 + (proxima_linha1[i+1] - '0');
            index_linha++;
        }
    }

    char matriz_resposta[N][N];

    for(int i=0; i<N; i++)
    {
        char linha1[14];
        fgets(linha1, 14, fd);
        for(int j=0; j<N; j++)
        {
            matriz_resposta[i][j] = linha1[j];
        }
    }

    int total_ap = 0;                                                       // se o total de números apagados necessários for igual ao total de números apagados, o loop do jogo se encerra e o jogador venceu
    int ap_atual = 0;                                              
    int vidas = 5;                                                          // se vidas do jogador acabar, o loop do jogo se encerra e o jogador perdeu

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            if(matriz_resposta[i][j] == '0')
            {
                total_ap++;
            }
        }
    }

    char linha_descartada[10];
    fgets(linha_descartada, sizeof(linha_descartada), fd);                  // descarta a estrelinha *, assim atualizando o offseat para a próxima matriz

    while(vidas && (total_ap != ap_atual))                                  // loop do jogo
    {                               
        printf("             ");                                            // print baseado no N(tamanho da matriz)
        for(int i=0; i<N; i++)
        {
            printf("%3d        ", vetor_coluna[i]);
        }
        printf("\n");

        for(int i=0; i<N; i++)
        {
            int j=0;
            printf("%3d     |   [%3c ]     [%3c ]     [%3c ]     [%3c ]", vetor_linha[i], matriz[i][j], matriz[i][j+1], matriz[i][j+2], matriz[i][j+3]);
            if(N == 6) 
            {
                printf("     [%3c ]     [%3c ]\n", matriz[i][j+4], matriz[i][j+5]);
            } else if(N == 7) {
                printf("     [%3c ]     [%3c ]     [%3c ]\n" ,matriz[i][j+4], matriz[i][j+5], matriz[i][j+6]);
            } else {
                printf("\n");
            }
        }

        int linha_ap, coluna_ap, condicao; 
        printf("\n** VOCE TEM %d VIDAS **\n", vidas);
        printf("Digite linha e coluna do elemento a ser apagado: "); scanf("%d %d", &linha_ap, &coluna_ap);

        if((linha_ap > 0 && linha_ap < N+1) && (coluna_ap > 0 && coluna_ap < N+1))
        {
            condicao = 1;
        } else {
            condicao = 0;
        }
            
        if(condicao)
        {
            if((matriz_resposta[linha_ap-1][coluna_ap-1] == '0') && (matriz[linha_ap-1][coluna_ap-1] != ' '))
            {

                int soma_linha; soma_linha = 0;
                int soma_coluna; soma_coluna = 0;
                matriz[linha_ap-1][coluna_ap-1] = ' '; ap_atual++;

                for(int i=0; i<N; i++)
                {
                    if(matriz[linha_ap-1][i] != ' ')
                    {
                        soma_linha += (matriz[linha_ap-1][i] - '0');
                    }
                        
                    if(matriz[i][coluna_ap-1] != ' ')
                    {
                        soma_coluna += (matriz[i][coluna_ap-1] - '0');
                    }
                        
                }

                if(soma_linha == vetor_linha[linha_ap-1])
                {
                    printf("VOCE CONCLUIU A LINHA %d!\n", linha_ap);
                    vetor_linha[linha_ap-1] = 0;
                }

                if(soma_coluna == vetor_coluna[coluna_ap-1])
                {
                    printf("VOCE CONCLUIU A COLUNA %d!\n", coluna_ap);
                    vetor_coluna[coluna_ap-1] = 0;
                }

                printf("MUITO BEM! TECLE <ENTER> PARA CONTINUAR"); getchar(); getchar();

            } else if(matriz[linha_ap-1][coluna_ap-1] == ' '){

                printf("VOCE JA APAGOU ESSE ELEMENTO, TECLE <ENTER> E TENTE NOVAMENTE"); getchar(); getchar();

            } else {

                printf("ERROU, TECLE <ENTER> E TENTE NOVAMENTE"); getchar(); getchar();
                vidas--;
            }
        } else {
            printf("VALOR INVALIDO! TECLE <ENTER> E TENTE NOVAMENTE"); getchar(); getchar();
        }
            system(CLEAR);
    } 

    printf("             ");
    for(int i=0; i<N; i++)
    {
        printf("%3d        ", vetor_coluna[i]);
    }
    printf("\n");

    for(int i=0; i<N; i++)
    {
        int j=0;
        printf("%3d     |   [%3c ]     [%3c ]     [%3c ]     [%3c ]", vetor_linha[i], matriz[i][j], matriz[i][j+1], matriz[i][j+2], matriz[i][j+3]);
        if(N == 6) 
        {
            printf("     [%3c ]     [%3c ]\n", matriz[i][j+4], matriz[i][j+5]);
        } else if(N == 7) {
            printf("     [%3c ]     [%3c ]     [%3c ]\n" ,matriz[i][j+4], matriz[i][j+5], matriz[i][j+6]);
        } else {
            printf("\n");
        }
    }

    if(!vidas)                                                              // verifica se o jogador venceu ou perdeu, se perdeu ele volta o offseat pro inicio da matriz(marcado pela * , caso não seja a primeira matriz do arquivo)
    {

        int pos_atual = ftell(fd);

        while (pos_atual > 0)  
        {
            pos_atual--;
            fseek(fd, pos_atual, SEEK_SET);  
            char caractere = fgetc(fd);

            if (caractere == '*')  
            {
                pos_atual+=2;
                fseek(fd, pos_atual, SEEK_SET);
                break;
            }
        }

        if(nivel == 1)  
        {
            nivel_anterior = 1;
            ponteiro_nivel_1 = fd;
        } else if(nivel == 2) {
            nivel_anterior = 2;
            ponteiro_nivel_2 = fd;
        } else {
            nivel_anterior = 3;
            ponteiro_nivel_3 = fd;
        }


        printf("VOCE PERDEU O JOGO! :(\n");
        printf("TECLE <ENTER> PARA VOLTAR AO MENU PRINCIPAL!\n"); getchar(); 

    } else {                                                                // se venceu atualiza no ranking a pontuação e pula pra próxima fase

        if(nivel == 1)
        {
            jogador.pontuacao += 50;
            nivel_anterior = 1;
        } else if(nivel == 2) {
            jogador.pontuacao += 100;
            nivel_anterior = 2;
        } else {
            jogador.pontuacao += 200;
            nivel_anterior = 3;
        }

        atualizar_ranking(); 
        
        printf("PARABENS! VOCE COMPLETOU O NIVEL! :)\n");
        printf("TECLE <ENTER> PARA VOLTAR AO MENU PRINCIPAL!\n"); getchar(); 
    }
    system(CLEAR);

}

// função de configurar, a qual é chamada casdo o jogador selecione a opção 2 no menu

void configuracoes()
{
    printf("+++ CONFIGURACOES +++\n");
    printf("\n1 - Zerar ranking\n");
    printf("2 - Modo de Dificuldade\n");
    printf("3 - Voltar ao menu principal\n");

    int opcao; scanf("%d", &opcao);                                         // recebe a opção do jogador e, dependendo dela, faz alguma função
    switch(opcao)
    {
        case 1:                                                             // se for 1, reinicializa o ranking(abre com a opção "wb")
            getchar();
            printf("\nConfirma a reinicializacao do ranking? (S/N)\n");
            char opcao2; scanf("%c", &opcao2);
            if(opcao2 == 'S' || opcao2 == 's')
            {
                FILE *fd = fopen(file_ranking, "wb"); fclose(fd);
                printf("RANK REINICALIZADO COM SUCESSO\n"); getchar(); getchar();
            } 
            system(CLEAR);
            configuracoes();
            break;
        case 2:                                                             // se for 2, altera o nível da fase
            printf("\nEscolhe um nivel de dificuldade:\n"); 
            printf("\n1: FACIL\n");
            printf("\n2: MEDIO\n");
            printf("\n3: DIFICIL\n");
            int opcao3; scanf("%d", &opcao3);
            switch(opcao3)
            {
                case 1:
                    nivel_atual = 1;
                    break;
                case 2:
                    nivel_atual = 2;
                    break;
                case 3:
                    nivel_atual = 3;
                    break;
                default:
                    printf("\nDIGITE UM VALOR VALIDO!\n"); getchar(); getchar();
                    break;
            }
            printf("MUDANCA DE NIVEL EFETUADA\n");
            printf("TECLE <ENTER> PARA CONTINUAR\n"); getchar(); getchar();
            system(CLEAR);
            configuracoes();
            break;
        case 3:                                                             // se for 3, retorna para o menu principal
            system(CLEAR);
            break;
    }


}

// função de mostrar as instruções, a qual é chamada casdo o jogador selecione a opção 3 no menu

void instrucoes()
{
    printf("+++ INSTRUCOES +++\n");
    printf("\nNUMBER SUMS E UM JOGO QUE CONSISTE EM TRES REGRAS SIMPLES:\n");
    printf("1: TODA RODADA VOCE ESCOLHE UMA COLUNA E UMA LINHA PARA ELIMINAR UM NUMERO\n"); 
    printf("2: OS NUMEROS QUE SOBRAREM NA LINHA/COLUNA DEVEM CORRESPONDER COM A SOMA RESPECTIVA DA LINHA/COLUNA, SE NAO VOCE PERDE UMA VIDA\n");
    printf("3: VOCE TEM 5 VIDAS, OU SEJA, CASO VOCE ERRE 5 VEZES VOCE PERDE O JOGO\n");
    printf("O OBJETIVO E QUE SOBRE APENAS OS NUMEROS QUE SOMEM A SOMA DE CADA LINHA E COLUNA\n\n");
    printf("O JOGO POSSUI 3 NIVEIS: FACIl, MEDIO E DIFICIL, CADA FASE VALENDO 50, 100 e 200 PONTOS, RESPECTIVAMENTE\n");
    printf("O JOGO ACABA QUANDO O JOGADOR TERMINAR A ULTIMA FASE DO NIVEL DIFICIL\n");
    printf("\nTECLE <ENTER> PARA VOLTAR AO MENU PRINCIPAL");
    getchar(); getchar();
    system(CLEAR); 

}

// função de mostrar o ranking, a qual é chamada casdo o jogador selecione a opção 4 no menu

void mostrar_ranking()
{
    FILE *fd;
    fd = fopen("ranking.bin", "rb");                                        // abre o arquivo para leitura

    int total_jogadores = 0;

    if(fd != NULL)                                                          // se já existir o arquivo, ordena os jogadores e depois printa cada um
    {

        dados_jogador jogadores[1000]; int index = 0;
        dados_jogador jogador_temporario; 
        while(fread(&jogador_temporario, sizeof(dados_jogador), 1, fd) == 1)
        {
            jogadores[index] = jogador_temporario;
            index++;
        }

        for(int i=1; i<index; i++)
        {
            int i_atual = i;
            while((i_atual > 0) && (jogadores[i_atual].pontuacao > jogadores[i_atual-1].pontuacao))
            {
                dados_jogador temp = jogadores[i_atual];
                jogadores[i_atual] = jogadores[i_atual-1];
                jogadores[i_atual-1] = temp;
                i_atual--;
            }
        }

        for(int i=0; i<index; i++)
        {
            printf("jogador : %s pontuacao : %d\n",jogadores[i].nickname, jogadores[i].pontuacao);
            total_jogadores++;
        }
    }
    
    if(total_jogadores == 0)                                                // se não existir, ou seja, o total de jogadores é 0, então ele avisa que não tem ninguém no ranking
    {
        printf("NAO TEM NINGUEM NO RANKING :(\n");
    }

    fclose(fd);
    printf("TECLE <ENTER> PARA VOLTAR AO MENU PRINCIPAL\n");
    getchar(); getchar();
    system(CLEAR); 

}

//função main, na qual roda o loop principal do jogo

int main()
{

    jogador.pontuacao = 0;                                                  //inicialização dos ponteiros globais
    ponteiro_nivel_1 = fopen("iniciante.txt", "r+");
    ponteiro_nivel_2 = fopen("intermediario.txt", "r+");
    ponteiro_nivel_3 = fopen("avancado.txt", "r+");
    ponteiro_ranking = fopen("ranking.bin", "a+b");

    printf("+++ JOGO DAS SOMAS +++\n");
    printf("\nBem vindo(a) ao jogo das somas +\n");
    printf("\nInforme seu nickname: "); scanf("%s", jogador.nickname);
    system(CLEAR);

    while(1)                                                                // chama a função menu principal e o retorno dela é guardado na variável escolha
    {
        escolha = menu_principal();

        switch(escolha)                                                     // dependendo da escolha, ele chama a função jogar, configuracoes, instrucoes, mostrar ranking ou sai do jogo                                                                    
        {
            case 1:
                jogar();
                break;
            case 2:
                configuracoes();
                break;
            case 3:
                instrucoes();
                break;
            case 4:
                mostrar_ranking();
                break;
            case 5:
                break;
        }

        if(escolha == 5)                                                    // se a opção for 5(sair do jogo), fecha os arquivos e quebra o loop
        {
            fclose(ponteiro_nivel_1);
            fclose(ponteiro_nivel_2);
            fclose(ponteiro_nivel_3);
            fclose(ponteiro_ranking);
            break;
        }

    }

    return 0;

}