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

typedef struct HPstruct {
    int currentHP;
    int maxHP;
} HP;

typedef struct CreatureStruct {
    char nome[40];
    char anotacoes[100];
    int inic;
    int hp;
    int hpMax;
} Creature;

int turnCount = 1;
int qntd;
int novosComb = 1;
int x;
int turno = -1;
Creature creatures[40];

void PrintGraph() {
    printf("\033[2J\033[1;1H");
    printf("----------------------------------------------------\n");
    printf("   Turno %d\n\n", turnCount);
	for(x = 0; x < qntd - 1 + novosComb; x++) {
		if(x == turno)
			printf(KYEL"[%d]"RESET" - ", x+1);
		else
			printf("[%d] - ", x+1);

        if(creatures[x].hp != 0) {
	        printf("%s, ", &creatures[x].nome[0]);
            printf(KGRN"Inic"RESET": %d, ", creatures[x].inic);
            printf(KRED"HP"RESET": %d/%d  ", creatures[x].hp, creatures[x].hpMax);

            if(strlen(creatures[x].anotacoes) != 0)
                printf("- "KCYN"%s"RESET, &creatures[x].anotacoes[0]);

            printf("\n");
   
        }
        else {
            printf(KRED"%s, ", &creatures[x].nome[0]);
            printf("Inic: %d, ", creatures[x].inic);
            printf("HP: %d/%d  "RESET, creatures[x].hp, creatures[x].hpMax);

            if(strlen(creatures[x].anotacoes) != 0)
                printf("- "KRED"%s"RESET, &creatures[x].anotacoes[0]);

            printf("\n");
        }
	}
    printf("----------------------------------------------------\n");
	printf("\n");
}

void BubbleSortCreatures() {
    int controle = 1;
    int inicTroca;
    int hpTroca;
    int hpMaxTroca;
    char nomeTroca[40];

    while(controle > 0) {
        controle = 0;
        for(x = 0; x < qntd - 1 + novosComb; x++) {
        	if(creatures[x].inic < creatures[x+1].inic && x < qntd - 2 + novosComb) {
			    controle++;
			    inicTroca = creatures[x+1].inic;
			    creatures[x+1].inic = creatures[x].inic;
			    creatures[x].inic = inicTroca;

                hpTroca = creatures[x+1].hp;
                creatures[x+1].hp = creatures[x].hp;
                creatures[x].hp = hpTroca;

                hpMaxTroca = creatures[x+1].hpMax;
                creatures[x+1].hpMax = creatures[x].hpMax;
                creatures[x].hpMax = hpMaxTroca;

                strcpy(nomeTroca, creatures[x+1].nome);
                strcpy(creatures[x+1].nome, creatures[x].nome);
                strcpy(creatures[x].nome, nomeTroca);
		    }
	    }
    }
}

int RollDice(int a, int b) {
	int resultado = 0;
	int dado;
	int x;
	
	for(x = 0; x < a; x++) {
		dado = (rand() % b) + 1;
		resultado += dado;
	}		

	return resultado;
}

int DiceExpressionToHp(char diceHP[], int current) {
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

    hpValue = RollDice(dice1, dice2);

    return hpValue + soma;
}

void Save(void) {
    int x;
    FILE *pFile;

    pFile = fopen("InitiativeSave.txt", "w");

    if(pFile == NULL) {
        printf("Não foi possível criar o arquivo para salvar\n");
        return;
    }

    fprintf(pFile, "%d\n", turno);
    fprintf(pFile, "%d\n", turnCount);
    fprintf(pFile, "%d\n", qntd + novosComb - 1);  

    for(x = 0; x < qntd + novosComb - 1; x++) {
        fprintf(pFile, "%s\n", &creatures[x].nome[0]);
        fprintf(pFile, "%d\n", creatures[x].inic);
        fprintf(pFile, "%d\n", creatures[x].hp);
        fprintf(pFile, "%d\n", creatures[x].hpMax);
        
        if(strlen(&creatures[x].anotacoes[0]) == 0)
            fprintf(pFile, "***---***\n");
        else
            fprintf(pFile, "%s\n", &creatures[x].anotacoes[0]);
    }

    fclose(pFile);
}

