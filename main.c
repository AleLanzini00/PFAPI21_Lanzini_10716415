/*NOTE
Funziona con 30L
*/
#include <stdio.h>
#include <stdlib.h>
#define infinito 4294967295     //2^32 -1 = 4294967295

int heapsize;
int heapsizeC = -1;

struct nodo{
    int nome;
    unsigned long int dist;
    int prev;
    int posmheap;
};

struct Grafo{       //struct usata per la classifica
    int ID;
    unsigned long int distanza;
};

//GESTIONE MIN_HEAP

void min_heapify(struct nodo *A,struct nodo *G ,int posiz){
    int min;    //posizione del minimo
    int l = (posiz*2)+1;
    int r = (posiz*2)+2; //figli sx e dx
    if(l <= heapsize && (((A+l)->dist) < (A+posiz)->dist)){
        min = l;
    }
    else{
        min = posiz;
    }
    if(r <= heapsize && (A+r)->dist<(A+min)->dist) {
        min = r;
    }
    if(min!=posiz){     //scambio posiz con min
        struct nodo swap;
        //prima aggirono le poszioni, poi faccio lo scambio
        (G+(A+posiz)->nome)->posmheap = min; //aggiorno posizione scambiati
        (G+(A+min)->nome)->posmheap = posiz; //aggiorno posizione scambiati
        //scambio effettivo
        swap=*(A+posiz);
        *(A+posiz)=*(A+min);
        *(A+min)=swap;
        min_heapify(A,G,min);
    }
}

void heap_decrease_key(struct nodo* A,int posiz,struct nodo* node,struct nodo* G){
    //passo il nodo anziche solo la chiave
    if((node->dist) > ((A+posiz)->dist)){
        printf("\nERRORE NUOVA CHIAVE PIU GRANDE DELLA VECCHIA");
    }
    //A[posiz]=key (node)
    (A+posiz)->nome = node->nome;
    (A+posiz)->dist = node->dist;
    (A+posiz)->prev = node->prev;
    node->posmheap = posiz;
    //non c'è bisogno di aggiornare lo heapsize

    while(posiz>0 && ((A+((posiz-1)/2))->dist) > ((A+posiz)->dist)){
        //scambia A[i] con A[padrei]
        node->posmheap = (posiz-1)/2;   //aggiorno la posizione nel minheap del nodo
        (G+((A+((posiz-1)/2))->nome))->posmheap = posiz;   //aggriono posiz vecchio padre

        struct nodo swap;
        swap=*(A+posiz);
        *(A+posiz) = *(A+((posiz-1)/2));
        *(A+((posiz-1)/2)) = swap;
        //i=parent(i)
        posiz=((posiz-1)/2);
    }
}

void min_heap_insert(struct nodo *A,struct nodo* node,struct nodo *G){   //aggiunta di un nodo al minheap
    heapsize = heapsize + 1;
    ((A + heapsize)->dist) = infinito;   //A[A.heapsize] = infinito
    ((A + heapsize)->nome) = -1;
    ((A + heapsize)->prev) = -1;
    node->posmheap = heapsize;
    heap_decrease_key(A, heapsize, node,G);
}

struct nodo heap_extract_min(struct nodo *A, struct nodo *G){
    struct nodo minimo;
    if(heapsize < 0){
        printf("\nErrore underflow");
    }
    minimo = *A; //min=A[0];
    //poszione nulla all'elemento eliminato
    (G+(A->nome))->posmheap =-1;
    *A = *(A + heapsize);     //A[0]=[A.heapsize]
    //aggiorno poszione del nodo messo in testa
    (G+(A+heapsize)->nome)->posmheap=0;
    heapsize = heapsize - 1;

    min_heapify(A,G,0);
    return minimo;
}

void dijkstra(int dim, unsigned long int *matp[dim], struct nodo *A, struct nodo *G){
    //A: min heap , G:rappresentazione del grafo
    int i;
    int riga;
    struct nodo u;
    heapsize=-1;   //inizializzo Q=0
    // -1 perchè il vet inizia da zero, così le posizioni coincidono
    //gestisco nodo 0
    G->dist=0;
    G->prev=-1;     //S.PREV = NIL
    min_heap_insert(A,G,G); //inserisco nodo 0 nel MIN_HEAP (G = primo elemento del grafo)

    for(i=1;i<dim;i++){     //inizializzo tutti i nodi
        (G+i)->dist=infinito;       //dist infinita
        (G+i)->prev=-1;             //predecessore NIL
        min_heap_insert(A,(G+i),G);   //inserisco tutti i nodi nello heap
    }

    while(heapsize>-1){   //DUBBIO SU QUESTA CONDIZOONE
        u = heap_extract_min(A,G);  //estraggo nodo con dist minima
        riga = u.nome;  //per l'n-esimo nodo trovo gli adiacenti nell'n-esima riga
        for(i=0;i<dim;i++){
            if(i!=riga && *(matp[riga]+i)!=0){     //scorro gli adiacenti a U
                //G+i = i-esimo nodo, cioè nodo v
                if((G+i)->dist > u.dist + *(matp[riga]+i)){
                    (G+i)->dist = u.dist + *(matp[riga]+i);
                    (G+i)->prev = u.nome;
                    heap_decrease_key(A,(G+i)->posmheap,(G+i),G);
                }
            }
        }
    }
}

