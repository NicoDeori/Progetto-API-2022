#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Definizione nodo
struct genericNode
{
    bool OnOFF, DXOn, SXOn;
    char word[25];
    struct genericNode *up, *right, *left;
};

// Definizione tree
typedef struct
{
    struct genericNode *nil, *root;
} tree;

// Funzione compare manuale
int compare ( char * str1, char * str2) {
    while(*str1 == *str2) {
        if(*str1 == '\0')
            break;
        if(*str1 == '\n')
            break;
        if(*str2 == '\0')
            break;
        if(*str2 == '\n')
            break;
        str1++;
        str2++;
    }
    return *str1 - *str2;
}


// Leggi da input k caratteri
char * read(char * input, int k){
    int i;
    for(i = 0; i < k; i++){
        if(input[i] == '\n') {
            input[i] = '\0';
            break;
        }
    }
    return input;
}

// Funzioni gestione tree
// Creazione nodo di chiave string, tutti i bit sono settatti ON di default
struct genericNode * createNode (char * string, int k) {
    struct genericNode * x = calloc(1, sizeof(struct genericNode));
    strncpy(x->word, string, k);
    x->OnOFF = true;
    x->DXOn = true;
    x->SXOn = true;
    return x;
}

// Creazione albero tramite root e nil
tree * createTree () {
    tree * tree_t = calloc (1, sizeof(tree));
    tree_t->root = calloc(1, sizeof (struct genericNode));
    tree_t->nil = tree_t->root;
    tree_t->nil->left = tree_t->nil;
    tree_t->nil->right = tree_t->nil;
    tree_t->nil->up = tree_t->nil;
    return tree_t;
}

// Dealloca la memoria occupata da ogni nodo dell'albero escluso il nil. Non dealloca la struttura dell'albero
void freeAlbero(struct genericNode * node, struct genericNode * nil){
    if(node != nil){
        freeAlbero(node->left, nil);
        freeAlbero(node->right, nil);
        free(node);
    }
}

// Ricerca la parola string nell'albero e restituisce il nodo corrispondente se la parola è contenuta, NULL altrimenti
struct genericNode * ricerca (struct genericNode * node, char * string, struct genericNode * nil) {
    if(compare(node->word, string) == 0)
        return node;
    else if (compare(node->word, string) > 0 && node->left != nil)
        return ricerca(node->left, string, nil);
    else if (compare(node->word, string) < 0 && node->right != nil)
        return ricerca(node->right, string, nil);
    else
        return NULL;
}

// Crea un nodo contenente la parola string e lo inserisce nel tree in posizione corretta e con il bit corretto.
void insert (tree * tree, char * string, bool bit, int k) {
    struct genericNode * z = createNode(string, k);
    z->OnOFF = bit;
    struct genericNode * y, * x;
    y = tree->nil;
    x = tree->root;
    // Inserimento con bit ON
    if(bit) {
        while (x != tree->nil) {
            y = x;
            if (compare(z->word, x->word) < 0) {
                x->SXOn = true;
                x = x->left;
            } else {
                x->DXOn = true;
                x = x->right;
            }
        }
    }
        // Inserimento con bit OFF
    else{
        while (x != tree->nil) {
            y = x;
            if (compare(z->word, x->word) < 0)
                x = x->left;
            else
                x = x->right;
        }
    }
    z->up = y;
    // Sistema la struttura dell'albero tramite il nodo nil
    // Verifica se l'albero è vuoto
    if(y == tree->nil)
        tree->root = z;
    else if(compare(z->word, y->word ) < 0)
        y->left = z;
    else
        y->right = z;
    z->left = tree->nil;
    z->right = tree->nil;
}

// Assegna il valore ON a tutti i bit dei nodi dell'albero tranne il nil
void rendiAlberoOn (struct genericNode * nil, struct genericNode * x) {
    if (x != nil) {
        rendiAlberoOn(nil, x->left);
        rendiAlberoOn(nil, x->right);
        x->OnOFF = true;
        x->DXOn = true;
        x->SXOn = true;
        if(x->left == nil)
            x->SXOn = false;
        if(x->right == nil)
            x->DXOn = false;
    }
}

