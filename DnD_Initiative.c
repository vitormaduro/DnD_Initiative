#include "stdio.h"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>
#include "time.h"
#define KRED "\x1B[31m"      // Vermelho
#define KGRN "\x1B[32m"      // Verde
#define KYEL "\x1B[33m"      // Amarelo
#define KBLU "\x1B[34m"      // Azul
#define KMAG "\x1B[35m"      // Magenta
#define KCYN "\x1B[36m"      // Ciano
#define KWHT "\x1B[37m"      // Branco
#define RESET "\x1B[0m"      // Reset

char nome[40][40];
char anotacoes[40][100];
int inic[40];
int hp[40];
int hpMax[40];
int turnCount = 1;
int qntd;
int novosComb = 1;
int x;
int turno = -1; 

void printGraph() {
    printf("\033[2J\033[1;1H");
    printf("----------------------------------------------------\n");
    printf("   Turno %d\n\n", turnCount);
	for(x = 0; x < qntd - 1 + novosComb; x++) {
		if(x == turno)
			printf(KYEL"[%d]"RESET" - ", x+1);
		else
			printf("[%d] - ", x+1);

        if(hp[x] != 0) {
	        printf("%s, ", &nome[x][0]);
            printf(KGRN"Inic"RESET": %d, ", inic[x]);
            printf(KRED"HP"RESET": %d/%d  ", hp[x], hpMax[x]);

            if(strlen(&anotacoes[x][0]) != 0)
                printf("- "KCYN"%s"RESET, &anotacoes[x][0]);

            printf("\n");
   
        }
        else {
            printf(KRED"%s, ", &nome[x][0]);
            printf("Inic: %d, ", inic[x]);
            printf("HP: %d/%d  "RESET, hp[x], hpMax[x]);

            if(strlen(&anotacoes[x][0]) != 0)
                printf("- "KRED"%s"RESET, &anotacoes[x][0]);

            printf("\n");
        }
	}
    printf("----------------------------------------------------\n");
	printf("\n");
}

void ordena() {
    int h;
    int controle = 1;
    int inicTroca;
    int hpTroca;
    int hpMaxTroca;
    char nomeTroca[40];

    while(controle > 0) {
        controle = 0;
        for(x = 0; x < qntd - 1 + novosComb; x++) {
        	if(inic[x] < inic[x+1] && x < qntd - 2 + novosComb) {
			    controle++;
			    inicTroca = inic[x+1];
			    inic[x+1] = inic[x];
			    inic[x] = inicTroca;

                hpTroca = hp[x+1];
                hp[x+1] = hp[x];
                hp[x] = hpTroca;

                hpMaxTroca = hpMax[x+1];
                hpMax[x+1] = hpMax[x];
                hpMax[x] = hpMaxTroca;

			    for(h = 0; h < 40; h++)
				    nomeTroca[h] = nome[x + 1][h];

			    for(h = 0; h < 40; h++)
				    nome[x+1][h] = nome[x][h];

			    for(h = 0; h < 40; h++)
				    nome[x][h] = nomeTroca[h];
		    }
	    }
    }
}

int jogaDados(int a, int b) {
	int resultado = 0;
	int dado;
	int x;
	
	for(x = 0; x < a; x++) {
		dado = (rand() % b) + 1;

		resultado += dado;
	}		

	return resultado;
}

int defineHP(char diceHP[], int current) {
    int hpValue = 0;
    int dice1 = 0;
    int dice2 = 0;
    int soma = 0;
    int loop;
    int loopReserva = 0;
    char numero1[5];
    char numero2[5];
    char numero3[5];

    for(loop = 0; loop < 5; loop++) {
        numero1[loop] = 'k';
        numero2[loop] = 'k';
        numero3[loop] = 'k';
    }

    for(loop = 0; loop < 10; loop++) {
        if(diceHP[loop] == 'd' || diceHP[loop] == 'D')
            break;
        else {
            numero1[loopReserva] = diceHP[loop];
            loopReserva++;
        }
    }

    loopReserva = 0;
    for(loop = loop + 1; loop < 10; loop++) {
        if(diceHP[loop] == '+')
            break;
        else {
            numero2[loopReserva] = diceHP[loop];
            loopReserva++;
        }
    }

    loopReserva = 0;
    for(loop = loop + 1; loop < 10; loop++) {
        numero3[loopReserva] = diceHP[loop];
        loopReserva++;
    }

    dice1 = atoi(&numero1[0]);
    dice2 = atoi(&numero2[0]);
    soma = atoi(&numero3[0]);

    hpValue = jogaDados(dice1, dice2);
    hpMax[current] = hpValue + soma;

    return hpValue + soma;
}