//GESTIONE CLASSIFICA CON MAXHEAP

void max_heapify(struct Grafo* A, int posiz){
    int massimo;    //posizione del massimo
    int l = (posiz*2)+1;
    int r = (posiz*2)+2; //figli sx e dx
    massimo = posiz;
    if( l <= heapsizeC && (((A+l)->distanza) > (A+posiz)->distanza)){
        massimo = l;
    }
    if( r <= heapsizeC && (((A+r)->distanza) > (A+massimo)->distanza)){
        massimo = r;
    }
    if(massimo != posiz){
        struct Grafo swap;
        swap=*(A+posiz);
        *(A+posiz)=*(A+massimo);
        *(A+massimo)=swap;
        max_heapify(A,massimo);
    }
}

void max_heap_insert(struct Grafo* A, struct Grafo* grafo){   //inserimento "in coda" con riordino
    //grafo: grafo da inserire
    //inserisco in coda e "risalgo"
    int posiz;
    heapsizeC = heapsizeC + 1;
    posiz = heapsizeC;
    (A+posiz)->ID = grafo->ID;
    (A+posiz)->distanza = grafo->distanza;
    while(posiz>0 && (((A+((posiz-1)/2))->distanza) < ((A+posiz)->distanza))){    //se il padre  è minore del figlio scambio
        struct Grafo swap;
        swap = *(A+((posiz-1)/2));
        *(A+((posiz-1)/2)) = *(A+posiz);
        *(A+posiz) = swap;
        posiz = (posiz-1)/2;
    }
}


void heap_change_max(struct Grafo *A, struct Grafo *new){
    //cambio valore al massimo
    *A = *new;
    //sistemo con heapify
    max_heapify(A,0);
}



int main() {
    int d,k;
    if(scanf("%d %d ",&d,&k)!=2){
        printf("\nErorre scanf");
    }   //leggo d e k

    struct nodo Q[d];       //min heap che conterrà i nodi, uso un vettore di struct
    struct nodo grafo[d];   //rappresenta il grafo inserito
    unsigned long int matrice[d][d];      //matrice per salvare l'input
    unsigned long int* matpointer[d];

    int maxdim = (d*10)+(d-1); //2^32=4.294.967.295, quindi 10 cifre per arco, più d-1 virgole
    char s[maxdim];
    char *res;
    struct Grafo classifica[k]; //la classifica è un vettore di struct
    struct Grafo letto;
    int i,c;
    int id=0;
    char* ptr;

    //leggo un comando Topk o AggiungiGrafo
    res=fgets(s,15,stdin);
    while(res!=NULL){
        if(s[0]=='A') {     //AggiungiGrafo

            //leggo la matrice per righe
            for(i=0;i<d;i++){
                if(fgets(s,maxdim+1,stdin)!=NULL) {     //leggo d righe
                    for (c = 0; c < d; c++) {
                        if(c==0){
                            matrice[i][c] = strtoul(s, &ptr, 10);
                        }
                        else
                            matrice[i][c] = strtoul(ptr+1,&ptr,10);
                    }
                    matpointer[i] = &matrice[i][0];   //inizializzo punt a matrice per ogni riga
                }
            }

            //init min_heap e grafo:
            for (i = 0; i < d; i++) {
                grafo[i].nome = i;                //do il nome a ogni nodo
                grafo[i].dist = 0;
                grafo[i].prev = -1;
            }
            //uso l'algoritmo di Dijkstra
            dijkstra(d, matpointer, Q, grafo);

            letto.ID=id;
            letto.distanza = 0;  //somma delle distanze
            for (i = 0; i < d; i++) {
                if (grafo[i].dist == infinito) {
                    grafo[i].dist = 0;    //gestisce nodi non collegati
                }
                letto.distanza = letto.distanza + grafo[i].dist;
            }


            //GESTIONE CLASSIFICA
            if(id<k){   //inizializzo la classifica con i primi K grafi
                max_heap_insert(classifica,&letto);
            }
            else{
                if(classifica[0].distanza > letto.distanza){
                    heap_change_max(classifica, &letto);
                }
            }
            //nuovo input
            id++;
            res=fgets(s,15,stdin);
        }

        if(s[0]=='T'){
            //stampa la classifica
            if(id<k){
                for(i=0;i<id;i++){
                    if(i!=id-1) {
                        printf("%d ", classifica[i].ID);
                    }
                    else{
                        printf("%d", classifica[i].ID);
                    }
                }
            }
            else {
                for (i = 0; i < k; i++) {
                    if(i!=k-1){
                        printf("%d ", classifica[i].ID);
                    }
                    else {
                        printf("%d", classifica[i].ID);
                    }
                }
            }
            printf("\n");
            res=fgets(s,15,stdin);
        }
    }
    return 0;
}