// Funzioni dizionario
// Comando stampa filtrate
void stampa_filtrate (struct genericNode * x, struct genericNode * nil) {
    if (x != nil) {
        if (x->SXOn == true)
            stampa_filtrate(x->left, nil);
        if (x->OnOFF == true)
            printf("%s\n", x->word);
        if (x->DXOn == true)
            stampa_filtrate(x->right, nil);
    }
}

// Conteggio parole presenti nel dizionario con il bit ON
int contaparole (struct genericNode * x) {
    int k = 0;
    if (x->OnOFF == true)
        k++;
    if (x->SXOn == true)
        k  = k + contaparole(x->left);
    if (x->DXOn == true)
        k = k + contaparole(x->right);
    return k;
}

// Filtra le parole non compatibili con i vincoli dell'ultima parola inserita
void filtra(char * vettSegniParolaInserita, char * tentativo, int lunghezza, char * carMin, int * min, int * isMax, struct genericNode * nil, struct genericNode * node) {
    if (node != nil) {
        if(node->SXOn == true)
            filtra(vettSegniParolaInserita, tentativo, lunghezza, carMin, min, isMax, nil, node->left);
        if(node->DXOn == true)
            filtra(vettSegniParolaInserita, tentativo, lunghezza, carMin, min, isMax, nil, node->right);
        if (node->OnOFF == true) {
            int i, j, l, occorrenze, notBan;
            // Controllo caso +
            for(i = 0; i < lunghezza; i++){
                if(vettSegniParolaInserita[i] == '+' && tentativo[i] != node->word[i]){
                    node->OnOFF = false;
                    if(node->DXOn == false && node->SXOn == false && node->OnOFF == false){
                        if(node->up->left == node)
                            node->up->SXOn = false;
                        else
                            node->up->DXOn = false;
                    }
                    return;
                }
            }
            // Controllo caso | e /, il carattere non è ammissibile in questa posizione
            for(i = 0; i < lunghezza; i++){
                if((vettSegniParolaInserita[i] == '|' || vettSegniParolaInserita[i] == '/') && tentativo[i] == node->word[i]) {
                        node->OnOFF = false;
                        if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                            if (node->up->left == node)
                                node->up->SXOn = false;
                            else
                                node->up->DXOn = false;
                        }
                        return;
                    }
            }
            // Controllo caso / per capire se il carattere è bannato
            for(i = 0; i < lunghezza; i++){
                if(vettSegniParolaInserita[i] == '/') {
                    // Controllo se il carattere è presente nel nodo
                    for (j = 0; j < lunghezza; j++) {
                        if(tentativo[i] == node->word[j]) {
                            notBan = 0;            // Cambio il valore a 1 se scopro che il carattere non è bannato
                            // Controllo se è presente in carMin, in modo da capire se il carattere è bannato
                            for (l = 0; carMin[l] != '/' && l < lunghezza; l++) {
                                if (carMin[l] == tentativo[i]) {
                                    notBan = 1;
                                    break;
                                }
                            }
                            // Il carattere non è presente in carMin, quindi è un carattere bannato
                            if (notBan == 0) {
                                node->OnOFF = false;
                                if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                                    if (node->up->left == node)
                                        node->up->SXOn = false;
                                    else
                                        node->up->DXOn = false;
                                }
                                return;
                            }
                            break;
                        }
                    }
                }
            }
            // Controllo che il nodo rispetti i vincoli max e min
            for(i = 0; carMin[i] != '/' && i < lunghezza; i++){
                occorrenze = 0;
                for(j = 0; j < lunghezza; j++){
                    if(carMin[i] == node->word[j])
                        occorrenze++;
                }
                // Caso in cui conosca il minimo
                if(isMax[i] == 0 && occorrenze < min[i]){
                    node->OnOFF = false;
                    if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                        if (node->up->left == node)
                            node->up->SXOn = false;
                        else
                            node->up->DXOn = false;
                    }
                    return;
                }
                // Caso in cui conosca il max
                else if(isMax[i] == 1 && occorrenze != min[i]){
                    node->OnOFF = false;
                    if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                        if (node->up->left == node)
                            node->up->SXOn = false;
                        else
                            node->up->DXOn = false;
                    }
                    return;
                }
            }
        }
        if(node->DXOn == false && node->SXOn == false && node->OnOFF == false){
            if(node->up->left == node)
                node->up->SXOn = false;
            else
                node->up->DXOn = false;
        }
    }
}

