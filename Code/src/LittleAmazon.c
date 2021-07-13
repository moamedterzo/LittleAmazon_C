/**
 * @file LittleAmazon.c
 * File principale dal quale vengono richiamate tutte le altre funzioni.
 * In questo file vengono inizializzati i file, richiamate le funzione del menu'
 * e infine al termine del programma vengono effettuate delle operazioni sui file che memorizzano i dati.
 * @authors Amedeo Racanati
 */

//Inizio inclusione file
#include "Utility.h"

#include "Categorie.h"
#include "Prodotti.h"
#include "Utenti.h"
#include "Preferenze.h"
//Fine inclusione file

//Inizio prototipi

/**
 * Carica gli utenti dal file in memoria.
 * @return 'True' se non va a buon fine, 'False' se va a buon fine
 */
bool CaricamentoUtenti();

/**
 * Carica i prodotti dal file in memoria.
 * @return 'True' se non va a buon fine, 'False' se va a buon fine
 */
bool CaricamentoProdotti();

/**
 * Carica le categorie dal file in memoria.
 * @return 'True' se non va a buon fine, 'False' se va a buon fine
 */
bool CaricamentoCategorie();

/**
 * Carica le preferenze dal file in memoria.
 * @return 'True' se non va a buon fine, 'False' se va a buon fine
 */
bool CaricamentoPreferenze();

/**
 * Carica le voci di menu' inerenti ai prodotti
 */
void GestioneProdotti();

/**
 * Carica le voci di menu' inerenti alle categorie
 */
void GestioneCategorie();

/**
 * Carica le voci di menu' inerenti agli utenti
 */
void GestioneUtenti();

/**
 * Ordina le categorie per nome e le salva nel file, dopodiche' annulla il puntatore al file
 */
void DisposeCategorie();

/**
 * Ordina i prodotti e li salva nel file, dopodiche' annulla il puntatore al file
 */
void DisposeProdotti();

/**
 * Ordina gli utenti e li salva nel file, dopodiche' annulla il puntatore al file
 */
void DisposeUtenti();

/**
 * Se ci sono state modifiche, aggiorna il file delle preferenze, dopodiche' annulla il puntatore al file
 */
void DisposePreferenze();
//Fine prototipi



int main()
{
	//Inizializzazione variabili globali
	 NCategorie = 0;
	 NUtenti= 0;
	 NProdotti = 0;
	 NPreferenze = 0;
	 SalvaModificheFilePreferenze = false;
	 LastIDUtente = 0;
	 LastIDProdotto = 0;

    //Caricamenti dei vari file
	if (CaricamentoUtenti())
	{
		puts("Non e' stato possibile caricare il file degli utenti, il programma ora ha termine.");
	}
	else if (CaricamentoProdotti())
	{
		puts("Non e' stato possibile caricare il file dei prodotti, il programma ora ha termine.");
	}
	else if (CaricamentoCategorie())
	{
		puts("Non e' stato possibile caricare il file delle categorie, il programma ora ha termine.");
	}
	else if (CaricamentoPreferenze())
	{
		puts("Non e' stato possibile caricare il file delle preferenze, il programma ora ha termine.");
	}
	else
	{
		//Benvenuto
		printf("Benvenuto!\n\nAl momento sono presenti:\n%d categorie\n%d prodotti\n%d utenti\n%d preferenze\n\n",
			NCategorie, NProdotti, NUtenti, NPreferenze);

		system("pause");

		//Menù principale
		MENU voci[4] = { {"Gestione prodotti", GestioneProdotti},
		{  "Gestione categorie", GestioneCategorie},
		{  "Gestione utenti", GestioneUtenti },
		{  "Gestione preferenze", GestionePreferenze }
		};

		GestisciMenu(voci, 4);


		//Chiusura dei vari file
		DisposeCategorie();
		DisposeProdotti();
		DisposeUtenti();
		DisposePreferenze();
	}

	system("pause");

	return 0;
}



void GestioneProdotti()
{
	MENU voci[7] = { { "Ricerca prodotti", ProdottoRicerca },
		{ "Inserisci prodotto", ProdottoInserimento },
		{ "Visualizza prodotto", ProdottoVisualizza },
		{ "Ricerca prodotti per ditta", ProdottoRicercaDitta },
		{ "Ricerca prodotti per fascia prezzo", ProdottoRicercaPrezzo },
		{ "Ultimi prodotti inseriti", ProdottoElencoNuovi },
		{ "Elenco dei prodotti piu' venduti", ProdottoElencoPiuVenduti }
	};

	GestisciMenu(voci, 7);
}


void GestioneCategorie()
{
	MENU voci[3] = { { "Elenco categorie", CategoriaElenco },
	{ "Inserisci categoria", CategoriaInserimento },
	{ "Elimina categoria", CategoriaEliminazione }
	};

	GestisciMenu(voci, 3);
}


