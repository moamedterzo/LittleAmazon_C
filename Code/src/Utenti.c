/**
 * @file Utenti.c
 *
 * @authors Amedeo Racanati
 */


#include "Utility.h"
#include "Prodotti.h"
#include "Preferenze.h"
#include "Utenti.h"


void UtenteRicerca()
{
	//Se non esistono utenti in memoria do un messaggio appropriato...
	if (!ExistsOneUtente())
	{
		printf("\nNon sono presenti utenti\n");
	}
	else
	{
		//Filtro di ricerca
		printf("\nInserire il campo di ricerca: ");
		char filter[L_FILTRO_RICERCA];

		GetStringFromInput(filter, L_FILTRO_RICERCA);

		//Eseguo le ricerche un-case sensitive
		strlwr(filter);

		//Intestazione
		printf("\n%-5s %-30s %-30s %-12s", "ID", "Nome", "Cognome", "Data nascita\n");

		bool flagExists = false;

		for (int i = 0; i < NUtenti; i++)
		{
			//Controllo del nome/cognome/email
			if (arrUtenti[i].ID != 0 &&
				(strstr(GetLowerString(arrUtenti[i].Nome, L_ARRAY_CHAR), filter) != NULL ||
					strstr(GetLowerString(arrUtenti[i].Cognome, L_ARRAY_CHAR), filter) != NULL ||
					strstr(GetLowerString(arrUtenti[i].Email, L_ARRAY_CHAR), filter) != NULL))
			{
				flagExists = true;
				ShowUtenteForElenco(arrUtenti[i]);
			}
		}

		//Se non c'è neanche un risultato do un messaggio appropriato
		if (!flagExists)
		{
			printf("\nNon esistono utenti corrispondenti alla ricerca\n");
		}

	}

	system("pause");
}

void UtenteInserimento()
{
	printf("\n");

	//Variabile che memorizza i dati
	Utente utente;

	//Nome
	printf("Inserire il nome: ");
	GetStringFromInput(utente.Nome, L_ARRAY_CHAR);

	//Cognome
	printf("Inserire il cognome: ");
	GetStringFromInput(utente.Cognome, L_ARRAY_CHAR);

	//Email
	printf("Inserire l'email: ");
	GetStringFromInput(utente.Email, L_ARRAY_CHAR);

	//Telefono (opzionale)
	printf("Inserire il numero di telefono (10 cifre o nessuna):");
	GetStringFromInputExact(utente.NumeroTelefono, L_NUMERO_TELEFONO, true);

	//Sesso (le teorie gender per fini pratici vengono ignorate)
	printf("Inserire il sesso dell'utente (0 se Maschio / 1 se Femmina): ");

	int sceltaSesso = GetIntegerFromInput(0, 1);
	utente.Sesso = sceltaSesso ? true : false;

	//Data inscrizione
	printf("Inserire la data di iscrizione: \n");
	GetDataFromInput(&utente.DataIscrizione);

	//Data nascita
	printf("Inserire la data di nascita: \n");
	GetDataFromInput(&utente.DataNascita);

	//Id automatico
	utente.ID = ++LastIDUtente;

	//Inserimento in memoria
	arrUtenti[NUtenti] = utente;

	//Scrittura sul file
	fseek(flUtenti, 0, SEEK_END);
	fwrite(&utente, sizeof(Utente), 1, flUtenti);

	//Aggiornamento del contatore degli utenti
	NUtenti++;


	printf("Utente inserito con successo\n");


	system("pause");

}

