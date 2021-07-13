/**
 * @file Prodotti.c
 *
 * @authors Amedeo Racanati
 */

#include "Utility.h"
#include "Categorie.h"
#include "Preferenze.h"
#include "Prodotti.h"

void ProdottoRicerca()
{
	//Se non esistono prodotti
	if (!ExistsOneProdotto())
	{
		printf("\nNon sono presenti prodotti\n");
	}
	else
	{
		//Ottengo filtro di ricerca
		printf("\nInserire il campo di ricerca: ");
		char filter[L_FILTRO_RICERCA];

		GetStringFromInput(filter, L_FILTRO_RICERCA);


		//Eseguo le ricerche un-case sensitive
		strlwr(filter);

		//Intestazione
		printf("\n%-5s %-30s %-7s %-5s", "ID", "Nome", "Prezzo", "Pezzi\n");

		bool flagExists = false;

		for (int i = 0; i < NProdotti; i++)
		{
			//Controllo i campi Nome/Descrizione/Categoria/Ditta
			if (arrProdotti[i].ID != 0 &&
				(strstr(GetLowerString(arrProdotti[i].Nome, L_ARRAY_CHAR), filter) != NULL ||
					strstr(GetLowerString(arrProdotti[i].Categoria, L_ARRAY_CHAR), filter) != NULL ||
					strstr(GetLowerString(arrProdotti[i].Ditta, L_ARRAY_CHAR), filter) != NULL ||
					strstr(GetLowerString(arrProdotti[i].Descrizione, L_ARRAY_CHAR), filter) != NULL))
			{
				flagExists = true;
				ShowProdottoForElenco(arrProdotti[i]);
			}
		}

		//Se non c'è neanche un risultato do un messaggio appropriato
		if (!flagExists)
		{
			printf("\nNon esistono prodotti corrispondenti alla ricerca\n");
		}
	}

	system("pause");
}

void ProdottoInserimento()
{
	printf("\n");

	//Se non esiste neanche una categoria non procedo...
	if (!ExistsOneCategoria())
	{
		printf("Per inserire un prodotto deve esistere almeno una categoria.\n");
	}
	else
	{
		//Variabile di memorizzazione dei dati
		Prodotto prodotto;

		//Nome
		printf("Inserire il nome: ");
		GetStringFromInput(prodotto.Nome, L_ARRAY_CHAR);

		//Elenco delle categorie disponibili
		printf("\nInserire il nome della categoria tra quelle disponibili:\n");
		for (int i = 0; i < NCategorie; i++)
		{
			if (strcmp(arrCategorie[i], ""))
			{
				printf("%s\n", arrCategorie[i]);
			}
		}

		//Categoria
		do
		{
			printf("Nome categoria: ");
			GetStringFromInput(prodotto.Categoria, L_NOME_CATEGORIA);

		} while (!CheckCategoria(prodotto.Categoria)); //mi assicuro che la categoria esista


		//Prezzo
		printf("\nInserire il prezzo: ");
		prodotto.Prezzo = GetFloatFromInput(true);

		//Numero pezzi
		printf("\nInserire il numero di pezzi in magazzino: ");
		prodotto.NumeroPezzi = GetIntegerFromInput(0, INT_MAX);

		//Data
		printf("Inserire la data di inserimento: \n");
		GetDataFromInput(&prodotto.DataInserimento);

		//Ditta
		printf("Inserire la ditta produttrice: ");
		GetStringFromInput(prodotto.Ditta, L_ARRAY_CHAR);

		//Descrizione
		printf("Inserire la descrizione: ");
		GetStringFromInput(prodotto.Descrizione, L_DESCRIZIONE);


		//Id automatico
		prodotto.ID = ++LastIDProdotto;

		//INSERIMENTO in memoria
		arrProdotti[NProdotti] = prodotto;

		//Scrittura sul file
		fseek(flProdotti, 0, SEEK_END);
		fwrite(&prodotto, sizeof(Prodotto), 1, flProdotti);

		//Incremento conta dei prodotti
		NProdotti++;

		printf("Prodotto inserito con successo\n");
	}

	system("pause");

}