//Filtra e fixa l'albero all'inizio di una partita e/o dopo un inserimento avvenuto durante una partita
void filtraPrimoTentativo(char * vettSegniParolaInserita, char * tentativo, int lunghezza, char * carMin, int * min, int * isMax, struct genericNode * nil, struct genericNode * node){
    if(node != nil) {
        filtraPrimoTentativo(vettSegniParolaInserita, tentativo, lunghezza, carMin, min, isMax, nil, node->left);
        filtraPrimoTentativo(vettSegniParolaInserita, tentativo, lunghezza, carMin, min, isMax, nil, node->right);
        // Sistema DXOn e SXOn
        if (node->right == nil)
            node->DXOn = false;
        else if (node->right->OnOFF == false && node->right->DXOn == false && node->right->SXOn == false)
            node->DXOn = false;
        else
            node->DXOn = true;
        if (node->left == nil)
            node->SXOn = false;
        else if (node->left->OnOFF == false && node->left->DXOn == false && node->left->SXOn == false)
            node->SXOn = false;
        else
            node->SXOn = true;
        int i, j, l, occorrenze, notBan;
        // Controllo caso +
        for(i = 0; i < lunghezza; i++){
            if(vettSegniParolaInserita[i] == '+' && tentativo[i] != node->word[i]){
                node->OnOFF = false;
                if(node->DXOn == false && node->SXOn == false && node->OnOFF == false){
                    if(node->up->left == node)
                        node->up->SXOn = false;
                    else
                        node->up->DXOn = false;
                }
                return;
            }
        }
        // Controllo caso | e /, il carattere non è ammissibile in questa posizione
        for(i = 0; i < lunghezza; i++){
            if((vettSegniParolaInserita[i] == '|' || vettSegniParolaInserita[i] == '/') && tentativo[i] == node->word[i]) {
                node->OnOFF = false;
                if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                    if (node->up->left == node)
                        node->up->SXOn = false;
                    else
                        node->up->DXOn = false;
                }
                return;
            }
        }
        // Controllo caso / per capire se il carattere è bannato
        for(i = 0; i < lunghezza; i++){
            if(vettSegniParolaInserita[i] == '/') {
                // Controllo se il carattere è presente nel nodo
                for (j = 0; j < lunghezza; j++) {
                    if(tentativo[i] == node->word[j]) {
                        notBan = 0;            // Cambio il valore a 1 se scopro che il carattere non è bannato
                        // Controllo se è presente in carMin, in modo da capire se il carattere è bannato
                        for (l = 0; carMin[l] != '/' && l < lunghezza; l++) {
                            if (carMin[l] == tentativo[i]) {
                                notBan = 1;
                                break;
                            }
                        }
                        // Il carattere non è presente in carMin, quindi è un carattere bannato
                        if (notBan == 0) {
                            node->OnOFF = false;
                            if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                                if (node->up->left == node)
                                    node->up->SXOn = false;
                                else
                                    node->up->DXOn = false;
                            }
                            return;
                        }
                        break;
                    }
                }
            }
        }
        // Controllo che il nodo rispetti i vincoli max e min
        for(i = 0; carMin[i] != '/' && i < lunghezza; i++){
            occorrenze = 0;
            for(j = 0; j < lunghezza; j++){
                if(carMin[i] == node->word[j])
                    occorrenze++;
            }
            // Caso in cui conosca il minimo
            if(isMax[i] == 0 && occorrenze < min[i]){
                node->OnOFF = false;
                if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                    if (node->up->left == node)
                        node->up->SXOn = false;
                    else
                        node->up->DXOn = false;
                }
                return;
            }
            // Caso in cui conosca il max
            if(isMax[i] == 1 && occorrenze != min[i]){
                node->OnOFF = false;
                if (node->DXOn == false && node->SXOn == false && node->OnOFF == false) {
                    if (node->up->left == node)
                        node->up->SXOn = false;
                    else
                        node->up->DXOn = false;
                }
                return;
            }
        }
        node->OnOFF = true;
    }
}