void GestioneUtenti()
{
	MENU voci[5] = { { "Ricerca utente", UtenteRicerca },
	{ "Inserisci utente", UtenteInserimento },
	{ "Visualizza utente", UtenteVisualizza },
	{ "Elenco degli utenti che effettuano piu' acquisti", UtenteElencoAcquisti },
	{ "Elenco dei nuovi utenti registrati", UtenteElencoNuovi }
	};

	GestisciMenu(voci, 5);
}



bool CaricamentoUtenti()
{
	//Apertura file
	flUtenti = fopen(FILE_UTENTI, "rb+");

	//Se non esiste il file lo creo
	if (flUtenti == NULL)
	{
		flUtenti = fopen(FILE_UTENTI, "wb+");

		//se è ancora nullo termino l'operazione
		if (flUtenti == NULL)
		{
			return 1;
		}
	}

	//Contatore degli utenti
	int i = 0;
	Utente utente;

	//Leggo dal file
	fread(&utente, sizeof(Utente), 1, flUtenti);

	//Finché non finisce il file...
	while (!feof(flUtenti))
	{
		//Se l'utente non è nullo...
		if (utente.ID != 0)
		{
			arrUtenti[i] = utente;
			i++;

			//Salvo l'ultimo ID
			LastIDUtente = utente.ID;
		}

		fread(&utente, sizeof(Utente), 1, flUtenti);

	}

	//Salvo numero di utenti
	NUtenti = i;

	return 0;
}

bool CaricamentoProdotti()
{
	//Apertura file
	flProdotti = fopen(FILE_PRODOTTI, "rb+");

	//Se non esiste il file lo creo
	if (flProdotti == NULL)
	{
		flProdotti = fopen(FILE_PRODOTTI, "wb+");

		//se è ancora nullo termino l'operazione
		if (flProdotti == NULL)
		{
			return 1;
		}
	}

	//Contatore degli prodotti
	int i = 0;
	Prodotto prodotto;

	//Leggo dal file
	fread(&prodotto, sizeof(Prodotto), 1, flProdotti);

	//Finché non finisce il file...
	while (!feof(flProdotti))
	{
		//Se il prodotto non è nullo...
		if (prodotto.ID != 0)
		{
			arrProdotti[i] = prodotto;
			i++;

			//Salvo l'ultimo ID
			LastIDProdotto = prodotto.ID;
		}

		fread(&prodotto, sizeof(Prodotto), 1, flProdotti);

	}

	//Salvo numero di prodotti
	NProdotti = i;

	return 0;
}

bool CaricamentoCategorie()
{
	//Apertura file
    flCategorie = fopen(FILE_CATEGORIE, "rb+");

    //Se non esiste il file lo creo
	if (flCategorie == NULL)
	{
		flCategorie = fopen(FILE_CATEGORIE, "wb+");

		//se è ancora nullo termino l'operazione
		if (flCategorie == NULL)
		{
			return 1;
		}
	}

	//Contatore delle categorie
	int i = 0;
	Categoria categoria;

	//Leggo dal file
	fread(&categoria, sizeof(Categoria), 1, flCategorie);

	//Finché non finisce il file...
	while (!feof(flCategorie))
	{
		//Copio array in memoria
		CopyArray(arrCategorie[i], categoria, L_NOME_CATEGORIA);

		i++;

		fread(&categoria, sizeof(Categoria), 1, flCategorie);

	}

	//Salvo numero di categorie
	NCategorie = i;

	return 0;

}

bool CaricamentoPreferenze()
{
	//Apertura file
	flPreferenze = fopen(FILE_PREFERENZE, "rb+");

	 //Se non esiste il file lo creo
	if (flPreferenze == NULL)
	{
		flPreferenze = fopen(FILE_PREFERENZE, "wb+");

		//se è ancora nullo termino l'operazione
		if (flPreferenze == NULL)
		{
			return 1;
		}
	}

	//Contatore delle preferenze
	int i = 0;
	Preferenza preferenza;

	//Leggo dal file
	fread(&preferenza, sizeof(Preferenza), 1, flPreferenze);

	//Finché non finisce il file...
	while (!feof(flPreferenze))
	{
		//Se la preferenza non è nulla...
		if (preferenza.IDUtente != 0)
		{
			arrPreferenze[i] = preferenza;
			i++;
		}

		fread(&preferenza, sizeof(Preferenza), 1, flPreferenze);

	}

	//Salvo numero preferenze
	NPreferenze = i;

	return 0;
}


