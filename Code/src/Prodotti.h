/**
 * @file Prodotti.h
 * Questa libreria definisce varie funzioni che lavorano sulla tipologia di dato 'Prodotto'
 * Sono presenti algoritmi di ricerca, di modifica dei dati e di output dei valori.
 *
 * @authors Amedeo Racanati
 */


#ifndef PRODOTTI_H_
#define PRODOTTI_H_

/**
 * Nome del file contenente i dati dei prodotti
 */
#define FILE_PRODOTTI "Prodotti.txt"

/**
 * Memorizza l'ID dell'ultimo prodotto in memoria, ossia il piu' grande
 */
int LastIDProdotto;

/**
 * Effettua una ricerca dei prodotti dato in input un filtro.
 * Restituisce tutti quei prodotti il cui nome/categoria/descrizione/ditta abbia caratteri comuni col filtro
 */
void ProdottoRicerca();

/**
 * Effettua l'inserimento di un prodotto chiedendo in input tutti i dati necessari
 */
void ProdottoInserimento();

/**
 * Visualizza i dati di un prodotto, dopodiche' e' possibile eliminarlo o modificarlo
 */
void ProdottoVisualizza();

/**
 * Visualizza a video i dati del prodotto
 * @param prodotto Variabile prodotto
 */
void ProdottoVisualizzaIntro(Prodotto prodotto);

/**
 * Si occupa della modifica del prodotto
 * @param prodotto Variabile puntatore al prodotto
 * @param index Posizione in memoria del prodotto
 */
void ProdottoModifica(Prodotto *prodotto, int index);

/**
 * Si occupa dell'eliminazione definitiva del prodotto
 *
 * @param prodotto Variabile prodotto
 * @param index Posizione in memoria del prodotto
 *
 */
void ProdottoElimina(Prodotto prodotto, int index);

/**
 * Effettua una ricerca dei prodotti in base alla ditta e li mostra a video
 */
void ProdottoRicercaDitta();

/**
 * Viene effettuata una ricerca dei prodotti per fascia prezzo.
 * Viene mostrato l'elenco dei prodotti che rientrano in una fascia minimo-massimo
 */
void ProdottoRicercaPrezzo();

/**
 * Viene effettuato un ordinamento dei prodotti in base alla data di inserimento.
 * Infine viene mostrato l'elenco dei primi N prodotti
 */
void ProdottoElencoNuovi();

/**
 * Viene effettuato un ordinamento dei prodotti in base ai piu' venduti.
 * Infine viene mostrato l'elenco dei primi N prodotti
 */
void ProdottoElencoPiuVenduti();

/**
 * Mostra i dati principali del prodotto in modalita' elenco
 * @param prodotto Variabile prodotto
 */
void ShowProdottoForElenco(Prodotto prodotto);

/**
 * Controlla se esiste almeno un prodotto in memoria
 * @return 'True' se esiste almeno un prodotto, 'False' altrimenti
 */
bool ExistsOneProdotto();

/**
 * Restituisce la posizione in memoria del prodotto in base all'ID specificato
 * @param ID ID del prodotto da trovare
 * @return La posizione del prodotto in memoria, -1 se non e' stato trovato
 */
int GetIndexProdottoFromArrayByID(int ID);


#endif /* PRODOTTI_H_ */
