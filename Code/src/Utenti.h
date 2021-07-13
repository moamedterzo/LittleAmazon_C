/**
 * @file Utenti.h
 * Questa libreria definisce varie funzioni che lavorano sulla tipologia di dato 'Utente'
 * Sono presenti algoritmi di ricerca, di modifica dei dati e di output dei valori.
 * Inoltre e' presente l'algoritmo che calcola i prodotti raccomandati di un utente.
 *
 * @authors Amedeo Racanati
 */

#ifndef UTENTI_H_
#define UTENTI_H_

#include "Utility.h"

/**
 * Nome del file contenente i dati degli utenti
 */
#define FILE_UTENTI "Utenti.txt"

/**
 * Numero che indica quanti utenti simili vengono considerati per la ricerca dei prodotti raccomandati
 */
#define UTENTI_SIMILI_DA_CONSIDERARE 5

/**
 * Numero dei prodotti da raccomandare
 */
#define PRODOTTI_DA_RACCOMANDARE 4

/**
 * Numero che indica una media delle preferenze che un utente puo' avere
 */
#define PREFERENZE_UTENTE 200

/**
 * Valore di stima di un prodotto da raccomandare se visualizzato da utenti affini
 */
#define STIMA_VISUALIZZATO 0.5

/**
 * Valore di stima di un prodotto da raccomandare se aggiunto alla lista desideri da utenti affini
 */
#define STIMA_LISTA_DESIDERI 1

/**
 * Valore di stima di un prodotto da raccomandare se acquistato da utenti affini
 */
#define STIMA_ACQUISTATO 1.5


/**
 * Struct usata per il calcolo degli utenti simili
 */
typedef struct
{
	int IndexUtente;
	float Similarita;
	Preferenza Preferenze[PREFERENZE_UTENTE]; ///Elenco delle preferenze associate all'utente
	int NumeroPreferenze;
} UtenteSimilarita;

/**
 * Struct usata per calcolare quanto un prodotto potrebbe piacere all'utente interessato
 */
typedef struct
{
	int IndexProdotto;
	float Stima;
} ProdottoStima;

/**
 *  Memorizza l'ID dell'ultimo utente in memoria, ossia il piu' grande
 */
int LastIDUtente;


/**
 * Effettua una ricerca degli utenti dato in input un filtro.
 * Restituisce tutti quegli utenti il cui nome/cognome/email2 abbia caratteri comuni col filtro
 */
void UtenteRicerca();

/**
 * Si occupa dell'inserimento di un utente chiedendo in input i dati necessari
 */
void UtenteInserimento();

/**
 * Visualizza i dati di un utente, dopodiche' e' possibile eliminarlo, modificarlo, o vedere i prodotti raccomandati
 */
void UtenteVisualizza();

/**
 * Visualizza a video i dati dell'utente
 * @param utente Variabile utente
 */
void UtenteVisualizzaIntro(Utente utente);

/**
 * Dato un ID utente, mostra a video le preferenze associate ad esso,
 * ossia il nome del prodotto e la tipologia di preferenza
 * @param IDUtente ID dell'utente da considerare
 */
void UtenteVisualizzaPreferenze(int IDUtente);

/**
 * Si occupa della modifica dell'utente
 * @param utente Variabile puntatore all'utente
 * @param index Posizione in memoria dell'utente
 */
void UtenteModifica(Utente *utente, int index);


/**
 * Si occupa dell'eliminazione definitiva dell'utente
 *
 * @param utente Variabile utente
 * @param index Posizione in memoria dell'utente
 *
 */
void UtenteElimina(int index, Utente utente);

/**
 * Viene effettuato un ordinamento degli utenti in base al numero di acquisti.
 * Infine viene mostrato l'elenco dei primi N utenti
 */
void UtenteElencoAcquisti();

/**
 * Viene effettuato un ordinamento degli utenti in base alla data di iscrizione.
 * Infine viene mostrato l'elenco dei primi N utenti
 */
void UtenteElencoNuovi();

/**
 * Mostra i dati principali dell'utente in modalita' elenco
 * @param utente Variabile utente
 */
void ShowUtenteForElenco(Utente utente);

/**
 * Controlla se esiste almeno un utente in memoria
 * @return 'True' se esiste almeno un utente, 'False' altrimenti
 */
bool ExistsOneUtente();

/**
 * Restituisce la posizione in memoria dell'utente in base all'ID specificato
 * @param ID ID dell'utente da trovare
 * @return La posizione dell'utente in memoria, -1 se non e' stato trovato
 */
int GetIndexUtenteFromArrayByID(int ID);

/**
 * Algoritmo che calcola i prodotti raccomandati per un utente.
 * Prima trova gli utenti simili, dopo trova i prodotti non visualizzati dall'utente e calcola la stima
 * di preferenza. Dopodiche' ordina i prodotti per stima e visualizza i primi N
 * @param IDUtente ID dell'utente specifico
 */
void GetProdottiRaccomandati(int IDUtente);

/**
 * Restituisce la similarità di un utente confrontandolo con l'utente principale.
 * L'indice utilizzato è quello di Jaccard, corretto in base al tipo di preferenza
 * @param preferenzeUtentePrincipale L'elenco delle preferenze dell'utente principale
 * @param nPrefUtentePrincipale Il numero di preferenze dell'utente principale
 * @param utente2 Il dato che salva la similarità del secondo utente
 */
void GetUtenteSimilarita(Preferenza preferenzeUtentePrincipale[], int nPrefUtentePrincipale, UtenteSimilarita *utente2);

/**
 * Restituisce l'elenco delle preferenze dell'utente.
 *
 * @param IDUtente ID dell'utente specifico
 * @param preferenze Elenco delle preferenze memorizzate
 * @return Il numero delle preferenze
 */
int GetElencoPreferenzeUtente(int IDUtente, Preferenza preferenze[]);


/**
 *Elimina le preferenze associate ad un utente
 * @param IDUtente ID dell'utente specifico
 */
void EliminazionePreferenzeUtente(int IDUtente);


#endif /* UTENTI_H_ */
