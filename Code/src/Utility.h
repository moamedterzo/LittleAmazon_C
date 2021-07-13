/**
 * @file Utility.h
 * Questa libreria definisce tutte quelle variabili e funzioni globali
 * utilizzate nel programma da altre funzioni.
 * Esse non riguardano percio' un'entita' specifica, ma risolvono problemi generali.
 * Inoltre qui dentro viene gestita la funzionalita' del menu' interattivo.
 *
 * @authors Amedeo Racanati
 */

#ifndef UTILITY_H_
#define UTILITY_H_

///Inclusioni
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include <limits.h>
///Fine inclusioni

/**
 * Tasto "Freccia su"
 */
#define FRECCIA_SU 80

/**
 * Tasto "Freccia giu'"
 */
#define FRECCIA_GIU 72

/**
 * Tasto "Invio"
 */
#define TASTO_INVIO 13

/**
 * Tasto "esc"
 */
#define TASTO_ESC 27

/**
 * E' utilizzato nella ricerca dell'indice di un elemento all'interno dell'array,
 * e indica che non e' stato trovato l'elemento
 */
#define NOT_FOUND -1

/**
 * Numero massimo di prodotti che si possono avere nel sistema
 */
#define MAX_PRODOTTI 2000

/**
 * Numero massimo di utenti che si possono avere nel sistema
 */
#define MAX_UTENTI 1000

/**
 * Numero massimo di categorie che si possono avere nel sistema
 */
#define MAX_CATEGORIE 50

/**
 * Numero massimo di preferenze che si possono avere nel sistema
 */
#define MAX_PREFERENZE 500000

/**
 * Lunghezza di una generica stringa per memorizzare i dati
 */
#define L_ARRAY_CHAR 30

/**
 * Lunghezza del nome della categoria
 */
#define L_NOME_CATEGORIA 20

/**
 * Lunghezza della descrizione
 */
#define L_DESCRIZIONE 200

/**
 * Lunghezza del numero di telefono
 */
#define L_NUMERO_TELEFONO 10

/**
 * Indica quanti elementi vengono visualizzati per la lista 'Prodotti nuovi'
 */
#define N_PRODOTTI_NUOVI 20

/**
 * Indica quanti elementi vengono visualizzati per la lista 'Utenti nuovi'
 */
#define N_UTENTI_NUOVI 20

/**
 * Indica quanti elementi vengono visualizzati per la lista 'Utenti che effettuano piu' acquisti'
 */
#define N_UTENTI_ACQUISTI 20

/**
 * Indica quanti elementi vengono visualizzati per la lista 'Prodotti piu' acquistati'
 */
#define N_PRODOTTI_ACQUISTI 20

/**
 * Lunghezza del filtro di ricerca
 */
#define L_FILTRO_RICERCA 100

/**
 * Colore della voce di menu' selezionata
 */
#define COLORE_ROSSO 4

/**
 * Colore delle altre voci di menu'
 */
#define COLORE_BIANCO 7


/**
 * Struct usata per la gestione del menu' interattivo
 */
typedef struct
{
	char VoceMenu[50];
	void(*Funzione)();
}MENU;

/**
 * Struct indicante la data in formato numerico
 */
typedef struct {
	int Giorno;
	int Mese;
	int Anno;
} Data;


/**
 * Entita' CATEGORIA
 */
typedef char Categoria[L_NOME_CATEGORIA];

/**
 * Entita' UTENTE
 */
typedef struct {
	int ID;
	char Email[L_ARRAY_CHAR];
	char Nome[L_ARRAY_CHAR];
	char Cognome[L_ARRAY_CHAR];
	Data DataNascita;
	Data DataIscrizione;
	/**
	 * 0 = maschio; 1 = femmina
	 */
	bool Sesso;
	char NumeroTelefono[L_NUMERO_TELEFONO];
} Utente;

/**
 * Entita' PRODOTTO
 */
typedef struct
{
	int ID;
	char Nome[L_ARRAY_CHAR];
	Categoria Categoria;
	float Prezzo;
	int NumeroPezzi;
	char Ditta[L_ARRAY_CHAR];
	Data DataInserimento;
	char Descrizione[L_DESCRIZIONE];
}Prodotto;

/**
 * Entita' PREFERENZA
 */
typedef struct {

	int IDUtente;
	int IDProdotto;
	short Tipo;  //-1 = visualizzato; 0 = lista dei desideri; 1 in poi = acquisto di N prodotti

}Preferenza;


/**
 * Struct usata per associare all'index di un elemento (utente o prodotto) il numero delle preferenze associate
 */
