struct DVD {
    int identificativo_film; // un intero tra 1 e 3
    int identificativo_copia; // un intero tra 1 e 2
    int stato; // DISPONIBILE=0, oppure AFFITTATO=1
};


struct Monitor{
    struct DVD dvd[6]; // vettore di DVD da gestire
};


int affitta(struct Monitor * m, int id_film);
void restituisci(struct Monitor * m, int id_film, int id_copia);
void stampa(struct Monitor * m);