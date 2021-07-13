/**
 * @file Preferenze.c
 *
 * @authors Amedeo Racanati
 */

#include "Utility.h"
#include "Prodotti.h"
#include "Utenti.h"
#include "Preferenze.h"

void GestionePreferenze()
{
	//Se non esiste un prodotto o un utente è impossibile procedere
	if (!ExistsOneProdotto())
	{
		printf("\nNon esistono prodotti, impossibile procedere.\n");
	}
	else if (!ExistsOneUtente())
	{
		printf("\nNon esistono utenti, impossibile procedere.\n");
	}
	else
	{
		//Richiesta ID prodotto
		printf("\nInserire l'ID del prodotto: ");
		int IDProdotto = GetIntegerFromInput(1, INT_MAX);

		int indexProdotto = GetIndexProdottoFromArrayByID(IDProdotto);

		if (indexProdotto == NOT_FOUND)
		{
			//Prodotto non trovato
			printf("Il prodotto con l'ID specificato non esiste.\n");

		}
		else
		{
			//Richiesta ID utente
			printf("\nInserire l'ID dell'utente: ");
			int IDUtente = GetIntegerFromInput(1, INT_MAX);

			int indexUtente = GetIndexUtenteFromArrayByID(IDUtente);

			if (indexUtente == NOT_FOUND)
			{
				//Utente non trovato
				printf("L'utente con l'ID specificato non esiste.\n");

			}
			else
			{
				//OK, ora si visualizzano i dati
				Utente utente = arrUtenti[indexUtente];
				Prodotto prodotto = arrProdotti[indexProdotto];

				//output
				printf("\nUtente: %s %s, prodotto: %s (%d pezzi in magazzino).\n", utente.Nome, utente.Cognome, prodotto.Nome, prodotto.NumeroPezzi);

				//Viene effettuata la scelta
				int scelta = GetSceltaPreferenza(prodotto.NumeroPezzi, IDUtente, IDProdotto);

				//Eventuale modifica dei pezzi in magazzino del prodotto, nel caso l'utente abbia acquistato dei prodotti
				if (scelta > 0)
				{
					//Salvataggio in memoria
					prodotto.NumeroPezzi -= scelta;
					arrProdotti[indexProdotto] = prodotto;

					//Salvataggio nel file
					fseek(flProdotti, indexProdotto * sizeof(Prodotto), SEEK_SET);
					fwrite(&prodotto, sizeof(Prodotto), 1, flProdotti);
				}
			}
		}

	}

	system("pause");
}

int GetSceltaPreferenza(int numeroPezzi, int IDUtente, int IDProdotto)
{
	//Cerco la preferenza in memoria
	int indexPreferenza = GetIndexPreferenzeFromArray(IDUtente, IDProdotto);

	Preferenza preferenza;

	//Mostro l'attuale stato della preferenza
	if (indexPreferenza == NOT_FOUND)
	{
		//Nessuna preferenza associata
		printf("Nessuna interazione col prodotto");
	}
	else
	{
		//Esiste la preferenza
		preferenza = arrPreferenze[indexPreferenza];

		if (preferenza.Tipo == TIPO_PREFERENZA_VISUALIZZATO)
		{
			//visualizzato
			printf("Il prodotto e' stato visualizzato");
		}
		else if (preferenza.Tipo == TIPO_PREFERENZA_LISTA_DESIDERI)
		{
			//aggiunto alla lista dei desideri
			printf("Il prodotto e' stato aggiunto alla lista dei desideri");
		}
		else if (preferenza.Tipo == 1)
		{
			//acquisto di un articolo
			printf("E' stato acquistato un pezzo di quel prodotto");
		}
		else
		{
			//acquisto di N articoli
			printf("Sono stati acquistati %d pezzi di quel prodotto", preferenza.Tipo);
		}

	}

	printf(".\n\n");

	//Chiedo all'utente cosa vuole fare
	int maxSceltaPreliminare = 2;

	printf("Inserire 1 per uscire, 2 per procedere con l'inserimento dei dati");

	//Se esiste la preferenza, do la possibilità di eliminarla
	if (indexPreferenza != NOT_FOUND)
	{
		maxSceltaPreliminare = 3;
		printf(", 3 per eliminare la preferenza");
	}
	printf(": ");

    //Ottengo valore
	int sceltaPreliminare = GetIntegerFromInput(1, maxSceltaPreliminare);

	//Uscita
	if (sceltaPreliminare == 1)
	{
		//restituisco un valore minore di 1, per evitare altre operazioni dopo l'uscita da questa funzione
		return 0;
	}
	//Eliminazione
	else if (sceltaPreliminare == 3)
	{
		//aggiornamento in memoria
		arrPreferenze[indexPreferenza].IDUtente = 0;
		OrdinaArrayPreferenze();

		printf("Preferenza eliminata con successo.\n");

		//restituisco un valore minore di 1, per evitare altre operazioni dopo l'uscita da questa funzione
		return 0;
	}
	else
	{
		//Ora chiedo quale dato vuole inserire l'utente
		//Si tenga presente che se un prodotto è stato aggiunto alla lista dei desideri, non è possibile impostare lo stato visualizzato
		//Se è stato acquistato un numero di prodotti, non è possibile impostare lo stato visualizzato o aggiunto alla lista dei desideri
		int sceltaMin = TIPO_PREFERENZA_VISUALIZZATO;

		//In questo caso non si può effettuare nessuna operazione, perché i prodotti sono finiti
		if (indexPreferenza != NOT_FOUND && preferenza.Tipo > -1 && numeroPezzi < 1)
		{
			printf("Non e' possibile effettuare l'acquisto di altri pezzi, perche' non sono disponibili in magazzino.\n");

			//restituisco un valore minore di 1, per evitare altre operazioni dopo l'uscita da questa funzione
			return 0;
		}
		else
		{
			printf("Inserire ");

			//Do per scontato che se un prodotto è stato acquistato, è stato anche visualizzato e aggiunto alla lista dei desideri
			//se è stato aggiunto alla lista dei desideri è stato anche visualizzato
			if (indexPreferenza == NOT_FOUND)
			{
				printf("-1 se il prodotto e' stato visualizzato, 0 se il prodotto e' stato aggiunto alla lista dei desideri");

				if (numeroPezzi > 0)
				{
					if (numeroPezzi == 1)
					{
						printf(", 1 per indicare l'acquisto di un pezzo di quel prodotto");
					}
					else
					{
						printf(", un numero da 1 a %d per indicare l'acquisto di N pezzi di quel prodotto", numeroPezzi);
					}
				}
			}
			else if (preferenza.Tipo == TIPO_PREFERENZA_VISUALIZZATO)
			{
				sceltaMin = TIPO_PREFERENZA_LISTA_DESIDERI;

				printf("0 se il prodotto e' stato aggiunto alla lista dei desideri");

				if (numeroPezzi > 0)
				{
					if (numeroPezzi == 1)
					{
						printf(", 1 per indicare l'acquisto di un pezzo di quel prodotto");
					}
					else
					{
						printf(", un numero da 1 a %d per indicare l'acquisto di N pezzi di quel prodotto", numeroPezzi);
					}
				}
			}
			else
			{
				//Nel caso in cui il prodotto sia soltanto acquistabile
				sceltaMin = 1;

				if (numeroPezzi == 1)
				{
					printf("1 per indicare l'acquisto di un pezzo di quel prodotto");
				}
				else
				{
					printf("un numero da 1 a %d per indicare l'acquisto di N pezzi di quel prodotto", numeroPezzi);
				}
			}

			printf(".\n");

			//Ottengo valore
			int scelta = GetIntegerFromInput(sceltaMin, numeroPezzi < 1 ? 0 : numeroPezzi);

			//Associazione dei dati
			preferenza.IDProdotto = IDProdotto;
			preferenza.IDUtente = IDUtente;

			if (indexPreferenza == NOT_FOUND || preferenza.Tipo < 0)
			{
				preferenza.Tipo = scelta;
			}
			else
			{
				//Se ho già effettuato degli acquisti bisogna considerarli e sommarli all'acquisto corrente
				preferenza.Tipo += scelta;
			}


			if (indexPreferenza == NOT_FOUND)
			{
				//inserimento in memoria
				arrPreferenze[NPreferenze++] = preferenza;
			}
			else
			{
				//modifica in memoria
				arrPreferenze[indexPreferenza] = preferenza;
			}

			//aggiorno array preferenze
			OrdinaArrayPreferenze();

			printf("Preferenza salvata con successo.\n");

			return scelta;
		}
	}
}



