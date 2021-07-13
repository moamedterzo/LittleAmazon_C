/**
 * @file Preferenze.h
 * Questa libreria definisce varie funzioni che lavorano sulla tipologia di dato 'Preferenza'
 *
 * @authors Amedeo Racanati
 */

#ifndef PREFERENZE_H_
#define PREFERENZE_H_

/**
 * Nome del file contenente i dati delle preferenze
 */
#define FILE_PREFERENZE "Preferenze.txt"

/**
 * Indica che il prodotto e' stato visualizzato
 */
#define TIPO_PREFERENZA_VISUALIZZATO -1

/**
 * Indica che il prodotto e' stato aggiunto alla lista dei desideri
 */
#define TIPO_PREFERENZA_LISTA_DESIDERI 0

/**
 * Si occupa di controllare se esistono prodotti e utenti nel sistema, e poi richiede l'ID dell'utente e del prodotto,
 * in modo tale da poter effettuare una creazione/modifica/eliminazione della preferenza
 */
void GestionePreferenze();

/**
 * Chiede all'utente l'operazione che vuole effettuare (creazione/modifica/eliminazione della preferenza)
 *
 * @param numeroPezzi Numero dei pezzi in magazzino del prodotto
 * @param IDUtente ID dell'utente
 * @param IDProdotto ID del prodotto
 * @return Il valore della scelta, se specificato
 */
int GetSceltaPreferenza(int numeroPezzi, int IDUtente, int IDProdotto);

/**
 * Restituisce il numero degli acquisti di un utente
 * @param IDUtente ID dell'utente specifico
 * @return Numero degli acquisti dell'utente
 */
int GetAcquistiUtente(int IDUtente);

/**
 * Restituisce il numero delle vendite di un prodotto
 * @param IDProdotto ID del prodotto specifico
 * @return Numero delle vendite di un prodotto
 */
int GetVenditeProdotto(int IDProdotto);

/**
 * Dato l'ID dell'utente e del prodotto, controlla se esiste una preferenza associata in memoria
 * @param IDUtente ID dell'utente specifico
 * @param IDProdotto ID del prodotto specifico
 * @return Indice della preferenza associata, -1 se non e' stata trovata.
 */
int GetIndexPreferenzeFromArray(int IDUtente, int IDProdotto);


#endif /* PREFERENZE_H_ */