void UtenteVisualizza()
{
	//Richiesta ID utente
	printf("\nInserire l'ID dell'utente: ");
	int IDUtente = GetIntegerFromInput(1, INT_MAX);

	//Ricerca in memoria
	int index = GetIndexUtenteFromArrayByID(IDUtente);

	if (index == NOT_FOUND)
	{
		//Utente non trovato
		printf("L'utente con l'ID specificato non esiste.\n");
		system("pause");
	}
	else
	{
		//Utente trovato, lo mostro
		Utente utente = arrUtenti[index];
		UtenteVisualizzaIntro(utente);

		//Mostro preferenze, se ci sono
		UtenteVisualizzaPreferenze(IDUtente);

		//Chiedo all'utente cosa vuole fare
		int scelta = -1;

		do
		{
			//Scelta menu
			printf("\nPremere 1 per modificare, 2 per eliminare, 3 per visualizzare i prodotti raccomandati, 4 per uscire: ");
			scelta = GetIntegerFromInput(1, 4);

			//Eliminazione
			if (scelta == 2)
			{
				//Conferma eliminazione
				printf("Sei sicuro di voler eliminare l'utente? s/n: ");
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
					UtenteElimina(index, utente);
					printf("\nUtente eliminato con successo.\n");

					//Eliminazione preferenze associate
					EliminazionePreferenzeUtente(IDUtente);

					system("pause");

					break;
				}

			}
			else if(scelta == 1)
			{
				//Modifica
				UtenteModifica(&utente, index);

				//mostro di nuovo i dati
				printf("\nUtente modificato\n\n");
				UtenteVisualizzaIntro(utente);
			}
			else if (scelta == 3)
			{
				//RECOMMENDER SYSTEM
				//Calcolo e mostro i prodotti raccomandati
				GetProdottiRaccomandati(IDUtente);
			}

		} while (scelta != 4);

	}
}

void UtenteVisualizzaIntro(Utente utente)
{
	printf("Nome: %s\n", utente.Nome);
	printf("Cognome: %s\n", utente.Cognome);
	printf("Email: %s\n", utente.Email);
	printf("Numero di telefono: %s\n", utente.NumeroTelefono);
	printf("Sesso: %s\n", utente.Sesso == false ? "Maschio" : "Femmina");
	printf("Data di iscrizione: %02d/%02d/%4d\n", utente.DataIscrizione.Giorno, utente.DataIscrizione.Mese, utente.DataIscrizione.Anno);
	printf("Data di nascita: %02d/%02d/%4d\n", utente.DataNascita.Giorno, utente.DataNascita.Mese, utente.DataNascita.Anno);
}

void UtenteVisualizzaPreferenze(int IDUtente)
{
	//Prima ottengo l'elenco delle preferenze associate all'utente, dopo mostro il nome del prodotto corrispondente

	//Algoritmo di ricerca binaria
	//Si tenga conto che l'array è ordinato per ID utente, e poi per ID prodotto

	//Array che memorizza le preferenze associate
	Preferenza *elencoPreferenzeAssociate = malloc(sizeof(Preferenza) * NProdotti);

	//Numero delle preferenze associate
	int nPreferenzeAssociate = 0;

	if (NPreferenze > 0)
	{
		int first = 0;
		int last = NPreferenze - 1;

		//Indice di ricerca
		int j;

		while (first <= last)
		{
			j = (first + last) / 2;

			//se è un elemento vuoto considero quello successivo
			while (arrPreferenze[j].IDUtente == 0 && j <= last)
			{
				j++;
			}

			//se è vero vuol dire che tutti gli elementi cercati da j a last sono vuoti,
			//perciò è come se scartassi la seconda metà dell'array
			if (j > last)
			{
				last = ((first + last) / 2) - 1;
			}
			else if (arrPreferenze[j].IDUtente == IDUtente)
			{
				//Ho trovato l'utente, ora salvo tutte le preferenze associate
				int posizione = j;

				//ricerca a destra
				do
				{
					elencoPreferenzeAssociate[nPreferenzeAssociate++] = arrPreferenze[posizione];

				} while (arrPreferenze[++posizione].IDUtente == IDUtente);

				//ricerca a sinistra
				posizione = j;

				while (arrPreferenze[--posizione].IDUtente == IDUtente)
				{
					elencoPreferenzeAssociate[nPreferenzeAssociate++] = arrPreferenze[posizione];
				}

				break;
			}
			else if (IDUtente > arrPreferenze[j].IDUtente)
			{
				first = j + 1;
			}
			else
			{
				last = j - 1;
			}

		}
	}

	//Ora ho l'elenco delle preferenze e il loro numero
	printf("\n");

	if (nPreferenzeAssociate == 0)
	{
		printf("Non sono presenti preferenze.\n");
	}
	else
	{
		printf("Preferenze:\n");

		//Mostro nome + tipologia preferenza
		for (int i = 0; i < nPreferenzeAssociate; i++)
		{
			//Ottengo il prodotto corrispondente
			Prodotto prodotto = arrProdotti[GetIndexProdottoFromArrayByID(elencoPreferenzeAssociate[i].IDProdotto)];

			//mostro il nome
			printf("%-30s: ", prodotto.Nome);

			//mostro il tipo di preferenza
			if (elencoPreferenzeAssociate[i].Tipo == TIPO_PREFERENZA_VISUALIZZATO)
			{
				printf("visualizzato");
			}
			else if (elencoPreferenzeAssociate[i].Tipo == TIPO_PREFERENZA_LISTA_DESIDERI)
			{
				printf("aggiunto alla lista dei desideri");
			}
			else if (elencoPreferenzeAssociate[i].Tipo == 1)
			{
				printf("acquistato un pezzo");
			}
			else
			{
				printf("acquistati %d pezzi", elencoPreferenzeAssociate[i].Tipo);
			}

			printf(".\n");
		}
	}
}