void ProdottoVisualizza()
{
	//Input ID
	printf("\nInserire l'ID del prodotto: ");
	int IDProdotto = GetIntegerFromInput(1, INT_MAX);

	//Controllo esistenza prodotto
	int index = GetIndexProdottoFromArrayByID(IDProdotto);

	if (index == NOT_FOUND)
	{
		//Prodotto non trovato
		printf("Il prodotto con l'ID specificato non esiste.\n");
		system("pause");
	}
	else
	{
		//Esiste il prodotto, lo mostro a video
		Prodotto prod = arrProdotti[index];
		ProdottoVisualizzaIntro(prod);

		//Ora chiedo all'utente cosa vuol fare
		int scelta = -1;

		do
		{
			//Scelta menu
			printf("\nPremere 1 per modificare, 2 per eliminare, 3 per uscire: ");
			scelta = GetIntegerFromInput(1, 3);

			//Eliminazione
			if (scelta == 2)
			{
				//Input conferma eliminazione
				printf("Sei sicuro di voler eliminare il prodotto? s/n: ");
				char sceltaElimina[3] = "";

				do
				{
					scanf("%s", sceltaElimina);

				} while (strcmp(sceltaElimina, "s") && strcmp(sceltaElimina, "n"));

				//Rieffettuo interrogazione se la scelta è No
				if (!strcmp(sceltaElimina, "n"))
				{
					scelta = -1;
				}
				else
				{
					//Eliminazione vera e propria
					ProdottoElimina(prod, index);
					printf("\nProdotto eliminato con successo.\n");

					//Eliminazione delle preferenze associate
					for (int i = 0; i < NPreferenze; i++)
					{
						if (arrPreferenze[i].IDProdotto == IDProdotto)
						{
							arrPreferenze[i].IDUtente = 0;
						}
					}

					OrdinaArrayPreferenze();

					system("pause");

					break;
				}
			}
			//Modifica
			else if (scelta == 1)
			{
				ProdottoModifica(&prod, index);

				//mostro di nuovo i dati
				printf("\nProdotto modificato\n\n");
				ProdottoVisualizzaIntro(prod);
			}

		} while (scelta != 3);

	}


}

void ProdottoVisualizzaIntro(Prodotto prodotto)
{
	printf("Nome: %s\n", prodotto.Nome);
	printf("Categoria: %s\n", prodotto.Categoria);
	printf("Prezzo: %.2f\n", prodotto.Prezzo);
	printf("Numero pezzi: %d\n", prodotto.NumeroPezzi);
	printf("Ditta: %s\n", prodotto.Ditta);
	printf("Data: %02d/%02d/%4d\n", prodotto.DataInserimento.Giorno, prodotto.DataInserimento.Mese, prodotto.DataInserimento.Anno);
	printf("Descrizione: %s\n", prodotto.Descrizione);
}

void ProdottoModifica(Prodotto *prodotto, int index)
{
	//Scelta menù
	printf("Inserire un numero per modificare un campo specifico:\n");
	printf("1: Nome\n");
	printf("2: Categoria\n");
	printf("3: Prezzo\n");
	printf("4: Numero prezzi\n");
	printf("5: Ditta\n");
	printf("6: Data\n");
	printf("7: Descrizione\n");

	int sceltaModifica = GetIntegerFromInput(0, 7);

	//In base alla scelta modifico un campo
	switch (sceltaModifica)
	{
	case 1:

		//Nome
		printf("Inserire il nome: ");
		GetStringFromInput(prodotto->Nome, L_ARRAY_CHAR);

		break;

	case 2:

		//Elenco delle categorie disponibili
		printf("\nInserire il nome della categoria tra quelle disponibili:\n");
		for (int i = 0; i < NCategorie; i++)
		{
			if (strcmp(arrCategorie[i], ""))
			{
				printf("%s\n", arrCategorie[i]);
			}
		}

		//Categoria
		do
		{
			printf("Nome categoria: ");
			GetStringFromInput(prodotto->Categoria, L_NOME_CATEGORIA);

		} while (!CheckCategoria(prodotto->Categoria)); //mi assicuro che la categoria esista


		break;

	case 3:

		//Prezzo
		printf("\nInserire il prezzo: ");
		prodotto->Prezzo = GetFloatFromInput(true);

		break;

	case 4:

		//Numero pezzi
		printf("\nInserire il numero di pezzi in magazzino: ");
		prodotto->NumeroPezzi = GetIntegerFromInput(0, INT_MAX);

		break;

	case 5:

		//Ditta
		printf("Inserire la ditta produttrice: ");
		GetStringFromInput(prodotto->Ditta, L_ARRAY_CHAR);

		break;

	case 6:

		//Data
		printf("Inserire la data di inserimento: \n");
		GetDataFromInput(&prodotto->DataInserimento);

		break;

	case 7:

		//Descrizione
		printf("Inserire la descrizione: ");
		GetStringFromInput(prodotto->Descrizione, L_DESCRIZIONE);

		break;
	}

	//Modifica in memoria
	arrProdotti[index] = *prodotto;

	//Scrittura sul file
	fseek(flProdotti, index * sizeof(Prodotto), SEEK_SET);
	fwrite(prodotto, sizeof(Prodotto), 1, flProdotti);
}

