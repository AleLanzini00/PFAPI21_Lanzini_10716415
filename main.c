/*NOTE
 * TROPPO LENTO
 * TEST 4 BLOCCATO
 * NON PUO LEGGERE 2 TOPK CONSECUTIVI
 *passare ad algoritmo la matrice con PUNTATORE
 *Testare con valori prova che la classifca funzioni correttamente
*/
#include <stdio.h>
#include<stdlib.h>
#include<math.h>

struct Grafo{
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

void visita(int nodo, int size, int matr[size][size],int* vettdistp){       //IMPLEM USO DI PUNT A MATRICE
    int i;
    for(i=0;i<size;i++){    //guardo la riga del nodo interessato
        if(matr[nodo][i]!=0 && i!=nodo){
            if((*(vettdistp+i) > (matr[nodo][i]+*(vettdistp+nodo)) || *(vettdistp+i)==-1)){     //allora esploro i suoi adiacenti
                *(vettdistp+i)=matr[nodo][i]+*(vettdistp+nodo); //aggiorno distanza nodo esplorato
                visita(i,size,matr,vettdistp);  //esploro adiacenti nuovo nodo

            }
        }
    }
}

int algoritmo(int dim,int mat[dim][dim]){
    int disttot=0;
    int n=0;
    //inizializzo i nodi
    int vettdist[dim];   //ogni poszione contiene la distanza di un nodo
    for(n=0;n<dim;n++){     //pongo tutte le distanze a -1 (valore simbolico)
        if(n==0){
            vettdist[n]=0;
        }
        else
            vettdist[n]=-1;
    }
    //esplorazione
    int r=0;
    int c;
    for(c=1;c<dim;c++){
        if(mat[r][c]!=0){
            if(vettdist[c]==-1 || vettdist[c]>mat[r][c]){
                vettdist[c]=mat[r][c]; //segno distanza nodi adiacenti a zero
                visita(c,dim,mat, vettdist);
            }
        }
    }
    for(n=0;n<dim;n++){
        //prova di stampa
        //printf("\nDistanza nodo %d: %d",n,vettdist[n]);
        disttot=disttot+vettdist[n];
    }
    return disttot;
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

int main() {
    int d,k,c,i,posiz,dist;
    int maxdist=0;
    int id=0;   //usato per contare i grafi inseriti, e quindi ne restituisce l'id
    int in=0;
    //printf("Inserire d e k:\n");
    in=scanf("%d %d",&d,&k);   //leggo d e k
    int maxdim=(d*10)+(d-1); //2^32=4.294.967.295, quindi 10 cifre per arco, più d-1 virgole
    char s[maxdim];
    struct Grafo classifica[k]; //la classifica è un vettore di struct
    int matrice[d][d];  //matrice per salvare input
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
        dist=algoritmo(d,matrice); //devo passare la matrice
        if(id<k){   //inizializzo la classifica con i primi K grafi
            classifica[id].ID=id;
            classifica[id].distanza=dist;
            if(id==k-1){
                maxdist=trovamassimo(classifica,k); //calcolo il massimo solo una volta riempito il vettore, prima inserisco sempre
            }

        }
        else{
            if(dist<classifica[maxdist].distanza){
                //sostituisco il maxid
                classifica[maxdist].ID=id;
                classifica[maxdist].distanza=dist;
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
    if(in==7){} //per eliminare warning su scanf
}