void Save(void) {
    int x;
    char fileName[30];
    FILE *pFile;

    printf("Digite o nome do arquivo a ser criado: ");
    scanf("%s", fileName);

    pFile = fopen(fileName, "w");

    if(pFile == NULL) {
        printf("Não foi possível criar o arquivo\n");
        return;
    }

    fprintf(pFile, "%d\n", turno);
    fprintf(pFile, "%d\n", turnCount);
    fprintf(pFile, "%d\n", qntd + novosComb - 1);  

    for(x = 0; x < qntd + novosComb - 1; x++) {
        fprintf(pFile, "%s\n", &nome[x][0]);
        fprintf(pFile, "%d\n", inic[x]);
        fprintf(pFile, "%d\n", hp[x]);
        fprintf(pFile, "%d\n", hpMax[x]);
        
        if(strlen(&anotacoes[x][0]) == 0)
            fprintf(pFile, "***---***\n");
        else
            fprintf(pFile, "%s\n", &anotacoes[x][0]);
    }

    fclose(pFile);

    printf("\033[2J\033[1;1H");
    printf("Salvo com sucesso\n");

    getchar();
    if(getchar())
        printGraph();
}

int Load(void) {
    int x;
    char fileName[30];
    FILE *pFile;

    printf("Digite o nome do arquivo a ser carregado: ");
    scanf("%s", fileName);

    pFile = fopen(fileName, "r");
    qntd = 0;

    if(pFile == NULL) {
        printf("\033[2J\033[1;1H");
        printf("Não foi possível abrir o arquivo. Verifique se o nome está correto\n");
        getchar();
        getchar();
        return -1;
    }

    fscanf(pFile, "%d", &turno);
    fscanf(pFile, "%d", &turnCount);
    fscanf(pFile, "%d", &qntd);
    novosComb = 1;

    for(x = 0; x < qntd; x++) {
        fscanf(pFile, "%s\n", &nome[x][0]);
        fscanf(pFile, "%d\n", &inic[x]);
        fscanf(pFile, "%d\n", &hp[x]);
        fscanf(pFile, "%d\n", &hpMax[x]);
        fscanf(pFile, "%[^\n]%*c", &anotacoes[x][0]);

        if(strcmp(&anotacoes[x][0], "***---***") == 0)
            strcpy(&anotacoes[x][0], "");
    }

    fclose(pFile);

    printGraph();

    return 1;
}

