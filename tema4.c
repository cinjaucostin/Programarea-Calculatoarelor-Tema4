#include "utils.h"

void *(*Abilities[4])(void *x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch (playerRole)
	{
	case Rotator:
		str = strcpy(str, "Rotator");
		break;
	case Decoder:
		str = strcpy(str, "Decoder");
		break;
	case Invertor:
		str = strcpy(str, "Invertor");
		break;
	case Impostor:
		str = strcpy(str, "Impostor");
		break;
	}
	return str;
}

// Task 1
void *RotateMatrix(void *input)
{
	// TODO
	char *buffer, *sir;
	int n = *((int *)input);//castarea input-ului

	buffer=calloc(MAX_LEN_STR_ATR,sizeof(char));//alocare memorie pentru buffer
    sir=calloc(MAX_LEN_STR_OUT,sizeof(char));//alocare memorie pentru sir

    for(int i=1; i<=n; i++)
    {
        for(int j=n; j>=1; j--)
        {
            if(j==1 && i!=n)
            {
                sprintf(buffer,"%d%d\n",j,i);//in cazul in care j==1(adica ultima coloana din matrice) si nu ne aflam pe ultima linie printam \n
            }
            else if(j==1 && i==n)
            {
                sprintf(buffer,"%d%d",j,i);//in cazul in care ne aflam pe ultima coloana si pe ultima linie printam doar numarul, nu si \n
            }
            else
            {
                sprintf(buffer,"%d%d ",j,i);//daca niciunul dintre celelalte doua cazuri nu este prezent printam doar numarul si spatiul dupa acesta
            }
			strcat(sir,buffer);
        }
    }
	free(buffer);//eliberam memoria ocupata de buffer
	return sir;
}
// Task 2
void *DecodeString(void *input)
{
	// TODO
	int decodificare = 0;
	char *sir, *token, delimitators[2] = "_", *buffer;
	sir = (char *)input;//castam inputul la un sir de caractere
	buffer = calloc(10 , sizeof(int));//alocam memoria pentru buffer
	token = strtok(sir, delimitators);
	while (token != NULL)
	{
		decodificare += atoi(token);
		token = strtok(NULL, delimitators);
	}
	sprintf(buffer, "%d", decodificare);
	return buffer;
}

//Task 3
void *InvertArray(void *input)
{
	// TODO
	int *p, lungime;
	char *buffer, *sir_output;
	p = (int *)input;//castam inputul la un vector de intregi
	lungime = *p;//prima valoare din input
	buffer = calloc(10 , sizeof(char));//alocam memorie pentru buffer
	sir_output = calloc(MAX_LEN_STR_ATR , sizeof(int));//alocam memorie pentru pointerul pe care il vom returna
	if (lungime % 2 == 0)//cazul in care primul numar din input este par
	{
		for (int i = 1; i <= lungime; i += 2)//schimbam elementele de pe pozitiile 2k+1 si 2k+2
		{
			int aux = *(p + i);
			*(p + i) = *(p + i + 1);
			*(p + i + 1) = aux;
		}
		for (int i = 1; i <= lungime; i++)
		{
			sprintf(buffer, "%d ", *(p + i));
			strcat(sir_output, buffer);
		}
	}
	else if (lungime % 2 == 1)//cazul in care primul numar din input este impar
	{
		for (int i = lungime; i >= 1; i--)//inversam ordinea elementelor(le-am introdus in ordinea inversa din input in output)
		{
			sprintf(buffer, "%d ", *(p + i));
			strcat(sir_output, buffer);
		}
	}
	free(buffer);//eliberam memoria ocupata de buffer
	return sir_output;
}

//Task 4
Player *allocPlayer()
{
	// TODO
	Player *jucator;
	jucator = malloc(sizeof(*jucator));
	jucator->name = malloc(MAX_LEN_STR_ATR * sizeof(char));
	jucator->color = malloc(MAX_LEN_STR_ATR * sizeof(char));
	jucator->hat = malloc(MAX_LEN_STR_ATR * sizeof(char));
	jucator->alive = 1;
	jucator->indexOfLocation = 0;
	return jucator;
}