void UtenteModifica(Utente *utente, int index)
{
	//Menù
	printf("Inserire un numero per modificare un campo specifico:\n");
	printf("1: Nome\n");
	printf("2: Cognome\n");
	printf("3: Email\n");
	printf("4: Numero di telefono\n");
	printf("5: Sesso\n");
	printf("6: Data di iscrizione\n");
	printf("7: Data di nascita\n");

	int sceltaModifica = GetIntegerFromInput(0, 7);

	//In base alla scelta modifico un campo
	switch (sceltaModifica)
	{
	case 1:

		//Nome
		printf("Inserire il nome: ");
		GetStringFromInput(utente->Nome, L_ARRAY_CHAR);

		break;

	case 2:

		//Cognome
		printf("\nInserire il cognome: ");
		GetStringFromInput(utente->Cognome, L_ARRAY_CHAR);

		break;

	case 3:

		//Email
		printf("Inserire l'email: ");
		GetStringFromInput(utente->Email, L_ARRAY_CHAR);

		break;

	case 4:

		//Telefono (opzionale)
		printf("Inserire il numero di telefono (10 cifre o nessuna):");
		GetStringFromInputExact(utente->NumeroTelefono, L_NUMERO_TELEFONO, true);

		break;

	case 5:

		//Sesso
		printf("Inserire il sesso dell'utente (0 se Maschio / 1 se Femmina): ");

		int	sceltaSesso = GetIntegerFromInput(0, 1);
		utente->Sesso = sceltaSesso ? true : false;

		break;

	case 6:

		//Data inscrizione
		printf("Inserire la data di iscrizione: \n");
		GetDataFromInput(&utente->DataIscrizione);

		break;

	case 7:

		//Data nascita
		printf("Inserire la data di nascita: \n");
		GetDataFromInput(&utente->DataNascita);

		break;

	}

	//Modifica in memoria
	arrUtenti[index] = *utente;

	//Scrittura sul file
	fseek(flUtenti, index * sizeof(Utente), SEEK_SET);
	fwrite(utente, sizeof(Utente), 1, flUtenti);
}

void UtenteElimina(int index, Utente utente)
{
	//Eliminazione in memoria
	utente.ID = 0;
	arrUtenti[index] = utente;

	//Scrittura sul file
	fseek(flUtenti, index * sizeof(Utente), SEEK_SET);
	fwrite(&utente, sizeof(Utente), 1, flUtenti);

	//Se è l'ultimo elemento, diminuisco di uno la conta degli utenti
	if (index == NUtenti - 1)
	{
		NUtenti--;
	}
}


void UtenteElencoAcquisti()
{
	//Se non esiste neanche un utente...
	if (!ExistsOneUtente())
	{
		printf("\nNon sono presenti utenti\n");
	}
	else
	{
		//Creo un nuovo array, leggo un elemento alla volta e dopo ordino gli elementi
		PreferenzeAssociate *arrayOrdinato = malloc(sizeof(PreferenzeAssociate) * NUtenti);

		//Indica quanti utenti sono presenti in memoria
		int c = 0;

		//In questo modo compatto l'array da ordinare, togliendo gli elementi nulli
		for (int i = 0; i < NUtenti; i++)
		{
			if (arrUtenti[i].ID != 0)
			{
				arrayOrdinato[c].Index = i;

				//ottengo il numero di acquisti
				arrayOrdinato[c].Numero = GetAcquistiUtente(arrUtenti[i].ID);

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
						j >= 0 && x.Numero > arrayOrdinato[j].Numero; //ordinamento per numero di acquisti
						j = j - gap)
				{
					arrayOrdinato[j + gap] = arrayOrdinato[j];
					arrayOrdinato[j] = x;
				}
			}

		}

		//Output dei primi N utenti che hanno effettuato più acquisti, mostro anche il numero degli acquisti
		printf("\n%-5s %-30s %-30s %-12s %-6s\n", "ID", "Nome", "Cognome", "Data nascita", "Numero acquisti");

		for (int i = 0; i < N_UTENTI_ACQUISTI && i < c; i++)
		{
			Utente utente = arrUtenti[arrayOrdinato[i].Index];

			printf("%5d %-30s %-30s   %02d/%02d/%4d %15d\n", utente.ID, utente.Nome, utente.Cognome, utente.DataNascita.Giorno, utente.DataNascita.Mese, utente.DataNascita.Anno, arrayOrdinato[i].Numero);

		}
	}

	system("pause");
}