int GetAcquistiUtente(int IDUtente)
{
	//Algoritmo di ricerca binaria
	//Si tenga conto che l'array è ordinato per ID utente, e poi per ID prodotto
	int result = 0;

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
				//Ho trovato l'utente, ora conto tutti gli acquisti
				int posizione = j;

				//conta a destra
				do
				{
					if(arrPreferenze[posizione].Tipo  > TIPO_PREFERENZA_LISTA_DESIDERI)
					{
						result += arrPreferenze[posizione].Tipo;
					}

				} while (arrPreferenze[++posizione].IDUtente == IDUtente);

				//conta a sinistra
				posizione = j;

				while (arrPreferenze[--posizione].IDUtente == IDUtente)
				{
					if(arrPreferenze[posizione].Tipo  > TIPO_PREFERENZA_LISTA_DESIDERI)
					{
						result += arrPreferenze[posizione].Tipo;
					}
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

	return result;
}

int GetVenditeProdotto(int IDProdotto)
{
	//Algoritmo di ricerca sequenziale
	int result = 0;

	for (int i = 0; i < NPreferenze; i++)
	{
		if (arrPreferenze[i].IDProdotto == IDProdotto && arrPreferenze[i].IDUtente != 0)
		{
			if(arrPreferenze[i].Tipo  > TIPO_PREFERENZA_LISTA_DESIDERI)
			{
				result += arrPreferenze[i].Tipo;
			}
		}
	}

	return result;
}


int GetIndexPreferenzeFromArray(int IDUtente, int IDProdotto)
{
	//Algoritmo di ricerca binaria
	//Si tenga conto che l'array è ordinato per ID utente, e poi per ID prodotto
	int posizione = NOT_FOUND;

	if (NPreferenze > 0)
	{
		int first = 0;
		int last = NPreferenze - 1;

		//Indice di ricerca
		int j;

		while ((first <= last) && (posizione == NOT_FOUND))
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
				//Ho trovato l'id utente, ora cerco l'id prodotto

				//Queste variabili servono per capire se la ricerca deve continuare o meno
				bool ricercaIndietro = false;
				bool ricercaAvanti = false;

				//Continuo finché l'id utente è corretto
				do
				{
					if (arrPreferenze[j].IDProdotto == IDProdotto)
					{
						return j;
					}
					if (IDProdotto < arrPreferenze[j].IDProdotto)
					{
						j--;

						ricercaIndietro = true;

						//Vuol dire che il prodotto non esiste
						if (ricercaAvanti)
							break;
					}
					else
					{
						j++;

						ricercaAvanti = true;

						//Vuol dire che il prodotto non esiste
						if (ricercaIndietro)
							break;
					}

				} while (arrPreferenze[j].IDUtente == IDUtente);

				return NOT_FOUND;
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

	return posizione;
}