//Task 4
Game *allocGame()
{
	// TODO
	Game *joc;
	joc = malloc(sizeof(*joc));
	joc->name = malloc(MAX_LEN_STR_ATR * sizeof(char));
	return joc;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	// TODO
	int *coordonate;
	char *roluri, rotator[8] = "Rotator", decoder[8] = "Decoder", invertor[9] = "Invertor", impostor[9] = "Impostor";
	Player *jucator;
	jucator = allocPlayer();
	fscanf(inputFile, "%s\n", jucator->name);
	fscanf(inputFile, "%s\n", jucator->color);
	fscanf(inputFile, "%s\n", jucator->hat);
	fscanf(inputFile, "%d\n", &jucator->numberOfLocations);
	jucator->locations = malloc(5 * jucator->numberOfLocations * sizeof(Location));
	coordonate = malloc(5 * jucator->numberOfLocations * sizeof(int));//alocarea memoriei pentru pointerul coordonate
	roluri = malloc(10 * sizeof(char));//alocam memoriei pentru pointerul roluri
	for (int i = 0; i < 2 * jucator->numberOfLocations; i += 2)//citim din fisier coordonatele din 2 in 2(pe pozitiile pare vom avea coordonata x iar pe pozitiile impare vom avea coordonata y)
	{
		fscanf(inputFile, "(%d,", coordonate + i);//citirea din fisier a x-ului
		fscanf(inputFile, "%d) ", coordonate + i + 1);//citirea din fisier a y-ului
	}
	for (int i = 0; i < 2 * jucator->numberOfLocations; i += 2)//introducem valorile din vectorul de coordonate in structura de locatii specifica jucatorului
	{
		jucator->locations[i / 2].x = *(coordonate + i);//introducerea coordonatei x
		jucator->locations[i / 2].y = *(coordonate + i + 1);//introducere coordonatei y
	}
	fscanf(inputFile, "%s", roluri);//citim din fisier rolul playerului
	if (strcmp(roluri, rotator) == 0)//daca rolul playerului este de rotator vom introduce in playerRole valoarea 0
		jucator->playerRole = 0;
	else if (strcmp(roluri, decoder) == 0)//daca rolul playerului este de decoder vom introduce in playerRole valoarea 1
		jucator->playerRole = 1;
	else if (strcmp(roluri, invertor) == 0)//daca rolul playerului este de invertor vom introduce in playerRole valoarea 2
		jucator->playerRole = 2;
	else if (strcmp(roluri, impostor) == 0)//daca rolul playerului este de impostor vom introduce in playerRole valoarea 3
		jucator->playerRole = 3;
	jucator->ability = Abilities[jucator->playerRole];//definim abilitatea playerului
	free(coordonate);//eliberam memoria ocupata de pointerul coordonate
	free(roluri);//eliberam memoria ocupata de pointerul roluri
	return jucator;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	// TODO
	Game *joc;
	joc = allocGame();
	fscanf(inputFile, "%s", joc->name);
	fscanf(inputFile, "%d", &joc->killRange);
	fscanf(inputFile, "%d", &joc->numberOfCrewmates);
	joc->crewmates = malloc(joc->numberOfCrewmates * sizeof(Player));//alocam memorie pentru crewmates
	int nr_crewmates = joc->numberOfCrewmates;
	for (int i = 0; i < nr_crewmates; i++)
	{
		joc->crewmates[i] = ReadPlayer(inputFile);//citim datele fiecarui player implicat in joc
	}
	joc->impostor = ReadPlayer(inputFile);//citim datele impostorului
	return joc;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	// TODO
	char *role;
	role=fromEnumtoString(player->playerRole);//ne folosim de functia fromEnumtoString pentru a calcula rolul jucatorului pe care ulterior il vom printa
	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n", player->name, player->color, player->hat, role);
	for (int i = 0; i < player->numberOfLocations; i++)//afisam toate locatiile playerului respectiv
	{
		if (i == 0)
			fprintf(outputFile, "Player's locations: (%d,%d) ", player->locations[i].x, player->locations[i].y);
		else
			fprintf(outputFile, "(%d,%d) ", player->locations[i].x, player->locations[i].y);
	}
	fprintf(outputFile, "\n");
	free(role);//eliberam memoria ocupata de pointerul role
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	// TODO
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game->numberOfCrewmates);
	fprintf(outputFile, "\tCrewmates:\n");
	for (int i = 0; i < game->numberOfCrewmates; i++)//apelam functia WritePlayer pentru fiecare crewmate
		WritePlayer(game->crewmates[i], outputFile);
	fprintf(outputFile, "\n");
	fprintf(outputFile, "\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);//apelam functia WritePlayer si pentru impostor
}

