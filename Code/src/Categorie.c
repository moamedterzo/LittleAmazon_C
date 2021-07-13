/**
 * @file Categorie.c
 *
 * @authors Amedeo Racanati
 */

#include "Utility.h"
#include "Categorie.h"


void CategoriaElenco()
{
	//Se non sono presenti categorie...
	if (NCategorie == 0)
	{
		printf("\nNon sono presenti categorie\n");
	}
	else
	{
		printf("\nElenco delle categorie:\n");

		//Per ogni categoria...
		for(int i = 0; i < NCategorie; i++)
		{
			//Se la categoria non è vuota la mostro
			if (strcmp(arrCategorie[i], ""))
			{
				printf("%s\n", arrCategorie[i]);
			}
		}
	}

	system("pause");
}

void CategoriaInserimento()
{
	//Leggo il nome della categoria
	Categoria categoria;

	printf("\nInserire il nome della categoria: ");
	scanf("%s", categoria);

	//Controllo che non esistano categorie con lo stesso nome
	if (CheckCategoria(categoria))
	{
		printf("Esiste gia' una categoria con questo nome\n");
	}
	else
	{
		//La aggiungo al sistema
		CopyArray(arrCategorie[NCategorie], categoria, L_NOME_CATEGORIA);

		//Scrivo sul file la categoria
		fseek(flCategorie, 0, SEEK_END);
		fwrite(&categoria, sizeof(Categoria), 1, flCategorie);

		//Aggiorno contatore
		NCategorie++;

		printf("Categoria inserita con successo\n");
	}

	system("pause");
}

void CategoriaEliminazione()
{
	//Leggo il nome della categoria
	Categoria categoria;

	printf("\nInserire il nome della categoria:");
	scanf("%s", categoria);

	//Indica se esiste una categoria col nome specificato
	bool trovato = false;
	bool haProdottiAssociati = false;

	//Cerco dentro l'array
	for (int i = 0; i < NCategorie; i++)
	{
		//Se ha lo stesso nome...
		if (!strcmp(arrCategorie[i], categoria))
		{
			trovato = true;

			//Controllo se ha prodotti associati, in tal caso è impossibile eliminarla
			for (int j = 0; j < NProdotti; j++)
			{
				//Se è un prodotto valido e ha la stessa categoria...
				if (arrProdotti[j].ID != 0 && !strcmp(arrProdotti[j].Categoria, arrCategorie[i]))
				{
					haProdottiAssociati = true;
					break;
				}
			}

			if (!haProdottiAssociati)
			{
				Categoria catVuota = "";

				//Azzero elemento
				CopyArray(arrCategorie[i], catVuota, L_NOME_CATEGORIA);

				//Salvo sul file
				fseek(flCategorie, i * sizeof(Categoria), SEEK_SET);
				fwrite(&catVuota, sizeof(Categoria), 1, flCategorie);

				//Se è l'ultimo elemento, diminuisco di uno la conta delle categorie
				if (i == NCategorie - 1)
				{
					NCategorie--;
				}
			}

			break;
		}
	}

	//Se ha prodotti associati sicuramente la categoria è stata trovata
	if (haProdottiAssociati)
	{
		printf("La categoria ha dei prodotti associati, pertanto non puo' essere eliminata\n");
	}
	else if (trovato)
	{
		printf("Categoria eliminata\n");
	}
	else
	{
		printf("Categoria non esistente\n");
	}

	system("pause");
}


bool CheckCategoria(Categoria categoria)
{
	bool esiste = false;

	for (int i = 0; i < NCategorie; i++)
	{
		//Se il nome della categoria è lo stesso di quello specificato...
		if (!strcmp(categoria, arrCategorie[i]))
		{
			esiste = true;
			break;
		}
	}

	return esiste;
}



bool ExistsOneCategoria()
{
	bool esiste = false;

	for (int i = 0; i < NCategorie; i++)
	{
		//Se il nome della categoria ha valore...
		if (strcmp(arrCategorie[i], ""))
		{
			esiste = true;
			break;
		}
	}

	return esiste;
}
