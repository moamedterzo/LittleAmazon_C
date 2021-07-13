/**
 * @file Categorie.h
 * Questa libreria definisce varie funzioni che lavorano sulla tipologia di dato 'Categoria'
 *
 * @authors Amedeo Racanati
 */


#ifndef CATEGORIE_H_
#define CATEGORIE_H_

#include "Utility.h"

/**
 * Nome del file contenente i dati delle categorie
 */
#define FILE_CATEGORIE "Categorie.txt"


/**
 * Mostra a video l'elenco delle categorie
 */
void CategoriaElenco();

/**
 * Metodo richiamato per aggiungere una categoria al sistema
 */
void CategoriaInserimento();

/**
 * Metodo richiamato per eliminare una categoria
 */
void CategoriaEliminazione();

/**
 * Controlla se esiste una categoria con un nome specificato
 *
 * @param categoria Nome della categoria da considerare
 * @return 'True' se esiste una categoria con quel nome, 'False' altrimenti
 */
bool CheckCategoria(Categoria categoria);

/**
 * Controlla se esiste almeno una categoria nel sistema
 * @return 'True' se esiste una categoria, 'False' altrimenti
 */
bool ExistsOneCategoria();



#endif /* CATEGORIE_H_ */
