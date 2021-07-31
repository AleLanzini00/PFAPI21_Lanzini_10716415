/*NOTE
 * non funziona (CODICE DA RIVEDERE)
 * rivedere se trovapos è necessario (consuma tempo)
 * PASSARE MATRICE CON PUNT
*/
#include <stdio.h>
#include <stdlib.h>
#define infinito 4000000

struct nodo{
    int nome;
    int dist;
    int prev;
    int* heapsizep;
};

int trovapos(struct nodo *A, int dim,int nomenodo){     //ricerca per nome del nodo e ne restituisce la posizione
    int i=0;
    int posizione=0;
    while(i<dim){
        if((A+i)->nome == nomenodo){
            posizione = i;
            i = dim+1;    //termino il ciclo
        }
        else
            i++;
    }
    return posizione;
}

//gestione di un MIN_HEAP
int heapsize;
void min_heapify(struct nodo * A, int posiz){
    int min;
    int l=posiz*2+1;
    int r=posiz*2+2; //figli sx e dx
    if(l<*(A->heapsizep) && (A+l)->dist<(A+posiz)->dist){
        min=l;
    }
    else{
        min=posiz;
    }
    if(r<*(A->heapsizep) && (A+l)->dist<(A+min)->dist) {
        min = r;
    }
    if(min!=posiz){
        struct nodo swap;
        swap=*(A+posiz);
        *(A+posiz)=*(A+min);
        *(A+min)=swap;
    }
}
/*
  void build_min_heap(struct nodo* A, int length){
    int i;
    *(A->heapsizep)=length;
    for(i=length/2;i--;i=0){
        min_heapify(A,i);
    }
}
*/
void heap_decrease_key(struct nodo* A,int posiz,struct nodo* node){
    if(node->dist > (A+posiz)->dist){
        printf("\nERRORE NUOVA CHIAVE PIU GRANDE DELLA VECCHIA");
    }
    *(A+posiz)=*node;
    while(posiz>0 && (A+((posiz-1)/2))->dist > (A+posiz)->dist){
        //scambia A[i] con A[padrei]
        struct nodo swap;
        swap=*(A+posiz);
        *(A+posiz)=*(A+((posiz-1)/2));
        *(A+((posiz-1)/2))=swap;
        //i=parenti(i)
        posiz=((posiz-1)/2);
    }
}

void min_heap_insert(struct nodo *A,struct nodo* node){   //aggiunta di un nodo al minheap
    *(A->heapsizep)=*(A->heapsizep)+1;
    ((A+*(A->heapsizep))->dist)=infinito;   //A[A.heapsize]=infinito
    heap_decrease_key(A,*(A->heapsizep),node);
}

struct nodo heap_extract_min(struct nodo *A){   //DA FARE
    struct nodo min;
    if(*(A->heapsizep)>0){
        printf("\nErrore underflow");
    }
    min=*A; //min=A[0];
    *A=*(A+*(A->heapsizep));     //A[0]=[A.heapsize]
    *(A->heapsizep)=*(A->heapsizep)-1;
    min_heapify(A,0);
    return min;
}

void dijkstra(int dim, int mat[dim][dim], struct nodo *A, struct nodo *G){  //DA FARE
    //A: min heap  G:rappresentazione del grafo
    int i;
    int riga;
    struct nodo u;
    *(A->heapsizep)=0;   //Q=0
    G->dist=0;
    G->prev=-1;
    min_heap_insert(A,G); //passo la struttura dati e il nodo zero
    for(i=1;i<dim;i++){
        (G+i)->dist=infinito;
        (G+i)->prev=-1;     //inserisco tutti i nodi nello heap
        min_heap_insert(A,(G+i));
    }
    while(*(A->heapsizep)>0){
        u=heap_extract_min(A);  //estraggo nodo con dist minima
        riga=u.nome;
        int pos;
        for(i=0;i<dim;i++){
            if(i!=riga && mat[riga][i]!=0){     //scorro gli adiacenti a U
                if((G+i)->dist > u.dist + mat[riga][i]){
                    (G+i)->dist = u.dist + mat[riga][i];
                    (G+i)->prev = u.nome;
                    pos=trovapos(A,dim,(G+i)->nome);    //potrebbe essere migliorabile
                    heap_decrease_key(A,pos,(G+i));
                }
            }
        }
    }
}

int main(){
    int d=3;
    struct nodo Q[d]; //min heap che conterrà i nodi, uso un vettore di struct
    struct nodo grafo[d];
    int matrice[d][d];
    int i,c;
    for(i=0;i<d;i++){
        for(c=0;c<d;c++) {
            scanf("\n%d", &matrice[i][c]);
        }
    }
    //init min_heap
    for(i=0;i<d;i++){
        Q[i].heapsizep=&heapsize;   //in ogni nodo tengo un puntatore alla varaibile heapsize
        grafo[i].nome=i;
        grafo[i].dist=-1;
        grafo[i].prev=-1;
    }
    dijkstra(d,matrice,Q,grafo);
    //stampa di prova
    int tot=0;
    for(i=0;i<d;i++){
        printf("\nDistanza nodo %d: %d",i,grafo[i].dist);
        tot=tot+grafo[i].dist;
    }
    printf("\nDISTANZA TOT: %d",tot);
}