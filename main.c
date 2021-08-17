/*NOTE
 * non puo leggere 2 topk consecutivi
 * prev è imutile?
 *  FORSE MEGLIO INIT Q IN DJIKSTRA E NON IN MAIN
 * rivedere infinito
 * rivedere se trovapos è necessario (consuma tempo)
 * PASSARE MATRICE CON PUNT
*/
#include <stdio.h>
#include <stdlib.h>
#define infinito 4000000
int heapsize;

struct nodo{
    int nome;
    int dist;
    int prev;
    //int* heapsizep;
};

struct Grafo{       //struct usata per la classifica
    int ID;
    int distanza;
};

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


void min_heapify(struct nodo *A, int posiz){
    int min;
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
    if(min!=posiz){
        struct nodo swap;
        swap=*(A+posiz);
        *(A+posiz)=*(A+min);
        *(A+min)=swap;
        min_heapify(A,min);
    }
}

void heap_decrease_key(struct nodo* A,int posiz,struct nodo* node){
    //passo il nodo anziche solo la chiave
    if((node->dist) > ((A+posiz)->dist)){
        printf("\nERRORE NUOVA CHIAVE PIU GRANDE DELLA VECCHIA");
    }
    //A[posiz]=key (node)
    (A+posiz)->nome = node->nome;
    (A+posiz)->dist = node->dist;
    (A+posiz)->prev = node->prev;
    //non c'è bisogno di aggiornare lo heapsize

    while(posiz>0 && ((A+((posiz-1)/2))->dist) > ((A+posiz)->dist)){
        //scambia A[i] con A[padrei]
        struct nodo swap;
        swap=*(A+posiz);
        *(A+posiz) = *(A+((posiz-1)/2));
        *(A+((posiz-1)/2)) = swap;
        //i=parenti(i)
        posiz=((posiz-1)/2);
    }
}

void min_heap_insert(struct nodo *A,struct nodo* node){   //aggiunta di un nodo al minheap
    heapsize = heapsize + 1;
    ((A + heapsize)->dist) = infinito;   //A[A.heapsize] = infinito
    ((A + heapsize)->nome) = -1;
    ((A + heapsize)->prev) = -1;
    heap_decrease_key(A, heapsize, node);
}

struct nodo heap_extract_min(struct nodo *A){
    struct nodo minimo;
    if(heapsize < 0){
        printf("\nErrore underflow");
    }
    minimo = *A; //min=A[0];
    *A = *(A + heapsize);     //A[0]=[A.heapsize]
    heapsize = heapsize - 1;
    min_heapify(A,0);
    return minimo;
}

void dijkstra(int dim, int mat[dim][dim], struct nodo *A, struct nodo *G){
    //A: min heap , G:rappresentazione del grafo
    int i;
    int riga;
    struct nodo u;
    heapsize=-1;   //inizializzo Q=0
                    // -1 perchè il vet inizia da zero, così le posizioni coincidono
    //gestisco nodo 0
    G->dist=0;
    G->prev=-1;     //S.PREV = NIL
    min_heap_insert(A,G); //inserisco nodo 0 nel MIN_HEAP (G = primo elemento del grafo)

    for(i=1;i<dim;i++){     //inizializzo tutti i nodi
        (G+i)->dist=infinito;       //dist infinita
        (G+i)->prev=-1;             //predecessore NIL
        min_heap_insert(A,(G+i));   //inserisco tutti i nodi nello heap
    }

    while(heapsize>-1){   //DUBBIO SU QUESTA CONDIZOONE
        u = heap_extract_min(A);  //estraggo nodo con dist minima
        riga = u.nome;  //per l'n-esimo nodo trovo gli adiacenti nell'n-esima riga
        int pos;
        for(i=0;i<dim;i++){
            if(i!=riga && mat[riga][i]!=0){     //scorro gli adiacenti a U
                                                //G+i = i-esimo nodo, cioè nodo v
                if((G+i)->dist > u.dist + mat[riga][i]){
                    (G+i)->dist = u.dist + mat[riga][i];
                    (G+i)->prev = u.nome;
                    pos = trovapos(A,dim,(G+i)->nome);    //potrebbe essere migliorabile
                    heap_decrease_key(A,pos,(G+i));
                }
            }
        }
    }
}


int main() {
    int d,k,in;                 //in per evitare warning su scanf
    in=scanf("%d %d",&d,&k);   //leggo d e k

    struct nodo Q[d];       //min heap che conterrà i nodi, uso un vettore di struct
    struct nodo grafo[d];   //rappresenta il grafo inserito
    int matrice[d][d];      //matrice per salvare l'input

    int maxdim = (d*10)+(d-1); //2^32=4.294.967.295, quindi 10 cifre per arco, più d-1 virgole
    char s[maxdim];
    struct Grafo classifica[k]; //la classifica è un vettore di struct
    int i,c,posiz,tot;
    int id=0;
    int maxdist=0; //inizializzo per warning

    //leggo un comando Topk o AggiungiGrafo
    in=scanf("%s",s);
    while(s[0]=='A' || s[0]=='T'){
        if(s[0]=='A') {     //AggiungiGrafo

            //leggo la matrice per righe
            for(c=0;c<d;c++){
                in=scanf("%s",s);
                posiz=0;    //posiz tiene conto della posizone del carattere nella stringa
                for(i=0;i<d;i++){
                    matrice[c][i]=atoi(&s[posiz]);
                    posiz=posiz+numcifre(matrice[c][i])+1;
                }
            }
            //STAMPO LA MATRICE PER VERIFICA
            /*
            int riga=0;
            int col=0;
            printf("Stampa di controllo\n");
            for(riga=0;riga<d;riga++){
                for(col=0;col<d;col++){
                    printf("\t%d",matrice[riga][col]);
                }
                printf("\n");
            }
             */

            //init min_heap e grafo:
            for (i = 0; i < d; i++) {
                //Q[i].heapsizep = &heapsize;   //in ogni nodo tengo un puntatore alla varaibile heapsize
                grafo[i].nome = i;                //do il nome a ogni nodo
                grafo[i].dist = -1;
                grafo[i].prev = -1;
                //grafo[i].heapsizep = &heapsize;
            }
            //uso l'algoritmo di Dijkstra
            dijkstra(d, matrice, Q, grafo);
            //stampa di prova
            tot = 0;  //somma delle distanze
            for (i = 0; i < d; i++) {
                if (grafo[i].dist == infinito) {
                    grafo[i].dist = 0;    //gestisce nodi non collegati
                }
                //printf("\nDistanza nodo %d: %d", i, grafo[i].dist);
                tot = tot + grafo[i].dist;
            }

            //GESTIONE CLASSIFICA
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
        if(s[0]=='T'){
            //stampa la classifica
            for(i=0;i<id;i++){
                printf("%d ",classifica[i].ID);
            }
            printf("\n");
        }
        in=scanf("%s",s);
        if(s[0]=='T'){
            break;      //NON PUO LEGGERE DUE TOPK CONSECUTIVI
        }
    }
    if(in==7){} //per eliminare warning su scanf
}