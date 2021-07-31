/*NOTE
 * Algortimo da solo funzionava su grAafi piccoli
 * ancora da testare su grandi grafi e più grafi
 * nessun test di input fatto (possibili errori di passaggio dati/lettura) CONTROLLARE
 */
#include <stdio.h>
#include<stdlib.h>
#define infinito 4000000

struct nodo{
    int nome;
    int dist;
    int prev;
    int* heapsizep;
};

struct Grafo{
    int ID;
    int distanza;
};

//FUNZIONI UTILI
int numcifre(int n){
    int cifre=1;
    int potenza=1;
    while(1) {
        if (n/potenza < 10)
            return cifre;
        else{
            cifre++;
            potenza=potenza*10;
        }
    }
}

int trovamassimo(struct Grafo vet[],int dim){   //restituisce la posizione del massimo
    int maxp=0;
    int maxd=-1;
    int i;
    for(i=0;i<dim;i++){
        if(vet[i].distanza>maxd){
            maxp=i;
            maxd=vet[i].distanza;
        }
    }
    return maxp;
}

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

//GESTIONE MIN_HEAP
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
    *(A->heapsizep) = *(A->heapsizep) + 1;
    //printf("\nProva: valore heapsize in min_heap_insert: %d",*(A->heapsizep));
    ((A + *(A->heapsizep))->dist) = infinito;   //A[A.heapsize] = infinito
    ((A + *(A->heapsizep))->nome) = -1;
    ((A + *(A->heapsizep))->prev) = -1;
    //printf("\nHeapsize fine insert: %d",heapsize);
    heap_decrease_key(A,*(A->heapsizep),node);
}

struct nodo heap_extract_min(struct nodo *A){
    struct nodo min;
    if(*(A->heapsizep)<0){
        printf("\nErrore underflow");
    }
    min=*A; //min=A[0];
    *A=*(A+*(A->heapsizep));     //A[0]=[A.heapsize]
    *(A->heapsizep)=*(A->heapsizep)-1;
    min_heapify(A,0);
    return min;
}

void dijkstra(int dim, int mat[dim][dim], struct nodo *A, struct nodo *G){
    //A: min heap  G:rappresentazione del grafo
    int i;
    int riga;
    struct nodo u;
    *(A->heapsizep)=0;   //inizializzo Q=0
    G->dist=0;
    G->prev=-1;     //S.PREV = NIL
    min_heap_insert(A,G); //inserisco nodo 0 nel MIN_HEAP
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


int main() {
    int d,k,c,i,posiz;
    int tot;    //contiene la distanza del grafo (matrice)
    int maxdist=0;
    int id=0;   //usato per contare i grafi inseriti, e quindi ne restituisce l'id
    int in=0;   //per rimuovere warning scanf
    //printf("Inserire d e k:\n");
    in=scanf("%d %d",&d,&k);   //leggo d e k
    int maxdim=(d*10)+(d-1); //2^32=4.294.967.295, quindi 10 cifre per arco, più d-1 virgole
    char s[maxdim];
    struct Grafo classifica[k]; //la classifica è un vettore di struct
    int matrice[d][d];  //matrice per salvare input
    struct nodo Q[d]; //min heap che conterrà i nodi, uso un vettore di struct
    struct nodo grafo[d];   //contengo i nodi

    //leggo un comando
    //printf("Inserire un comando (Aggiungigrafo o TopK):\n");
    in=scanf("%s",s);
    while(s[0]=='A' || s[0]=='T'){
        if(s[0]=='A'){//AggiungiGrafo
            //leggo la matrice  per righe
            for(c=0;c<d;c++){
                in=scanf("%s",s);
                posiz=0;    //posiz tiene conto della posizone del carattere nella stringa
                for(i=0;i<d;i++){
                    matrice[c][i]=atoi(&s[posiz]);
                    posiz=posiz+numcifre(matrice[c][i])+1;
                }
            }
            //stampo la matrice per verifica
            /*int riga=0;
            int col=0;
             printf("Stampa di controllo\n");
            for(riga=0;riga<d;riga++){
                for(col=0;col<d;col++){
                    printf("\t%d",matrice[riga][col]);
                }
                printf("\n");
            }*/
            //CALCOLO DIST E GESTISCO TOPK
            //init min_heap
            for(i=0;i<d;i++){
                Q[i].heapsizep = &heapsize;   //in ogni nodo tengo un puntatore alla varaibile heapsize
                grafo[i].nome=i;
                grafo[i].dist=-1;
                grafo[i].prev=-1;
                grafo[i].heapsizep=&heapsize;
            }
            //uso l'algoritmo di Dijkstra
            dijkstra(d,matrice,Q,grafo);
            //stampa di prova
            tot=0;
            for(i=0;i<d;i++){
                if(grafo[i].dist==infinito){
                    grafo[i].dist=0;    //gestisce nodi non collegati
                }
                printf("\nDistanza nodo %d: %d",i,grafo[i].dist);
                tot=tot+grafo[i].dist;
            }
            printf("\nDISTANZA TOT: %d",tot);
            if(id<k){   //inizializzo la classifica con i primi K grafi
                classifica[id].ID=id;
                classifica[id].distanza=tot;
                if(id==k-1){
                    maxdist=trovamassimo(classifica,k); //calcolo il massimo solo una volta riempito il vettore, prima inserisco sempre
                }

            }
            else{
                if(tot<classifica[maxdist].distanza){
                    //sostituisco il maxid
                    classifica[maxdist].ID=id;
                    classifica[maxdist].distanza=tot;
                    maxdist=trovamassimo(classifica,k); //ricalcolo il peggiore
                }
            }
            //nuovo input
            id++;
            in=scanf("%s",s);
        }
        if(s[0]=='T'){  //TOPK
            //STAMPA LA CLASSIFICA
            for(i=0;i<id;i++){
                printf("%d ",classifica[i].ID);
            }
            printf("\n");
            in=scanf("%s",s);
            if(s[0]=='T'){
                break;      //NON PUO LEGGERE DUE TOPK CONSECUTIVI
            }
        }
    }
    if(in==7){

    } //per eliminare warning su scanf
}