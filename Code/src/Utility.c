/**
 * @file Utility.c
 *
 * @authors Amedeo Racanati
 */

#include "Utility.h"


void GestisciMenu(MENU menu[], int NumeroElementi)
{
	//Indica la selezione corrente di una voce di menù
	int sceltaCorrente = 0;

	//Continuo finché l'utente non preme il tasto 'esc'
	while (1)
	{
		int sceltaTasto;

		//Eseguo questa operazione finché l'utente non preme invio o esc
		do
		{
			//Cancello le scritte sulla console
			system("cls");

			//Stampo a video gli elementi
			for (unsigned i = 0; i < NumeroElementi; i++)
			{
				//L'elemento selezionato assume una colorazione differente
				if (sceltaCorrente == i)
				{
					SetColor(COLORE_ROSSO);
				}
				else
				{
					SetColor(COLORE_BIANCO);
				}

				//Nome della voce di menu'
				printf(menu[i].VoceMenu);
				printf("\n");
			}

			//Leggo il tasto premuto
			sceltaTasto = getch();

			//La variabile assume questo valore se è stato premuto un tasto freccia
			if (sceltaTasto == 224)
			{
				sceltaTasto = getch();

				if (sceltaTasto == FRECCIA_SU)
				{
					//la selezione va verso l'alto
					if (sceltaCorrente < NumeroElementi - 1)
						sceltaCorrente++;

				}
				else if (sceltaTasto == FRECCIA_GIU)
				{
					//la selezione va verso il basso
					if (sceltaCorrente > 0)
						sceltaCorrente--;
				}
			}


		} while (sceltaTasto != TASTO_INVIO && sceltaTasto != TASTO_ESC);


		//reimposto il colore bianco
		SetColor(COLORE_BIANCO);

		if (sceltaTasto == TASTO_ESC)
		{
			//esco
			break;
		}
		else
		{
			//eseguo la funzione
			menu[sceltaCorrente].Funzione();
		}
	}
}


void SetColor(int ForgC)
{
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}



void CopyArray(char arrTo[], char arrFrom[], int length)
{
	//Copio tanti elementi quanto è length
	for (int i = 0; i < length; i++)
	{
		arrTo[i] = arrFrom[i];
	}
}


void GetStringFromInput(char str[], int length)
{
	//Continuo finché la stringa e' vuota
	do
	{
		if (fgets(str, length, stdin))
		{
			//Legge gli ultimi caratteri del buffer
			char *p =  strchr(str, '\n');

			//Se viene letto il carattere \n non bisogna accettarlo
			if (p)
			{
				*p = 0;
			}
			else
			{
				//Leggo tutti i caratteri fino a \n
				scanf("%*[^\n]");
				scanf("%*c");
			}
		}
	} while (!strcmp(str, ""));

}


void GetStringFromInputExact(char str[], int length, bool alsoNullable)
{
	//Continuo finché la stringa
	do
	{
		if (fgets(str, length, stdin))
		{
			//Legge gli ultimi caratteri del buffer
			char *p =  strchr(str, '\n');

			//Se viene letto il carattere \n non bisogna accettarlo
			if (p)
			{
				*p = 0;
			}
			else
			{
				//Leggo tutti i caratteri fino a \n
				scanf("%*[^\n]");
				scanf("%*c");
			}
		}

	} while (strlen(str) != (length - 1) && (strcmp(str, "") || !alsoNullable));
	//Questo significa che per continuare il numero dei caratteri inseriti deve essere diverso da quello specificato
	//e che il numero dei caratteri deve essere diverso da 0, in tal caso se ho specificato
	//alsoNullable = true allora la stringa vuota è accettata ed esco dal ciclo

}


float GetFloatFromInput(bool strettamentePositivo)
{
	//Valore di ritorno
	float result;

	//Indica il risultato della lettura dei valori della console
	int i;
	do
	{

		i = scanf("%f", &result);

		//leggo tutto il resto che è stato inserito
		scanf("%*[^\n]");
		scanf("%*c");

	} while (!i || (strettamentePositivo && result <= 0) || (!strettamentePositivo && result < 0));
	// mi assicuro che il valore sia strettamente positivo nel caso sia stato specificato

	//arrotondo il valore alle due cifre decimali
	return floor(result * 100) / 100;
}