// Stampa a schermo il risultato del confronto tra il tentativo e il riferimento
void stampaRisultato(char * tentativo, int lunghezza, char * riferimento, tree * dizionario, char * guessedWord, char ** caratteriProibiti, char * banCaratteri, bool primoTentativo, int * isMax, char * carMin, int * min) {
    int i, j, l, ban, occorrenze;
    int bit[lunghezza];
    char vettSegniParolaIns[lunghezza];
    // Set up valori
    for(i = 0; i < lunghezza; i++) {
        vettSegniParolaIns[i] = '/';
        bit[i] = 0;
    }
    for(i = 0; i < lunghezza; i++){
        // Caso +, salvo i caratteri in guessedWord[]
        if(tentativo[i] == riferimento[i]) {
            vettSegniParolaIns[i] = '+';
            guessedWord[i] = riferimento[i];
            bit[i] = 1;
        }
        // Caso |
        else{
            for(j = 0; j < lunghezza; j++){
                if(tentativo[i] == riferimento[j] && riferimento[j] != tentativo[j] && bit[j] == 0){
                    bit[j] = 1;
                    vettSegniParolaIns[i] = '|';
                    // Aggiungo il carattere a caratteriProibiti[][] per quella posizione se non è presente
                    for(l = 0; l < lunghezza; l ++){
                        if(caratteriProibiti[i][l] == tentativo[i])
                            break;
                        else if(caratteriProibiti[i][l] == '/'){
                            caratteriProibiti[i][l] = tentativo[i];
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    // Calcolo il minimo di occorrenze noto
    for(i = 0; i < lunghezza; i++){
        if(vettSegniParolaIns[i] == '+' || vettSegniParolaIns[i] == '|'){
            occorrenze = 0;
            for(j = 0; j < lunghezza; j++){
                if(tentativo[i] == tentativo[j] && (vettSegniParolaIns[j] == '+' || vettSegniParolaIns[j] == '|'))
                    occorrenze++;
            }
            // Se ho scoperto un nuovo minimo per una lettera aggiorno min[]
            for(j = 0; j < lunghezza; j ++){
                if(carMin[j] == tentativo[i]){
                    if(occorrenze > min[j])
                        min[j] = occorrenze;
                    break;
                } else if(carMin[j] == '/'){
                    min[j] = occorrenze;
                    carMin[j] = tentativo[i];
                    break;
                }
            }
        }
    }
    // Capisco il significato di / in vettSegniParolaIns e salvo l'eventuale nuovo massimo
    for(i = 0; i < lunghezza; i++) {
        if (vettSegniParolaIns[i] == '/') {
            ban = 0;
            for (j = 0; j < lunghezza; j++) {
                if (carMin[j] == tentativo[i]) {
                    // Ho capito che minimo e massimo corrispondono
                    isMax[j] = 1;
                    ban = 1;
                    // Ho anche capito che il carattere non può apparire in posizione i
                    for (l = 0; l < lunghezza; l++) {
                        if (caratteriProibiti[i][l] == tentativo[i])
                            break;
                        else if (caratteriProibiti[i][l] == '/') {
                            caratteriProibiti[i][l] = tentativo[i];
                            break;
                        }
                    }
                    break;
                }
            }
            // Ban resta a 0 se il carattere non è presente in carMin[] e quindi deve essere bannato
            if (ban == 0) {
                for (j = 0; j < 65; j++) {
                    // Caso già bannato
                    if (banCaratteri[j] == tentativo[i])
                        break;
                    // Caso non ancora bannato
                    else if (banCaratteri[j] == '/') {
                        banCaratteri[j] = tentativo[i];
                        break;
                    }
                }
            }
        }
    }
    // Stampo la stringa e filtro
    for (i = 0; i < lunghezza; i++)
        printf("%c", vettSegniParolaIns[i]);
    printf("\n");
    if(primoTentativo == false)
        filtra(vettSegniParolaIns, tentativo, lunghezza, carMin, min,  isMax, dizionario->nil, dizionario->root);
    else
        filtraPrimoTentativo(vettSegniParolaIns, tentativo, lunghezza, carMin, min, isMax, dizionario->nil, dizionario->root);
}

// Inserimento tramite il comando +inserisci_inizio nel corso di una partita
void inserimentoFiltrato(tree * dizionario, int lunghezza, char * parolaInserita, char * guessedWord, char * banCaratteri, char ** caratteriProibiti, int * isMax, char * carMin, int * min){
    int i, j, occorrenze;
    // Controllo guessedWord
    for(i = 0; i < lunghezza; i++){
        if(guessedWord[i] != '/' && guessedWord[i] != parolaInserita[i]){
            insert(dizionario, parolaInserita, false, lunghezza);
            return;
        }
    }
    // Controllo banCaratteri
    for(i = 0; i < lunghezza; i++){
        for(j = 0; banCaratteri[j] != '/'; j++){
            if(parolaInserita[i] == banCaratteri[j]){
                insert(dizionario, parolaInserita, false, lunghezza);
                return;
            }
        }
    }
    // Controllo caratteriProibiti
    for(i = 0; i < lunghezza; i++){
        for(j = 0; caratteriProibiti[i][j] != '/'; j++){
            if(caratteriProibiti[i][j] == parolaInserita[i]){
                insert(dizionario, parolaInserita, false, lunghezza);
                return;
            }
        }
    }
    // Controllo min e max
    for(i = 0; carMin[i] != '/' && i < lunghezza; i++){
        occorrenze = 0;
        for(j = 0; j < lunghezza; j++){
            if(carMin[i] == parolaInserita[j])
                occorrenze++;
        }
        // Caso in cui conosca il minimo
        if(isMax[i] == 0 && occorrenze < min[i]){
            insert(dizionario, parolaInserita, false, lunghezza);
            return;
        }
        // Caso in cui conosca il max
        if(isMax[i] == 1 && occorrenze != min[i]){
            insert(dizionario, parolaInserita, false, lunghezza);
            return;
        }
    }
    insert(dizionario, parolaInserita, true, lunghezza);
}

// Parse input
void parse(int lunghezza){
    // Dichiarazione
    char * string = calloc(25, sizeof (char));
    char * riferimento = calloc(25, sizeof (char));
    tree * dizionario = createTree();
    int tentativi = 0, i, j;
    bool partita = false, primoTentativo = false;

    // Variabili utilizzate per salvare i vincoli ottenuti dai tentativi
    char * guessedWord = calloc(25, sizeof (char));                       // Caratteri indovinati al posto giusto
    char * banCaratteri = calloc(65, sizeof(char));                       // Caratteri non presenti nel riferimento
    char ** caratteriProibiti = calloc(lunghezza + 1, sizeof(char *));    // Caratteri non presenti all'iesima posizione
    for(i = 0; i < lunghezza; i++)
        caratteriProibiti[i] = calloc(lunghezza, sizeof(char));
    int * isMax = calloc(lunghezza + 1, sizeof(int));                     // Bit messi a zero se conosco il min, messi a 1 se ho il max
    char * carMin = calloc(lunghezza + 1, sizeof(char));                  // Caratteri di cui conosco il numero minimo di occorrenze nella parola di riferimento
    int * min = calloc(lunghezza + 1, sizeof(int));                       // Numero di occorrenze corrispondenti ai caratteri di carMin

    // Gestione input
    while (fgets(string, 25, stdin) != NULL){
        read(string, 25);
        // Comando nuova partita
        if (compare(string, "+nuova_partita" ) == 0){
            partita = true;
            primoTentativo = true;
            // Salvataggio parola di riferimento
            if(fgets(string, 25, stdin) != NULL) {
                read(string, 25);
                strncpy(riferimento, string, lunghezza);
            }
            // Salvataggio numero di tentativi
            if(fscanf(stdin, "%d\n", &tentativi) == 1) {
                if (tentativi == 0) {
                    printf("ko\n");
                    partita = false;
                }
            }
            // Setting vettori per il salvataggio dei vincoli
            for(i = 0; i < lunghezza; i++) {
                guessedWord[i] = '/';
                isMax[i] = 0;
                carMin[i] = '/';
                min[i] = 0;
                for(j = 0; j < lunghezza; j++)
                    caratteriProibiti[i][j] = '/';
            }
            for(i = 0; i < 65; i++)
                banCaratteri[i] = '/';
        }
            // Comando stampa filtrate
        else if(compare(string, "+stampa_filtrate" ) == 0){
            if(primoTentativo == true || partita == false)
                rendiAlberoOn(dizionario->nil, dizionario->root);
            stampa_filtrate(dizionario->root, dizionario->nil);
            primoTentativo = false;
        }
            // Comando inserisci inizio
        else if(compare(string, "+inserisci_inizio" ) == 0) {
            while (compare(read(fgets(string, 25, stdin), 25), "+inserisci_fine") != 0) {
                if (partita == false)
                    insert(dizionario, string, true, lunghezza);
                else
                    inserimentoFiltrato(dizionario, lunghezza, string, guessedWord, banCaratteri, caratteriProibiti, isMax, carMin, min);
            }
            if(partita == false)
                primoTentativo = true;
        }
            // Inserimento parole nel dizionario prima della partita
        else if(partita == false){
            insert(dizionario, string, true, lunghezza);
        }
            // Gestione tentativi
        else{
            // Il tentativo è corretto
            if (compare(string, riferimento) == 0){
                partita = false;
                printf("ok\n");
            }
                // Il tentativo non è presente nel dizionario
            else if(ricerca(dizionario->root, string, dizionario->nil) == NULL) {
                printf("not_exists\n");
            }
                // Tentativo sbagliato, filtro il dizionario con le nuove informazioni e riduco il numero di tentativi
            else{
                stampaRisultato(string, lunghezza, riferimento, dizionario, guessedWord, caratteriProibiti, banCaratteri, primoTentativo, isMax, carMin, min);
                primoTentativo = false;
                printf("%d\n", contaparole(dizionario->root));
                tentativi--;
                // Se i tentativi sono esauriti la partita termina
                if(tentativi == 0){
                    partita = false;
                    printf("ko\n");
                }
            }
        }
    }
    for(i = 0; i < lunghezza; i++)
        free(caratteriProibiti[i]);
    free(caratteriProibiti);
    freeAlbero(dizionario->root, dizionario->nil);
    free(dizionario->nil);
    free(dizionario);
    free(string);
    free(riferimento);
    free(guessedWord);
    free(isMax);
    free(banCaratteri);
    free(min);
    free(carMin);
}

int main(int argc,char* argv[]){
    int lunghezza;
    if (fscanf(stdin, "%d\n", &lunghezza) == 1)
        parse(lunghezza);
    return 0;
}