void ProdottoElimina(Prodotto prodotto, int index)
{
	//Eliminazione in memoria
	prodotto.ID = 0;
	arrProdotti[index] = prodotto;

	//Scrittura sul file
	fseek(flProdotti, index * sizeof(Prodotto), SEEK_SET);
	fwrite(&prodotto, sizeof(Prodotto), 1, flProdotti);

	//Se è l'ultimo elemento, diminuisco di uno la conta dei prodotti
	if (index == NProdotti - 1)
	{
		NProdotti--;
	}
}

void ProdottoRicercaDitta()
{
	//Se non esiste neanche un prodotto do un messaggio differente
	if (!ExistsOneProdotto())
	{
		printf("\nNon sono presenti prodotti\n");
	}
	else
	{
		//Filtro ricerca
		printf("\nInserire il nome della ditta: ");
		char ditta[L_ARRAY_CHAR];

		GetStringFromInput(ditta, L_ARRAY_CHAR);

		//Eseguo ricerca un-case sensitive
		strlwr(ditta);

		//Intestazione
		printf("\n%-5s %-30s %-7s %-5s", "ID", "Nome", "Prezzo", "Pezzi\n");

		bool flagExists = false;

		for (int i = 0; i < NProdotti; i++)
		{
			//Controllo se la ditta sia uguale al filtro
			if (arrProdotti[i].ID != 0 && arrProdotti[i].NumeroPezzi > 0
					&& !strcmp(GetLowerString(arrProdotti[i].Ditta, L_ARRAY_CHAR), ditta))
			{
				flagExists = true;
				ShowProdottoForElenco(arrProdotti[i]);
			}
		}

		//Se non c'è neanche un risultato do un messaggio appropriato
		if (!flagExists)
		{
			printf("\nNon esistono prodotti corrispondenti alla ricerca\n");
		}

	}

	system("pause");
}

void ProdottoRicercaPrezzo()
{
	//Se non esiste neanche un prodotto do un messaggio differente
	if (!ExistsOneProdotto())
	{
		printf("\nNon sono presenti prodotti\n");
	}
	else
	{
		//Prezzo minimo
		printf("\nInserire il prezzo minimo: ");
		float prezzoMin = GetFloatFromInput(false);

		//Prezzo massimo. Il valore 0 indica che non viene considerato come parametro
		printf("\nInserire il prezzo massimo (0 per non specificare nulla): ");
		float prezzoMax;

		//mi assicuro che il prezzo massimo sia maggiore del prezzo minimo
		do
		{
			prezzoMax = GetFloatFromInput(false);
		} while(prezzoMax != 0 && prezzoMax < prezzoMin);

		//Intestazione
		printf("\n%-5s %-30s %-7s %-5s", "ID", "Nome", "Prezzo", "Pezzi\n");

		bool flagExists = false;

		for (int i = 0; i < NProdotti; i++)
		{
			if (arrProdotti[i].ID != 0)
			{
				//Ricerca per fascia prezzo
				if (arrProdotti[i].NumeroPezzi > 0
						&& (prezzoMax == 0 || arrProdotti[i].Prezzo <= prezzoMax)
						&& arrProdotti[i].Prezzo >= prezzoMin)
				{
					flagExists = true;
					ShowProdottoForElenco(arrProdotti[i]);
				}
			}
		}

		//Se non c'è neanche un risultato do un messaggio appropriato
		if (!flagExists)
		{
			printf("\nNon esistono prodotti corrispondenti alla ricerca\n");
		}
	}

	system("pause");

}

void ProdottoElencoNuovi()
{
	//Se non esiste neanche un prodotto do un messaggio differente
	if (!ExistsOneProdotto())
	{
		printf("\nNon sono presenti prodotti\n");
	}
	else
	{
		//Creo un nuovo array, leggo un elemento alla volta e dopo ordino gli elementi
		Prodotto *arrayOrdinato = malloc(sizeof(Prodotto) * NProdotti);

		//Indica il numero di prodotti presenti in memoria
		int c = 0;

		//In questo modo compatto l'array da ordinare, togliendo gli elementi nulli
		for (int i = 0; i < NProdotti; i++)
		{
			if (arrProdotti[i].ID != 0)
			{
				arrayOrdinato[c] = arrProdotti[i];
				c++;
			}
		}

		//Shell sort
		int gap;
		Prodotto x;
		int a[7] = { 21, 13, 9, 5, 3, 2, 1 };

		for (int k = 0; k < 7; k++)
		{
			gap = a[k];

			for (int i = gap; i < c; i++)
			{
				x = arrayOrdinato[i];

				for (int j = i - gap;
						j >= 0 && IsFirstDataBigger(x.DataInserimento, arrayOrdinato[j].DataInserimento); //ordinamento per data inserimento
						j = j - gap)
				{
					arrayOrdinato[j + gap] = arrayOrdinato[j];
					arrayOrdinato[j] = x;
				}
			}
		}

		//Output dei primi N prodotti nuovi, mostro anche la data di inserimento
		printf("\n%-5s %-30s %-7s %-5s %-15s\n", "ID", "Nome", "Prezzo", "Pezzi","Data inserimento");

		for (int i = 0; i < N_PRODOTTI_NUOVI && i < c; i++)
		{
			if (arrayOrdinato[i].ID != 0)
			{
				printf("%5d %-30s %7.2f %5d       %02d/%02d/%4d\n", arrayOrdinato[i].ID, arrayOrdinato[i].Nome, arrayOrdinato[i].Prezzo, arrayOrdinato[i].NumeroPezzi,
					arrayOrdinato[i].DataInserimento.Giorno, arrayOrdinato[i].DataInserimento.Mese, arrayOrdinato[i].DataInserimento.Anno);
			}
		}
	}

	system("pause");
}