int GetIntegerFromInput(int min, int max)
{
	//Valore di ritorno
	int result;

	//Indica il risultato della lettura dei valori della console
	int i;
	do
	{

		i = scanf("%d", &result);

		//leggo tutto il resto che è stato inserito
		scanf("%*[^\n]");
		scanf("%*c");

	} while (!i || result < min || result > max);
	// mi assicuro che il valore rientri nel range specificato

	return result;
}


void GetDataFromInput(Data *dataPtr)
{
	//Richiesta anno
	printf("Anno: ");
	dataPtr->Anno = GetIntegerFromInput(1900, 2200);

	//Richiesta mese
	printf("Mese: \n");

	printf("1 = Gennaio \n");
	printf("2 = Febbraio \n");
	printf("3 = Marzo \n");
	printf("4 = Aprile \n");
	printf("5 = Maggio \n");
	printf("6 = Giugno \n");
	printf("7 = Luglio \n");
	printf("8 = Agosto \n");
	printf("9 = Settembre \n");
	printf("10 = Ottobre \n");
	printf("11 = Novembre \n");
	printf("12 = Dicembre \n");

	dataPtr->Mese = GetIntegerFromInput(1, 12);

	//Richiesta giorno
	printf("Giorno: ");

	//Calcolo giorno massimo in base al mese ed eventualmente anche in base all'anno
	int maxGiorno = 31;

	if (dataPtr->Mese == 2)
	{
		//se è febbraio valuto se l'anno è bisestile
		if (dataPtr->Anno % 400 == 0)
			maxGiorno = 29;
		else if (dataPtr->Anno % 100 == 0)
			maxGiorno = 28;
		else if (dataPtr->Anno % 4 == 0)
			maxGiorno = 29;
		else
			maxGiorno = 28;
	}
	else if (dataPtr->Mese == 4 ||
		dataPtr->Mese == 6 ||
		dataPtr->Mese == 9 ||
		dataPtr->Mese == 11)
	{
		//mese da 30 giorni
		maxGiorno = 30;
	}

	dataPtr->Giorno = GetIntegerFromInput(1, maxGiorno);
}

bool IsFirstDataBigger(Data data1, Data data2)
{
	if (data1.Anno > data2.Anno)
	{
		return true;
	}
	else if(data1.Anno == data2.Anno)
	{
		if (data1.Mese > data2.Mese)
		{
			return true;
		}
		else if (data1.Mese == data2.Mese)
		{
			return data1.Giorno > data2.Giorno;
		}
	}

	return false;
}

//Restituisce la copia della stringa specificata in lower case
char* GetLowerString(char str[], const int length)
{
	//Alloco una porzione di memoria per il nuovo puntatore
	char *lowerString = malloc(sizeof(char) * length);

	//Copio i caratteri nella nuova stringa
	CopyArray(lowerString, str, length);

	//Eseguo il lower-case dei caratteri
	return strlwr(lowerString);
}




void OrdinaArrayPreferenze()
{
	//Le modifiche verranno salvate anche sul file
	SalvaModificheFilePreferenze = true;

	//Variabile indicante il numero delle preferenze
	int c = 0;

	//Conto quante preferenze ci sono e compatto l'array
	//Una preferenza non è valida se ha il campo 'IDUtente' impostato a 0
	for (int i = 0; i < NPreferenze; i++)
	{
		if (arrPreferenze[i].IDUtente != 0)
		{
			arrPreferenze[c] = arrPreferenze[i];
			c++;
		}
	}

	//Azzero gli ultimi elementi
	for (int i = c; i < NPreferenze; i++)
	{
		arrPreferenze[i].IDUtente = arrPreferenze[i].IDProdotto = arrPreferenze[i].Tipo = 0;
	}


	//Shell sort con 8 gap
	int gap;
	Preferenza x;
	int a[8] = { 34, 21, 13, 9, 5, 3, 2, 1 };

	for (int k = 0; k < 8; k++)
	{
		gap = a[k];

		for (int i = gap; i < c; i++)
		{
			x = arrPreferenze[i];

			for (int j = i - gap;
				j >= 0 && (x.IDUtente < arrPreferenze[j].IDUtente || (x.IDUtente == arrPreferenze[j].IDUtente && x.IDProdotto < arrPreferenze[j].IDProdotto));
				j = j - gap)
			{
				arrPreferenze[j + gap] = arrPreferenze[j];
				arrPreferenze[j] = x;
			}
		}
	}

	//Salvo il numero delle preferenze
	NPreferenze = c;
}