typedef struct
{
	int Index;
	int Numero;
} PreferenzeAssociate;


/**
 * Numero di categorie nel sistema
 */
int NCategorie;

/**
 * Numero di utenti nel sistema
 */
int NUtenti;

/**
 * Numero di prodotti nel sistema
 */
int NProdotti;

/**
 * Numero di preferenze nel sistema
 */
int NPreferenze;

/**
 * Indica se l'elenco delle preferenze e' stato modificato, in tal caso verra' aggiornato il file specifico
 */
bool SalvaModificheFilePreferenze;


/**
 * Categorie salvate in memoria
 */
Categoria arrCategorie[MAX_CATEGORIE];

/**
 * Utenti salvate in memoria
 */
Utente arrUtenti[MAX_UTENTI];

/**
 * Prodotti salvate in memoria
 */
Prodotto arrProdotti[MAX_PRODOTTI];

/**
 * Preferenze salvate in memoria
 */
Preferenza arrPreferenze[MAX_PREFERENZE];


/**
 * File ad accesso binario degli utenti
 */
FILE *flUtenti;

/**
 * File ad accesso binario delle categorie
 */
FILE *flCategorie;

/**
 * File ad accesso binario dei prodotti
 */
FILE *flProdotti;

/**
 * File ad accesso binario delle preferenze
 */
FILE *flPreferenze;

/**
 * Gestisce la selezione di una voce di menu'
 * @param menu Elenco delle voci di menu'
 * @param NumeroElementi Numero delle voci di menu'
 */
void GestisciMenu(MENU menu[], int NumeroElementi);

/**
 * Imposta un colore ai caratteri stampati sulla console.
 *
 * @param ForgC Numero indicante il colore da impostare
 */
void SetColor(int ForgC);

/**
 * Copia in un array il contenuto dell'altro array.
 *
 * @param arrTo Array dove vengono scritti i valori
 * @param arrFrom Array da dove vengono letti i valori
 * @param length Elementi da copiare
 */
void CopyArray(char arrTo[], char arrFrom[], int length);

/**
 * Assegna alla variabile str una stringa di una lunghezza massima equivalente al parametro 'length' inserita dalla console,
 * la stringa restituita può contenere spazi e deve essere lunga almeno un carattere.
 *
 * @param str Stringa di memorizzazione
 * @param length Lunghezza dei caratteri da leggere
 */
void GetStringFromInput(char str[], int length);

/**
 * Assegna alla variabile str una stringa di una lunghezza esattamente pari al parametro 'length' inserita dalla console,
 * la stringa restituita può contenere spazi e puo' essere vuota se il parametro 'alsoNullable' e' impostato a true
 *
 * @param str Stringa di memorizzazione
 * @param length Lunghezza dei caratteri da leggere
 * @param alsoNullable Indica se la stringa restituita possa essere anche vuota
 */
void GetStringFromInputExact(char str[], int length, bool alsoNullable);

/**
 * Restituisce un numero di tipo float inserito dalla console.
 *
 * @param strettamentePositivo Indica se il valore deve essere strettamente maggiore di zero.
 * @return Il numero di tipo float arrotondato a due cifre decimali.
 */
float GetFloatFromInput(bool strettamentePositivo);

/**
 * Restituisce un numero di tipo intero inserito dalla console, rientrante in un range specificato.
 *
 * @param min Valore minimo  del risultato.
 * @param max Valore massimo del risultato.
 * @return Il numero di tipo integer.
 */
int GetIntegerFromInput(int min, int max);

/**
 * Interroga l'utente per l'inserimento della data (Giorno/Mese/Anno)
 * @param dataPtr Puntatore al quale verranno associati i valori
 */
void GetDataFromInput(Data *dataPtr);

/**
 * Valuta quale delle due date è la maggiore
 *
 * @param data1 Prima data
 * @param data2 Seconda data
 * @return 'True' se la prima data e' maggiore della seconda, 'False' altrimenti
 */
bool IsFirstDataBigger(Data data1, Data data2);

/**
 * Restituisce un puntatore ad una stringa contenente caratteri tutti lower-case, partendo dalla stringa  'str'
 *
 * @param str Stringa da considerare
 * @param length Lunghezza della stringa
 * @return Stringa contenente caratteri tutti lower-case
 */
char* GetLowerString(char str[], const int length);


/**
 * Ordina l'array delle preferenze per IDUtente, e poi per IDProdotto in maniera ascendente.
 */
void OrdinaArrayPreferenze();



#endif /* UTILITY_H_ */