int Load(char fileToLoad[]) {
    int x;
    char fileName[30];
    FILE *pFile;

    strcpy(fileName, fileToLoad);

    pFile = fopen(fileName, "r");
    qntd = 0;

    if(pFile == NULL) {
        return -1;
    }

    fscanf(pFile, "%d", &turno);
    fscanf(pFile, "%d", &turnCount);
    fscanf(pFile, "%d", &qntd);
    novosComb = 1;

    for(x = 0; x < qntd; x++) {
        fscanf(pFile, "%s\n", &creatures[x].nome[0]);
        fscanf(pFile, "%d\n", &creatures[x].inic);
        fscanf(pFile, "%d\n", &creatures[x].hp);
        fscanf(pFile, "%d\n", &creatures[x].hpMax);
        fscanf(pFile, "%[^\n]%*c", &creatures[x].anotacoes[0]);

        if(strcmp(&creatures[x].anotacoes[0], "***---***") == 0)
            strcpy(&creatures[x].anotacoes[0], "");
    }

    fclose(pFile);

    PrintGraph();

    return 1;
}

int IsHpValid(char stringHP[]) {
    int z;

    for(z = 0; z < strlen(stringHP); z++) {
        if(stringHP[z] != '1' && stringHP[z] != '2' && stringHP[z] != '3' && 
            stringHP[z] != '4' && stringHP[z] != '5' && stringHP[z] != '6' && 
            stringHP[z] != '7' && stringHP[z] != '8' && stringHP[z] != '9' && 
            stringHP[z] != '0' && stringHP[z] != 'd' && stringHP[z] != '/' &&
            stringHP[z] != '+') {
                return 0;
            }
    }

    return 1;
}

int SetInitiative(char stringInit[]) {
    int init = 0;

    if(stringInit[0] == '+' && isdigit(stringInit[1]) != 0) {
        return RollDice(1,20) + atoi(&stringInit[1]);
    } else if(stringInit[0] == '-' && isdigit(stringInit[1]) != 0) {
        init = RollDice(1,20) - atoi(&stringInit[1]);

        if(init < 1)
            return 1;
        else
            return init;
    } else if(atoi(&stringInit[0]) != 0) {
        return atoi(&stringInit[0]);
    } else {
        printf(KRED"--> ERRO AO LER A INICIATIVA\n"RESET);
        return -1;
    }
}

HP SetHP(char stringHP[]) {
    int y;
    int diceReturn;
    int breakHP = 0;
    char hpTest[10];
    HP hpToReturn;

    hpToReturn.currentHP = -1;
    hpToReturn.maxHP = -1;

    if(IsHpValid(&stringHP[0]) == 0) {
        printf(KRED"--> ERRO. FORMATO DE HP INVÁLIDO\n"RESET);
        return hpToReturn;
    }

    for(y = 0; y < strlen(stringHP); y++) {
        if(stringHP[y] == 'd' || stringHP[y] == 'D') {
            diceReturn = DiceExpressionToHp(&stringHP[0], x);
            hpToReturn.currentHP = diceReturn;
            hpToReturn.maxHP = diceReturn;

            return hpToReturn;
        } else if(stringHP[y] == '/') {
            breakHP = 1;
            break;
        } else {
            hpTest[y] = stringHP[y];
        }
    }

    if(breakHP == 1) {
        hpToReturn.currentHP = atoi(&hpTest[0]);
        hpToReturn.maxHP = atoi(&stringHP[y + 1]);

        return hpToReturn;
    } else {
        hpToReturn.currentHP = atoi(&hpTest[0]);
        hpToReturn.maxHP = hpToReturn.currentHP;

        return hpToReturn;
    }

}