void UtenteElencoNuovi()
{
	//Se non esiste neanche un utente...
	if (!ExistsOneUtente())
	{
		printf("\nNon sono presenti utenti\n");
	}
	else
	{
		//Creo un nuovo array, leggo un elemento alla volta e dopo ordino gli elementi
		Utente *arrayOrdinato = malloc(sizeof(Utente) * NUtenti);

		//Indica quanti utenti sono presenti in memoria
		int c = 0;

		//In questo modo compatto l'array da ordinare, togliendo gli elementi nulli
		for (int i = 0; i < NUtenti; i++)
		{
			if (arrUtenti[i].ID != 0)
			{
				arrayOrdinato[c] = arrUtenti[i];
				c++;
			}
		}

		//Shell sort
		int gap;
		Utente x;
		int a[7] = { 21, 13, 9, 5, 3, 2, 1 };

		for (int k = 0; k < 7; k++)
		{
			gap = a[k];

			for (int i = gap; i < c; i++)
			{
				x = arrayOrdinato[i];

				for (int j = i - gap;
						j >= 0 && IsFirstDataBigger(x.DataIscrizione, arrayOrdinato[j].DataIscrizione); //Ordinamento per data di iscrizione
						j = j - gap)
				{
					arrayOrdinato[j + gap] = arrayOrdinato[j];
					arrayOrdinato[j] = x;
				}
			}

		}

		//Output dei primi N utenti nuovi, mostro anche la data di iscrizione
		printf("\n%-5s %-30s %-30s %-12s %-15s\n", "ID", "Nome", "Cognome", "Data nascita", "Data iscrizione");

		for (int i = 0; i < N_UTENTI_NUOVI && i < c; i++)
		{
			if (arrayOrdinato[i].ID != 0)
			{
				printf("%5d %-30s %-30s   %02d/%02d/%4d      %02d/%02d/%4d\n", arrayOrdinato[i].ID, arrayOrdinato[i].Nome, arrayOrdinato[i].Cognome, arrayOrdinato[i].DataNascita.Giorno, arrayOrdinato[i].DataNascita.Mese, arrayOrdinato[i].DataNascita.Anno,
					   arrayOrdinato[i].DataIscrizione.Giorno, arrayOrdinato[i].DataIscrizione.Mese, arrayOrdinato[i].DataIscrizione.Anno);
			}
		}

	}


	system("pause");
}

void ShowUtenteForElenco(Utente utente)
{
	printf("%5d %-30s %-30s   %02d/%02d/%4d\n", utente.ID, utente.Nome, utente.Cognome, utente.DataNascita.Giorno, utente.DataNascita.Mese, utente.DataNascita.Anno);
}


bool ExistsOneUtente()
{
	bool esiste = false;

	for (int i = 0; i < NUtenti; i++)
	{
		//Se l'utente è valorizzato...
		if (arrUtenti[i].ID != 0)
		{
			esiste = true;
			break;
		}
	}

	return esiste;
}