//Task 7
void *KillPlayer(void *input)
{
	// TODO
	Game *game;
	game = (Game *)input;
	char *nume_crewmate_omorat, *buffer;
	nume_crewmate_omorat = malloc(MAX_LEN_STR_ATR * sizeof(char));//alocarea memoriei pentru pointerul nume_crewmate_omorat
	buffer = malloc(5 * MAX_LEN_STR_ATR * sizeof(char));//alocare memoriei pentru pointerul buffer
	int MIN = 99999999, poz;
	for (int j = 0; j < game->numberOfCrewmates; j++)
	{
		int distanta_manhattan;
		int x_impostor = game->impostor->locations[game->impostor->indexOfLocation].x;//x-ul la care se afla impostorul
		int y_impostor = game->impostor->locations[game->impostor->indexOfLocation].y;//y-ul la care se afla impostorul
		int x_crewmate = game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].x;//x-ul la care se afla in momentul respectiv crewmate-ul de pe pozitia j
		int y_crewmate = game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].y;//y-ul la care se afla in momentul respectiv crewmate-ul de pe pozitia j
		distanta_manhattan = abs(x_impostor - x_crewmate) + abs(y_impostor - y_crewmate);//calcului distantei manhattan dupa formula sa
		//verificam daca distanta manhattan calculata este mai mica decat minimul calculat pana atunci
		//daca aceasta este mai mica decat killRange-ul jocului
		//si daca crewmate-ul pentru care o calculam este in viata
		if (distanta_manhattan < MIN && distanta_manhattan <= game->killRange && game->crewmates[j]->alive == 1)
		{
			MIN = distanta_manhattan;//actualizam minimului
			strcpy(nume_crewmate_omorat, game->crewmates[j]->name);//salvam numele crewmate-ului
			poz = j;//salvam index-ul crewmate-ului
		}
		//verificam daca distanta pe care o calculam este egala cu minimul calculat pana la momentul respectiv
		//daca aceasta este mai mica decat killRange-ul jocului si daca crewmate-ul este in viata
		//am tratat cazul in care mai multi colegi de echipa se afla la acceasi distanta fata de impostor si il selectam pe cel cu index-ul cel mai mare(j>poz)
		else if (distanta_manhattan == MIN && distanta_manhattan <= game->killRange && game->crewmates[j]->alive == 1 && j > poz)
		{
			MIN = distanta_manhattan;
			strcpy(nume_crewmate_omorat, game->crewmates[j]->name);
			poz = j;
		}
	}
	if (MIN > game->killRange)//in cazul in care minimul calculat este mai mare decat killRange
		sprintf(buffer, "Impostor %s couldn't kill anybody.", game->impostor->name);
	else if (MIN <= game->killRange)//inseamna ca am gasit un player ce poate fi ucis de catre impostor
	{
		game->crewmates[poz]->alive = 0;//actualizam campul alive al playerului omorat cu valoarea 0
		sprintf(buffer, "Impostor %s has just killed crewmate %s from distance %d.", game->impostor->name, nume_crewmate_omorat, MIN);
	}
	free(nume_crewmate_omorat);//eliberam memoria ocupata de pointerul nume_crewmate_omorat
	return buffer;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	// TODO
	char *kill=NULL;
	for (int j = 0; j < game->numberOfCrewmates; j++)
	{
		char *decode=NULL, *invert=NULL, *rotate=NULL;
		game->crewmates[j]->indexOfLocation++;//incrementam index-ul locatiei crewmate-ului de pe pozitia j
		//in cazul in care suntem pe cale de a depasi numarul locatiilor crewmate-ului respectiv actualizam index-ul locatiei acestuia cu valoarea 0
		if (game->crewmates[j]->indexOfLocation >= game->crewmates[j]->numberOfLocations)
			game->crewmates[j]->indexOfLocation = 0;
		if (game->crewmates[j]->alive == 0)//verificam daca crewmate-ul este in viata iar in caz contrar printam mesajul corespunzator
			fprintf(outputFile, "Crewmate %s is dead.\n", game->crewmates[j]->name);
		else if (game->crewmates[j]->alive == 1)//cazul in care crewmate-ul este in viata
		{
			if (game->crewmates[j]->playerRole == 0)//verificam daca rolul acestuia este 0(rotator) si realizam abilitatea de care acesta dispune
			{
				fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game->crewmates[j]->name, game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].x, game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].y);
				fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[j]->name);
				rotate = RotateMatrix(inputMatrix[j]);//salvam in pointerul decode outputul functiei RotateMatrix aplicatat pentru linia matricei corespunzatoare crewmate-ului
				fprintf(outputFile, "%s\n", rotate);
				free(rotate);//eliberarea memoriei ocupate de pointerul rotate
			}
			else if (game->crewmates[j]->playerRole == 1)
			{
				fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game->crewmates[j]->name, game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].x, game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].y);
				fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[j]->name);
				decode = DecodeString(inputMatrix[j]);
				fprintf(outputFile, "%s\n", decode);
				free(decode);
			}
			else if (game->crewmates[j]->playerRole == 2)
			{
				fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", game->crewmates[j]->name, game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].x, game->crewmates[j]->locations[game->crewmates[j]->indexOfLocation].y);
				fprintf(outputFile, "Crewmate %s's output:\n", game->crewmates[j]->name);
				invert = InvertArray(inputMatrix[j]);
				fprintf(outputFile, "%s\n", invert);
				free(invert);
			}
		}
	}
	//aceeasi idee ca la un crewmate obisnuit doar ca este aplicata pentru abilitatea impostorului de a ucide playeri
	game->impostor->indexOfLocation++;
	if (game->impostor->indexOfLocation >= game->impostor->numberOfLocations)
		game->impostor->indexOfLocation = 0;
	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", game->impostor->name, game->impostor->locations[game->impostor->indexOfLocation].x, game->impostor->locations[game->impostor->indexOfLocation].y);
	fprintf(outputFile, "Impostor %s's output:\n", game->impostor->name);
	kill = KillPlayer(game);
	fprintf(outputFile, "%s\n", kill);
	free(kill);
}

// Task 9
void FreePlayer(Player *player)
{
	// TODO
	free(player->name);//eliberareea memoriei ocupate de pointerul name
	free(player->color);//eliberarea memoriei ocupate de pointerul color
	free(player->hat);//eliberarea memoriei ocupate de pointerul  hat
	free(player->locations);//eliberarea memoriei ocupate de pointerul locations
	free(player);//eliberarea memoriei ocupate de pointerul la structura player
}

// Task 9
void FreeGame(Game *game)
{
	// TODO
	free(game->name);//eliberarea memoriei ocupate de pointerul name
	for (int i = 0; i < game->numberOfCrewmates; i++)
		FreePlayer(game->crewmates[i]);//eliberarea memoriei ocupate de fiecare crewmate in parte utilizand functia definita anterior
	FreePlayer(game->impostor);//eliberarea memoriei ocupate de impostor
	free(game->crewmates);//eliberarea memoriei ocupate de pointerul crewmates
	free(game);//eliberarea memoriei ocupate de pointerul la structura game
}