int main(void) {
    int arrayLoop;
    int y;   
    int z;
    int dano;
    int newHP;
    int breakHP = 0;
    int bToRemove = 0;
    int dado1;
    int dado2;
    int dadoTotal = 0;
    int dadoSoma = 0;
    int escolheNumero;
    int turnDecided = 0;
    int stringPosToBreak = 0;
    int invalidHP = 0;
    char hpTest[10];
	char decisao = 'o';                                   //Usada para verificar o que o usuário quer fazer
    char nomeReserva[40];
    char dadoJogado[10];
    char stringTeste[20];


    srand(time(NULL));
    
	printf("Quantidade de criaturas (digite -1 para carregar batalha salva): ");
	scanf("%d", &qntd);

    if(qntd == -1) {
        int testeLoad = Load();

        while(testeLoad == -1)
            testeLoad = Load();
    }
    else {
        getchar();

	    for(x = 0; x < qntd; x++) {                            //Loop de entrada de nomes e iniciativas
            int whiteSpaces = 0;

            for(arrayLoop = 0; arrayLoop < 10; arrayLoop++)
                hpTest[arrayLoop] = 'k';

            breakHP = 0;
            invalidHP = 0;

		    printf("\nNome da criatura %d: ", x + 1);
            scanf("%[^\n]%*c", &nome[x][0]);
            printf("Iniciativa e HP de %s: ", &nome[x][0]);
            scanf("%[^\n]%*c", stringTeste);

            for(z = 0; z < strlen(stringTeste); z++) {
                if(stringTeste[z] == ' ') {
                    whiteSpaces++;
                    stringPosToBreak = z;
                }
            }

            inic[x] = atoi(&stringTeste[0]);

            for(z = 0; z < 10; z++) {
                hpTest[z] = stringTeste[stringPosToBreak + z + 1];

                if(hpTest[z] == '\0')
                    break;

                if(hpTest[z] != '1' && hpTest[z] != '2' && hpTest[z] != '3' && 
                    hpTest[z] != '4' && hpTest[z] != '5' && hpTest[z] != '6' && 
                    hpTest[z] != '7' && hpTest[z] != '8' && hpTest[z] != '9' && 
                    hpTest[z] != '0' && hpTest[z] != 'd' && hpTest[z] != '/' &&
                    hpTest[z] != '+')

                    invalidHP = 1;
            }

            while(whiteSpaces != 1 || inic[x] == 0 || invalidHP == 1) {
                invalidHP = 0;

                printf("--> Erro ao ler as informações. Digite-as novamente\n");
                printf("Iniciativa e HP de %s: ", &nome[x][0]);

                scanf("%[^\n]%*c", stringTeste);
                whiteSpaces = 0;

                for(z = 0; z < strlen(stringTeste); z++) {
                    if(stringTeste[z] == ' ')
                    {
                        whiteSpaces++;
                        stringPosToBreak = z;
                    }
                }

                inic[x] = atoi(&stringTeste[0]);

                for(z = 0; z < 10; z++)
                {
                    hpTest[z] = stringTeste[stringPosToBreak + z + 1];

                    if(hpTest[z] == '\0')
                        break;

                    if(hpTest[z] != '1' && hpTest[z] != '2' && hpTest[z] != '3' && 
                        hpTest[z] != '4' && hpTest[z] != '5' && hpTest[z] != '6' && 
                        hpTest[z] != '7' && hpTest[z] != '8' && hpTest[z] != '9' && 
                        hpTest[z] != '0' && hpTest[z] != 'd' && hpTest[z] != '/' &&
                        hpTest[z] != '+')

                        invalidHP = 1;
                }
            }
            
            for(y = 0; y < 10; y++) {
                if(hpTest[y] == 'd' || hpTest[y] == 'D') {
                    hp[x] = defineHP(&hpTest[0], x);
                    breakHP = 1;
                    break;
                }
            } 

            if(breakHP != 1) {
                hp[x] = atoi(&hpTest[0]);
                hpMax[x] = hp[x];

                for(y = 0; y < 10; y++) {
                    if(hpTest[y] == '/')
                        hpMax[x] = atoi(&hpTest[y+1]);
                }
            }
	    }

	    ordena();

	    printGraph();
	    printf("\n\n");
    }

 	while(decisao != 'p') {
		printf("O que deseja fazer agora?\n");
        printf("[a] - Criar uma anotação em um combatente\n");
        printf("[s] - Apagar a anotação de um combatente\n");
        printf("[z] - Passar o turno\n");
        printf("[x] - Causar dano/Curar\n");
        printf("[c] - Adicionar um novo combatente\n");
        printf("[v] - Remover um combatente\n");
        printf("[b] - Set HP\n");
        printf("[n] - Rolar dados\n");
        printf("[k] - Salvar batalha em um arquivo\n");
        printf("[p] - Sair\n");
		scanf("\n%c", &decisao);

		if(decisao == 'z') {           //Passa o turno para o próximo combatente
			printf("\033[2J\033[1;1H");
            turnDecided = 0;

            if(turno == qntd - 2 + novosComb) {
                if(hp[0] == 0) {
                    turno++;

                    while(hp[turno] == 0) {
                        turno++;

                        if(turno > qntd - 1 + novosComb) {
                            turno = 0;
                            turnCount++;
                        }
                    }

                    turnDecided = 1;
                }
            }
            else {
                if(hp[turno+1] == 0) {
                    turno++;

                    while(hp[turno] == 0) {
                        turno++;
                        
                        if(turno > qntd - 1 + novosComb) {
                            turno = 0;
                            turnCount++;
                        }
                    }

                    turnDecided = 1;
                }
            }

            if(turnDecided == 0) {
			    if(turno < qntd - 2 + novosComb)
				    turno++;
			    else {
				    turno = 0;
                    turnCount++;
                }
            }            

            printGraph();
		}

        if(decisao == 'x') {              //Causa dano
            printf("\nDigite o nome do combatente e o dano a ser causado (valores negativos irão curar): ");
            scanf("%s %d", &nomeReserva[0], &dano);

            for(x = 0; x < qntd + novosComb; x++) {
                if(strcmp(nomeReserva, nome[x]) == 0) {
                    hp[x] -= dano;
                    if(hp[x] > hpMax[x])
                        hp[x] = hpMax[x];
                    break;
                }
            }

            if(x == qntd + novosComb) {
                printf("\nNÃO ENCONTRADO. APERTE ENTER PARA CONTINUAR");
                getchar();
                while(getchar() != '\n');
            }

            if(hp[x] < 0)
                hp[x] = 0;

            printGraph();
        }

        if(decisao == 'a') {      // Escreve anotações
            printf("\nDigite o nome ou número do combatente: ");
            scanf("%s", &nomeReserva[0]);
            escolheNumero = atoi(&nomeReserva[0]);

            if(escolheNumero != 0) {
                getchar();

                strcpy(&anotacoes[escolheNumero-1][0], "");

                printf("Digite a anotação: ");
                scanf ("%[^\n]%*c", &anotacoes[escolheNumero - 1][0]);
                printGraph();
            }
            else {
                for(x = 0; x < qntd + novosComb; x++) {
                    if(strcmp(nomeReserva, nome[x]) == 0) {
                        getchar();
                    
                        strcpy(&anotacoes[x][0], "");

                        printf("Digite a anotação: ");
                        scanf ("%[^\n]%*c", &anotacoes[x][0]);
                        printGraph();

                        break;
                    }
                }
            }
        }

        if(decisao == 's') {
            printf("\nDigite o nome ou número do combatente: ");
            scanf("%s", &nomeReserva[0]);
            escolheNumero = atoi(&nomeReserva[0]);

            if(escolheNumero != 0) {
                strcpy(&anotacoes[escolheNumero-1][0], "");

                printGraph();
            }
            else {
                for(x = 0; x < qntd + novosComb; x++) {
                    if(strcmp(nomeReserva, nome[x]) == 0) { 
                        strcpy(&anotacoes[x][0], "");

                        printGraph();

                        break;
                    }
                }
            }
        }

		if(decisao == 'c') {      //Adiciona um novo combatente
            printGraph();

			printf("\nDigite o nome, a iniciativa, e o HP do novo combatente: ");
			scanf("%s %d %s", &nome[qntd - 1 + novosComb][0], &inic[qntd - 1 + novosComb], &hpTest[0]);

            breakHP = 0;
            for(y = 0; y < 10; y++) {
                if(hpTest[y] == 'd' || hpTest[y] == 'D') {
                    hp[x] = defineHP(&hpTest[0], qntd + novosComb);
                    hpMax[x] = hp[x];
                    breakHP = 1;
                    break;
                }
            }

            if(breakHP == 0) {
                hp[x] = atoi(&hpTest[0]);
                hpMax[x] = hp[x];

                for(y = 0; y < 10; y++) {
                    if(hpTest[y] == '/')
                    hpMax[x] = atoi(&hpTest[y+1]);
                }
            }

			novosComb++;

			ordena();

            printGraph();
            decisao = 'o';
		}

        if(decisao == 'v') {      //Remove um combatente
            printf("\nDigite o nome ou número do combatente a ser removido: ");
            scanf("%s", &nomeReserva[0]);
            escolheNumero = atoi(&nomeReserva[0]);

            if(escolheNumero != 0) {
                bToRemove = 1;
                inic[escolheNumero-1] = -100;
            }
            else {
                for(x = 0; x < qntd + novosComb; x++) {
                    if(strcmp(nomeReserva, nome[x]) == 0) {
                        bToRemove = 1;
                        inic[x] = -100;
                        break;
                    }
                }
            }

            if(x == qntd + novosComb) {
                printf("\nNÃO ENCONTRADO. APERTE ENTER PARA CONTINUAR");
                getchar();
                while(getchar() != '\n');
            }

            printf("\033[2J\033[1;1H");

			ordena();

            if(bToRemove == 1)
                qntd--;

            printGraph();

            bToRemove = 0;
        }

        if(decisao == 'b') {              //Set HP
            printf("\nDigite o nome do combatente e o novo valor do HP atual: ");
            scanf("%s %d", &nomeReserva[0], &newHP);

            for(x = 0; x < qntd + novosComb; x++) {
                if(strcmp(nomeReserva, nome[x]) == 0) {
                    if(newHP <= hpMax[x])
                        hp[x] = newHP;

                    if(newHP < 0)
                        hp[x] = 0;

                    break;
                }
            }

            if(x == qntd + novosComb) {
                printf("\nNÃO ENCONTRADO. APERTE ENTER PARA CONTINUAR");
                getchar();
                while(getchar() != '\n');
            }

            printGraph();
        }

        if(decisao == 'n') {         //Rola dados     
            for(x = 0; x < 10; x++)
                dadoJogado[x] = ' ';

            printGraph();
            dadoTotal = 0;
            dadoSoma = 0;
            printf("\nDigite os dados a serem jogados: ");
            scanf("%s", dadoJogado);

            dado1 = atoi(&dadoJogado[0]);

            for(x = 0; x < 10; x++) {
                if(dadoJogado[x] == 'd')
                    dado2 = atoi(&dadoJogado[x+1]);

                if(dadoJogado[x] == '+')
                    dadoSoma = atoi(&dadoJogado[x+1]);
            }

            printf("\nNúmeros rolados (%dd%d): ", dado1, dado2);

            for(x = 0; x < dado1; x++) {
                y = jogaDados(1,dado2);
                dadoTotal += y;
                printf("%d ", y);
            }

            if(dadoSoma != 0)
                printf("\nSoma total: %d + %d = %d\n\n", dadoTotal, dadoSoma, dadoTotal+dadoSoma);
            else
                printf("\nSoma total: %d\n\n", dadoTotal);
        }

        if(decisao == 'k')
            Save();

		if(decisao == 'p')
			return 1;
	}	

	return 0;
}