int AddCreature(int numberOfCreatures, char creatureName[], int current) {
    int addedCreatures = 0;
    int y;
    int z;
    char testString[40];
    char intToString[5];
    HP hpToCopy;

    if(numberOfCreatures > 1) {
        printf(KYEL"Adicionando %d criaturas\n"RESET, numberOfCreatures);
        printf("Digite o nome das criaturas: ");
        scanf("%[^\n]%*c", testString);

        z = 1;

        for(y = x; y < numberOfCreatures + x; y++) {
            sprintf(intToString, "_%d", z);
            z++;

            strcpy(&creatures[y].nome[0], testString);
            strcat(&creatures[y].nome[0], intToString);
        }

        printf("Digite a iniciativa das criaturas: ");
        scanf("%[^\n]%*c", &testString[0]);

        for(y = current; y < numberOfCreatures + current; y++) {
            creatures[y].inic = SetInitiative(&testString[0]);

            while(creatures[y].inic == -1) {      // Erro ao ler a iniciativa
                printf("Digite a iniciativa das criaturas: ");
                scanf("%[^\n]%*c", testString);
                creatures[y].inic = SetInitiative(&testString[0]);
            }
        }

        printf("Digite o HP das criaturas: ");
        scanf("%[^\n]%*c", testString);

        for(y = x; y < numberOfCreatures + x; y++) {
            hpToCopy = SetHP(&testString[0]);

            while(hpToCopy.maxHP == -1) {
                printf("HP das criaturas: ");
                scanf("%[^\n]%*c", testString);
                hpToCopy = SetHP(&testString[0]);
            }

            creatures[y].hp = hpToCopy.currentHP;
            creatures[y].hpMax = hpToCopy.maxHP;

            addedCreatures++;
        }

        return addedCreatures;
    } else {
        strcpy(&creatures[x].nome[0], &creatureName[0]);

        printf("Iniciativa de %s: ", &creatures[x].nome[0]);
        scanf("%[^\n]%*c", testString);

        creatures[x].inic = SetInitiative(&testString[0]);

        while(creatures[x].inic == -1) {      // Erro ao ler a iniciativa
            printf("Iniciativa de %s: ", &creatures[x].nome[0]);
            scanf("%[^\n]%*c", testString);
            creatures[x].inic = SetInitiative(&testString[0]);
        }

        printf("HP de %s: ", &creatures[x].nome[0]);
        scanf("%[^\n]%*c", testString);

        hpToCopy = SetHP(testString);

        while(hpToCopy.maxHP == -1) {
            printf("HP de %s: ", &creatures[x].nome[0]);
            scanf("%[^\n]%*c", testString);
            hpToCopy = SetHP(testString);
        }

        creatures[x].hp = hpToCopy.currentHP;
        creatures[x].hpMax = hpToCopy.maxHP;

        addedCreatures++;

        return addedCreatures;
    }
}