void DisposeCategorie()
{
	//indice di posizionamento
	int currentIndex = 0;

	//indice di lettura
	int readIndex = 0;

	//Variabile di lettura
	Categoria tempCategoria;
	Categoria voidCategoria = "";

	//Accorpo le categorie escludendo gli spazi vuoti
	for (int i = 0; i < NCategorie; i++)
	{
		//Se la categoria non è nulla
		if (strcmp(arrCategorie[i], ""))
		{
			//Se devo mettere la categoria in uno spazio vuoto lo faccio
			if (currentIndex < readIndex)
			{
				strcpy(arrCategorie[currentIndex], arrCategorie[readIndex]);
				strcpy(arrCategorie[readIndex], voidCategoria);
			}

			currentIndex++;
		}

		readIndex++;
	}

	//Ordino le categorie per nome
	//Ordinamento a bolle
	for (int i = 0; i < currentIndex; i++)
	{
		for (int j = 0; j < currentIndex - 1; j++)
		{
			if (strcmp(arrCategorie[j], arrCategorie[j + 1]) > 0)
			{
				strcpy(tempCategoria, arrCategorie[j]);

				strcpy(arrCategorie[j], arrCategorie[j + 1]);

				strcpy(arrCategorie[j + 1], tempCategoria);
			}
		}
	}

	//Chiudo il file e lo riapro in scrittura, eliminando il contenuto precedente
	fclose(flCategorie);
	flCategorie = fopen(FILE_CATEGORIE, "wb");

	//Mi posiziono all'inizio del file e scrivo su di esso
	fseek(flCategorie, 0, SEEK_SET);

	for (int i = 0; i < NCategorie; i++)
	{
		if (strcmp(arrCategorie[i], ""))
		{
			fwrite(&arrCategorie[i], sizeof(Categoria), 1, flCategorie);
		}
	}

	//Chiusura file
	fclose(flCategorie);
}

void DisposeProdotti()
{
	//indice di posizionamento
	int currentIndex = 0;

	//indice di lettura
	int readIndex = 0;

	//Variabile di lettura
	Prodotto tempProdotto;

	//Variabile azzeratrice
	Prodotto voidProdotto;
	voidProdotto.ID = 0;

	//Inizio a leggere i prodotti e rimuovo gli spazi vuoti
	fseek(flProdotti, 0, SEEK_SET);

	fread(&tempProdotto, sizeof(Prodotto), 1, flProdotti);

	while (!feof(flProdotti))
	{
		//se il prodotto ha valore...
		if (tempProdotto.ID != 0)
		{
			//se devo mettere la prodotto in uno spazio vuoto lo faccio
			if (currentIndex < readIndex)
			{
				//mi posiziono all'elemento vuoto e scrivo il prodotto
				fseek(flProdotti, sizeof(Prodotto) * currentIndex, SEEK_SET);
				fwrite(&tempProdotto, sizeof(Prodotto), 1, flProdotti);

				//nella posizione originale azzero i dati
				fseek(flProdotti, sizeof(Prodotto) * readIndex, SEEK_SET);
				fwrite(&voidProdotto, sizeof(Prodotto), 1, flProdotti);

				//ritorno alla posizione originale
				fseek(flProdotti, sizeof(Prodotto) * (readIndex + 1), SEEK_SET);
			}

			currentIndex++;
		}

		readIndex++;

		fread(&tempProdotto, sizeof(Prodotto), 1, flProdotti);
	}

	//Chiusura file
	fclose(flProdotti);
}

void DisposeUtenti()
{
	//indice di posizionamento
	int currentIndex = 0;

	//indice di lettura
	int readIndex = 0;

	//Variabile di lettura
	Utente tempUtente;

	//Variabile azzeratrice
	Utente voidUtente;
	voidUtente.ID = 0;

	//Inizio a leggere gli utenti e rimuovo gli spazi vuoti
	fseek(flUtenti, 0, SEEK_SET);

	fread(&tempUtente, sizeof(Utente), 1, flUtenti);

	while (!feof(flUtenti))
	{
		//se l'utente ha valore
		if (tempUtente.ID != 0)
		{
			//se devo mettere l'utente in uno spazio vuoto lo faccio
			if (currentIndex < readIndex)
			{
				//mi posiziono all'elemento vuoto e scrivo l'utente
				fseek(flUtenti, sizeof(Utente) * currentIndex, SEEK_SET);
				fwrite(&tempUtente, sizeof(Utente), 1, flUtenti);

				//nella posizione originale azzero i dati
				fseek(flUtenti, sizeof(Utente) * readIndex, SEEK_SET);
				fwrite(&voidUtente, sizeof(Utente), 1, flUtenti);

				//ritorno alla posizione originale
				fseek(flUtenti, sizeof(Utente) * (readIndex + 1), SEEK_SET);
			}

			currentIndex++;
		}

		readIndex++;

		fread(&tempUtente, sizeof(Utente), 1, flUtenti);
	}

    //Chiusura file
	fclose(flUtenti);
}

void DisposePreferenze()
{
	//Se bisogna effettuare dei salvataggi...
	if (SalvaModificheFilePreferenze)
	{
		//Mi posiziono all'inizio del file e scrivo i valori
		fseek(flPreferenze, 0, SEEK_SET);

		for (int i = 0; i < NPreferenze; i++)
		{
			fwrite(&arrPreferenze[i], sizeof(Preferenza), 1, flPreferenze);
		}
	}

    //Chiudo il file
	fclose(flPreferenze);
}