void ProdottoElencoPiuVenduti()
{
	//Se non esiste neanche un prodotto do un messaggio differente
	if (!ExistsOneProdotto())
	{
		printf("\nNon sono presenti prodotti\n");
	}
	else
	{
		//Creo un nuovo array, leggo un elemento alla volta e dopo ordino gli elementi
		PreferenzeAssociate *arrayOrdinato = malloc(sizeof(PreferenzeAssociate) * NProdotti);

		//Indica il numero di prodotti presenti in memoria
		int c = 0;

		//In questo modo compatto l'array da ordinare, togliendo gli elementi nulli
		for (int i = 0; i < NProdotti; i++)
		{
			if (arrProdotti[i].ID != 0)
			{
				arrayOrdinato[c].Index = i;

				//Ottengo le vendite associate
				arrayOrdinato[c].Numero = GetVenditeProdotto(arrProdotti[i].ID);

				c++;
			}
		}

		//Shell sort
		int gap;
		PreferenzeAssociate x;
		int a[7] = { 21, 13, 9, 5, 3, 2, 1 };

		for (int k = 0; k < 7; k++)
		{
			gap = a[k];

			for (int i = gap; i < c; i++)
			{
				x = arrayOrdinato[i];

				for (int j = i - gap;
						j >= 0 && x.Numero > arrayOrdinato[j].Numero;  //ordinamento per prodotti più venduti
						j = j - gap)
				{
					arrayOrdinato[j + gap] = arrayOrdinato[j];
					arrayOrdinato[j] = x;
				}
			}

		}

		//Output dei primi N prodotti più venduti, mostro anche il numero delle vendite
		printf("\n%-5s %-30s %-7s %-5s %-6s\n", "ID", "Nome", "Prezzo", "Pezzi", "Numero vendite");

		for (int i = 0; i < N_PRODOTTI_ACQUISTI && i < c; i++)
		{
			Prodotto prodotto = arrProdotti[arrayOrdinato[i].Index];

			printf("%5d %-30s %7.2f %5d %14d\n", prodotto.ID, prodotto.Nome, prodotto.Prezzo, prodotto.NumeroPezzi, arrayOrdinato[i].Numero);

		}

	}


	system("pause");
}

void ShowProdottoForElenco(Prodotto prodotto)
{
	printf("%5d %-30s %7.2f %5d\n", prodotto.ID, prodotto.Nome, prodotto.Prezzo, prodotto.NumeroPezzi);
}


bool ExistsOneProdotto()
{
	bool esiste = false;

	for (int i = 0; i < NProdotti; i++)
	{
		//Se il prodotto ha valore...
		if (arrProdotti[i].ID != 0)
		{
			esiste = true;
			break;
		}
	}

	return esiste;
}

int GetIndexProdottoFromArrayByID(int ID)
{
	//Algoritmo di ricerca binaria, considerando che potrebbero essere presenti elementi vuoti
	int posizione = NOT_FOUND;

	int first = 0;
	int last = NProdotti - 1;

	//Indice posizione corrente di ricerca
	int j;

	while ((first <= last) && (posizione == NOT_FOUND))
	{
		j = (first + last) / 2;

		//se è un elemento vuoto considero quello successivo
		while (arrProdotti[j].ID == 0 && j <= last)
		{
			j++;
		}

		//se è vero vuol dire che tutti gli elementi cercati da j a last sono vuoti,
		//perciò è come se scartassi la seconda metà dell'array
		if (j > last)
		{
			last = ((first + last) / 2) - 1;
		}
		else if (arrProdotti[j].ID == ID)
		{
			//Assegno la posizione del prodotto
			posizione = j;
		}
		else if (ID > arrProdotti[j].ID)
		{
			first = j + 1;
		}
		else
		{
			last = j - 1;
		}

	}

	return posizione;
}