int main(int argc, char *argv[]) {
    int arrayLoop;
    int y;
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
    int creaturesToAdd = 1;
    char hpTest[10];
	char decisao = 'o';         //Usada para verificar o que o usuário quer fazer
    char nomeReserva[40];
    char dadoJogado[10];
    char stringTeste[20];

    srand(time(NULL));

    if(argc > 1) {
        if(strcmp(argv[1], "-l") == 0) {
            int testeLoad = Load("InitiativeSave.txt");

            if(testeLoad == -1) {
                printf("Erro ao carregar o arquivo\n");
                return 0;
            }
        }
    } else {
        printf("Quantidade de criaturas: ");
        scanf("%d", &qntd);

        getchar();

        for(x = 0; x < qntd; x++) {                            //Loop de entrada de nomes e iniciativas

            for(arrayLoop = 0; arrayLoop < 10; arrayLoop++)
                hpTest[arrayLoop] = 'k';

            breakHP = 0;

            printf("\nNome ou quantidade da criatura %d: ", x + 1);
            scanf("%[^\n]%*c", stringTeste);

            creaturesToAdd = atoi(&stringTeste[0]);

            if(creaturesToAdd != 0 && creaturesToAdd <= qntd - x) {
                AddCreature(creaturesToAdd, "", x);

                x += creaturesToAdd - 1;

            } else if(creaturesToAdd > qntd - x) {
                printf(KRED"--> NÚMERO DIGITADO MAIOR DO QUE O TOTAL DE CRIATURAS RESTANTES\n"RESET);
                x--;
            } else {
                AddCreature(1, stringTeste, x);
            }
        }
    }

    BubbleSortCreatures();

    PrintGraph();
    printf("\n\n");

 	while(1) {
        printf("[a] - Criar uma anotação\t\t[s] - Apagar anotações\n");
        printf("[c] - Adicionar nova(s) criatura(s)\t[v] - Remover uma criatura\n");
        printf("[z] - Passar o turno\t\t\t[x] - Causar dano/Curar\n");
        printf("[b] - Set HP\t\t\t\t[d] - Rolar dados\n");
		scanf("\n%c", &decisao);

        /* PASSAO TURNO */
		if(decisao == 'z') {
			printf("\033[2J\033[1;1H");
            turnDecided = 0;

            if(turno == qntd - 2 + novosComb) {
                if(creatures[0].hp == 0) {
                    turno++;

                    while(creatures[turno].hp == 0) {
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
                if(creatures[turno+1].hp == 0) {
                    turno++;

                    while(creatures[turno].hp == 0) {
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

            PrintGraph();
		}

        /* CAUSA DANO */
        if(decisao == 'x') {
            printf("\nDigite o nome do combatente e o dano a ser causado (valores negativos irão curar): ");
            scanf("%s %d", &nomeReserva[0], &dano);

            for(x = 0; x < qntd + novosComb; x++) {
                if(strcmp(nomeReserva, &creatures[x].nome[0]) == 0) {
                    creatures[x].hp -= dano;
                    if(creatures[x].hp > creatures[x].hpMax)
                        creatures[x].hp = creatures[x].hpMax;
                    break;
                }
            }

            if(x == qntd + novosComb) {
                printf("\nNÃO ENCONTRADO. APERTE ENTER PARA CONTINUAR");
                getchar();
                while(getchar() != '\n');
            }

            if(creatures[x].hp < 0)
                creatures[x].hp = 0;

            PrintGraph();
        }

        /* ESCREVE ANOTAÇÕES */
        if(decisao == 'a') {
            printf("\nDigite o nome ou número da criatura: ");
            scanf("%s", &nomeReserva[0]);
            escolheNumero = atoi(&nomeReserva[0]);

            if(escolheNumero != 0) {
                getchar();

                strcpy(&creatures[escolheNumero-1].anotacoes[0], "");

                printf("Digite a anotação: ");
                scanf ("%[^\n]%*c", &creatures[escolheNumero-1].anotacoes[0]);
                PrintGraph();
            }
            else {
                for(x = 0; x < qntd + novosComb; x++) {
                    if(strcmp(nomeReserva, &creatures[x].nome[0]) == 0) {
                        getchar();
                    
                        strcpy(&creatures[x].anotacoes[0], "");

                        printf("Digite a anotação: ");
                        scanf ("%[^\n]%*c", &creatures[x].anotacoes[0]);
                        PrintGraph();

                        break;
                    }
                }
            }
        }

        /* APAGA ANOTAÇÃO */
        if(decisao == 's') {
            printf("\nDigite o nome ou número da criatura: ");
            scanf("%s", &nomeReserva[0]);
            escolheNumero = atoi(&nomeReserva[0]);

            if(escolheNumero != 0) {
                strcpy(&creatures[escolheNumero-1].anotacoes[0], "");

                PrintGraph();
            }
            else {
                for(x = 0; x < qntd + novosComb; x++) {
                    if(strcmp(nomeReserva, &creatures[x].nome[0]) == 0) { 
                        strcpy(&creatures[x].anotacoes[0], "");

                        PrintGraph();

                        break;
                    }
                }

                if(x == qntd + novosComb) {
                    printf("\nNÃO ENCONTRADO. APERTE ENTER PARA CONTINUAR");
                    getchar();
                    while(getchar() != '\n');
                }
            }
        }

        /* ADICIONA CRIATURAS */
		if(decisao == 'c') {
            PrintGraph();

            printf("Digite o nome ou quantidade da nova criatura: ");
            scanf("%[^\n]%*c", &stringTeste[0]);

            creaturesToAdd = atoi(&stringTeste[0]);

            if(creaturesToAdd != 0 && creaturesToAdd <= qntd - x) {
                AddCreature(creaturesToAdd, "", x);

                x += creaturesToAdd - 1;
            } else if(creaturesToAdd > qntd - x) {
                printf(KRED"--> NÚMERO DIGITADO MAIOR DO QUE O TOTAL DE CRIATURAS RESTANTES\n"RESET);
                x--;
            } else {
                AddCreature(1, stringTeste, x);
            }

            breakHP = 0;
            for(y = 0; y < 10; y++) {
                if(hpTest[y] == 'd' || hpTest[y] == 'D') {
                    creatures[x].hp = DiceExpressionToHp(&hpTest[0], qntd + novosComb);
                    creatures[x].hpMax = creatures[x].hp;
                    breakHP = 1;
                    break;
                }
            }

            if(breakHP == 0) {
                creatures[x].hp = atoi(&hpTest[0]);
                creatures[x].hpMax = creatures[x].hp;

                for(y = 0; y < 10; y++) {
                    if(hpTest[y] == '/')
                    creatures[x].hpMax = atoi(&hpTest[y+1]);
                }
            }

			novosComb++;

			BubbleSortCreatures();

            PrintGraph();
            decisao = 'o';
		}

        /* REMOVE CRIATURAS */
        if(decisao == 'v') {
            printf("\nDigite o nome ou número da criatura a ser removido: ");
            scanf("%s", &nomeReserva[0]);
            escolheNumero = atoi(&nomeReserva[0]);

            if(escolheNumero != 0) {
                bToRemove = 1;
                creatures[escolheNumero-1].inic = -100;
            }
            else {
                for(x = 0; x < qntd + novosComb; x++) {
                    if(strcmp(nomeReserva, &creatures[x].nome[0]) == 0) {
                        bToRemove = 1;
                        creatures[x].inic = -100;
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

			BubbleSortCreatures();

            if(bToRemove == 1)
                qntd--;

            PrintGraph();

            bToRemove = 0;
        }

        /* SET HP ATUAL */
        if(decisao == 'b') {
            printf("\nDigite o nome da criatura e o novo valor do HP atual: ");
            scanf("%s %d", &nomeReserva[0], &newHP);

            for(x = 0; x < qntd + novosComb; x++) {
                if(strcmp(nomeReserva, &creatures[x].nome[0]) == 0) {
                    if(newHP <= creatures[x].hpMax)
                        creatures[x].hp = newHP;

                    if(newHP < 0)
                        creatures[x].hp = 0;

                    break;
                }
            }

            if(x == qntd + novosComb) {
                printf("\nNÃO ENCONTRADO. APERTE ENTER PARA CONTINUAR");
                getchar();
                while(getchar() != '\n');
            }

            PrintGraph();
        }

        /* ROLA DADOS */
        if(decisao == 'd') {  
            for(x = 0; x < 10; x++)
                dadoJogado[x] = ' ';

            PrintGraph();
            dadoTotal = 0;
            dadoSoma = 0;
            printf("\nDigite os dados a serem jogados: ");
            scanf("%s", dadoJogado);

            dado1 = atoi(&dadoJogado[0]);

            for(x = 0; x < strlen(dadoJogado); x++) {
                if(dadoJogado[x] == 'd')
                    dado2 = atoi(&dadoJogado[x+1]);

                if(dadoJogado[x] == '+')
                    dadoSoma = atoi(&dadoJogado[x+1]);

                if(dadoJogado[x] == '-')
                    dadoSoma = atoi(&dadoJogado[x+1]) * -1;
            }

            printf("\nNúmeros rolados (%dd%d): ", dado1, dado2);

            for(x = 0; x < dado1; x++) {
                y = RollDice(1,dado2);
                dadoTotal += y;
                printf("%d ", y);
            }

            if(dadoSoma != 0)
                printf("\nSoma total: %d + %d = %d\n\n", dadoTotal, dadoSoma, dadoTotal+dadoSoma);
            else
                printf("\nSoma total: %d\n\n", dadoTotal);
        }

        Save();
	}	

	return 0;
}