int GetIndexUtenteFromArrayByID(int ID)
{
	//Algoritmo di ricerca binaria, considerando che potrebbero essere presenti elementi vuoti
	int posizione = NOT_FOUND;

	int first = 0;
	int last = NUtenti - 1;

	//Indice di ricerca
	int j;

	while ((first <= last) && (posizione == NOT_FOUND))
	{
		j = (first + last) / 2;

		//se è un elemento vuoto considero quello successivo
		while (arrUtenti[j].ID == 0 && j <= last)
		{
			j++;
		}

		//se è vero vuol dire che tutti gli elementi cercati da j a last sono vuoti,
		//perciò è come se scartassi la seconda metà dell'array
		if (j > last)
		{
			last = ((first + last) / 2) - 1;
		}
		else if (arrUtenti[j].ID == ID)
		{
			//Assegno la posizione dell'utente
			posizione = j;
		}
		else if (ID > arrUtenti[j].ID)
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



void GetProdottiRaccomandati(int IDUtente)
{
	//Ottengo le preferenze dell'utente principale e il loro numero
	Preferenza preferenzeUtentePrincipale[PREFERENZE_UTENTE];
	int nPrefUtentePrincipale = GetElencoPreferenzeUtente(IDUtente, preferenzeUtentePrincipale);


	//Creo un array contenente gli utenti più simili all'utente principale
	UtenteSimilarita arrUtentiSimili[UTENTI_SIMILI_DA_CONSIDERARE];

	//Per ogno utente...
	for (int i = 0; i < NUtenti; i++)
	{
		//Se l'utente esiste ed è diverso da quello principale
		if (arrUtenti[i].ID != 0 && arrUtenti[i].ID != IDUtente)
		{
			//Ottengo la similarità dell'utente rispetto a quello principale
			UtenteSimilarita utentesimile;
			utentesimile.IndexUtente = i;
			GetUtenteSimilarita(preferenzeUtentePrincipale, nPrefUtentePrincipale, &utentesimile);

			//Vedo se questo utente è più simile degli altri utenti, in tal caso lo salvo nell'array.
			//E' stato fatto così per problemi di memoria legati all'istanziamento di array molto grandi

			//Se questo utente ha una similarità inferiore agli altri, indexMinimo rimane uguale a -1
			int indexMinimo = -1;
			int minimo = utentesimile.Similarita;

			//Calcolo il minimo
			for (int j = 0; j < UTENTI_SIMILI_DA_CONSIDERARE; j++)
			{
				if (arrUtentiSimili[j].Similarita < minimo)
				{
					indexMinimo = j;
					minimo = arrUtentiSimili[j].Similarita;
				}
			}

			//Rimpiazzo nel caso
			if (indexMinimo != -1)
			{
				arrUtentiSimili[indexMinimo] = utentesimile;
			}
		}
	}


	//ORA ottengo i prodotti non visualizzati e calcolo la stima in base agli utenti simili
	ProdottoStima prodottiDaRaccomandare[MAX_PRODOTTI];

	 //contatore prodotti da raccomandare
	int c = 0;

	 //considero solo i primi N utenti simili
	for (int i = 0; i < UTENTI_SIMILI_DA_CONSIDERARE && i < NUtenti; i++)
	{
		//per ogni preferenza dell'utente simile...
		for (int j = 0; j < arrUtentiSimili[i].NumeroPreferenze; j++)
		{
			//indica se il prodotto è stato già visto dall'utente principale
			bool visualizzato = false;

			//Controllo se è stato visualizzato o meno
			for (int k = 0; k < nPrefUtentePrincipale; k++)
			{
				if (arrUtentiSimili[i].Preferenze[j].IDProdotto == preferenzeUtentePrincipale[k].IDProdotto)
				{
					visualizzato = true;
					break;
				}
			}

			//Se non è stato visualizzato, controllo se il prodotto è stato già messo nella lista,
			//dopodiché aggiorno il valore stima di gradimento
			if (!visualizzato)
			{
				//indica se il prodotto è già presente nella lista o meno
				bool trovato = false;

				//ottengo l'indice del prodotto
				int indexProdotto = GetIndexProdottoFromArrayByID(arrUtentiSimili[i].Preferenze[j].IDProdotto);

				//se lo trovo nella lista aumento il valore della stima del prodotto
				for (int k = 0; k < c; k++)
				{
					if (prodottiDaRaccomandare[k].IndexProdotto == indexProdotto)
					{
						trovato = true;

						//aggiornamento stima
						if (arrUtentiSimili[i].Preferenze[j].Tipo > TIPO_PREFERENZA_LISTA_DESIDERI)
						{
							prodottiDaRaccomandare[k].Stima += STIMA_ACQUISTATO;
						}
						else if (arrUtentiSimili[i].Preferenze[j].Tipo > TIPO_PREFERENZA_VISUALIZZATO)
						{
							prodottiDaRaccomandare[k].Stima += STIMA_LISTA_DESIDERI;
						}
						else
						{
							prodottiDaRaccomandare[k].Stima += STIMA_VISUALIZZATO;
						}

						break;
					}
				}

				//se il prodotto non è stato trovato lo aggiungo alla lista
				if (!trovato)
				{
					prodottiDaRaccomandare[c].IndexProdotto = indexProdotto;

					if (arrUtentiSimili[i].Preferenze[j].Tipo > TIPO_PREFERENZA_LISTA_DESIDERI)
					{
						prodottiDaRaccomandare[c].Stima = STIMA_ACQUISTATO;
					}
					else if (arrUtentiSimili[i].Preferenze[j].Tipo > TIPO_PREFERENZA_VISUALIZZATO)
					{
						prodottiDaRaccomandare[c].Stima = STIMA_LISTA_DESIDERI;
					}
					else
					{
						prodottiDaRaccomandare[c].Stima = STIMA_VISUALIZZATO;
					}

					c++;
				}
			}
		}
	}

	//Ora ordino per stima
	//Shell sort
	int gap;
	ProdottoStima x;
	int a[7] = { 21, 13, 9, 5, 3, 2, 1 };

	for (int k = 0; k < 7; k++)
	{
		gap = a[k];

		for (int i = gap; i < c; i++)
		{
			x = prodottiDaRaccomandare[i];

			for (int j = i - gap;
					j >= 0 && x.Stima > prodottiDaRaccomandare[j].Stima; //ordinamento per stima
					j = j - gap)
			{
				prodottiDaRaccomandare[j + gap] = prodottiDaRaccomandare[j];
				prodottiDaRaccomandare[j] = x;
			}
		}
	}

	//Visualizzo primi N prodotti
	printf("Prodotti raccomandati: \n");

	if (c > 0)
	{
		//Intestazione
		printf("\n%-5s %-30s %-7s %-5s", "ID", "Nome", "Prezzo", "Pezzi\n");

		//Elenco
		for (int i = 0; i < PRODOTTI_DA_RACCOMANDARE && i < c; i++)
		{
			ShowProdottoForElenco(arrProdotti[prodottiDaRaccomandare[i].IndexProdotto]);
		}
	}
	else
	{
		printf("Nessuno.");
	}
}


void GetUtenteSimilarita(Preferenza preferenzeUtentePrincipale[], int nPrefUtentePrincipale, UtenteSimilarita *utente2)
{
	//Ottengo le preferenze dell'altro utente
	utente2->NumeroPreferenze = GetElencoPreferenzeUtente(arrUtenti[utente2->IndexUtente].ID, utente2->Preferenze);


	//Viene utilizzato l'indice di Jaccard per calcolare l'affinità degli utenti, corretto in base al tipo di preferenza

	//Vedo se l'utente ha prodotti che l'utente principale non ha, inoltre calcolo l'intersezione

	//Variabile che indica l'unione insiemistica dei prodotti
	int unioneProdotti = nPrefUtentePrincipale;

	//Variabile che indica l'intersezione insiemistica dei prodotti, corretto in base al tipo di preferenza
	float intersezioneProdotti = 0;

	//Per ogni preferenza dell'utente da valutare...
	for (int i = 0; i < utente2->NumeroPreferenze; i++)
	{
		//Indica se il prodotto è stato visualizzato da entrambi gli utenti
		bool esiste = false;

		//Per ogni preferenza dell'utente principale, controllo se c'è la preferenza dell'utente principale
		for (int j = 0; j < nPrefUtentePrincipale; j++)
		{
			if (preferenzeUtentePrincipale[j].IDProdotto == utente2->Preferenze[i].IDProdotto)
			{
				//Il valore cambia in base al tipo di visualizzazione
				//0.5 se gli utenti l'hanno almeno visualizzato, 1 se è stato almeno aggiunto alla lista dei desideri, 1.5 se è stato comprato da entrambi
				if (preferenzeUtentePrincipale[j].Tipo >  TIPO_PREFERENZA_LISTA_DESIDERI
						&& utente2->Preferenze[j].Tipo > TIPO_PREFERENZA_LISTA_DESIDERI)
				{
					intersezioneProdotti += STIMA_ACQUISTATO;
				}
				else if (preferenzeUtentePrincipale[j].Tipo > TIPO_PREFERENZA_VISUALIZZATO
						&& utente2->Preferenze[j].Tipo > TIPO_PREFERENZA_VISUALIZZATO)
				{
					intersezioneProdotti += STIMA_LISTA_DESIDERI;
				}
				else
				{
					intersezioneProdotti += STIMA_VISUALIZZATO;
				}

				esiste = true;
				break;
			}
		}

		//se non c'è corrispondenza è un elemento in più nell'unione
		if (!esiste)
			unioneProdotti++;
	}

	//La similarità è data dall'intersezione/l'unione
	utente2->Similarita = unioneProdotti == 0 ? 0 : intersezioneProdotti / unioneProdotti;
}

int GetElencoPreferenzeUtente(int IDUtente, Preferenza preferenze[])
{
	//Algoritmo di ricerca binaria
	//Si tenga conto che l'array è ordinato per ID utente, e poi per ID prodotto

	//Conta il numero di preferenze associate all'utente
	int c = 0;

	if (NPreferenze > 0)
	{
		int first = 0;
		int last = NPreferenze - 1;

		//Indice di ricerca
		int j;

		while (first <= last)
		{
			j = (first + last) / 2;

			//se è un elemento vuoto considero quello successivo
			while (arrPreferenze[j].IDUtente == 0 && j <= last)
			{
				j++;
			}

			//se è vero vuol dire che tutti gli elementi cercati da j a last sono vuoti,
			//perciò è come se scartassi la seconda metà dell'array
			if (j > last)
			{
				last = ((first + last) / 2) - 1;
			}
			else if (arrPreferenze[j].IDUtente == IDUtente)
			{
				//Ho trovato l'utente, ora memorizzo tutte le preferenze associate
				int posizione = j;

				//ricerca a destra
				do
				{

					preferenze[c++] = arrPreferenze[posizione];

				} while (arrPreferenze[++posizione].IDUtente == IDUtente);

				//ricerca a sinistra
				posizione = j;

				while (arrPreferenze[--posizione].IDUtente == IDUtente)
				{
					preferenze[c++] = arrPreferenze[posizione];
				}

				break;
			}
			else if (IDUtente > arrPreferenze[j].IDUtente)
			{
				first = j + 1;
			}
			else
			{
				last = j - 1;
			}

		}
	}

	return c;
}




void EliminazionePreferenzeUtente(int IDUtente)
{
	//Algoritmo di ricerca binaria
	//Si tenga conto che l'array è ordinato per ID utente, e poi per ID prodotto
	if (NPreferenze > 0)
	{
		//Controlla se sono state effettuate modifiche o meno
		bool modificheEffettuate = false;

		int first = 0;
		int last = NPreferenze - 1;

		//Indice di ricerca
		int j;

		while (first <= last)
		{
			j = (first + last) / 2;

			//se è un elemento vuoto considero quello successivo
			while (arrPreferenze[j].IDUtente == 0 && j <= last)
			{
				j++;
			}

			//se è vero vuol dire che tutti gli elementi cercati da j a last sono vuoti,
			//perciò è come se scartassi la seconda metà dell'array
			if (j > last)
			{
				last = ((first + last) / 2) - 1;
			}
			else if (arrPreferenze[j].IDUtente == IDUtente)
			{
				//ho trovato l'utente, ora azzero tutte le preferenze associate
				modificheEffettuate = true;

				int posizione = j;

				//azzeramento a destra
				do
				{
					arrPreferenze[posizione].IDUtente = 0;
				} while (arrPreferenze[++posizione].IDUtente == IDUtente);

				//azzeramento a sinistra
				posizione = j;

				while (arrPreferenze[--posizione].IDUtente == IDUtente)
				{
					arrPreferenze[posizione].IDUtente = 0;
				}

				break;
			}
			else if (IDUtente > arrPreferenze[j].IDUtente)
			{
				first = j + 1;
			}
			else
			{
				last = j - 1;
			}

		}

		//Se ho effettuato delle modifiche riordino l'array delle preferenze
		if (modificheEffettuate)
		{
			OrdinaArrayPreferenze();
		}
	}

}

