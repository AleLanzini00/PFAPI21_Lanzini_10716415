/*NOTE
 * Per aggiornare la classifica sostituisco il MASSIMO
 * Scrivere algoritmo trovamassimo: data la calssifica e k(dim), reistuisce la posiz del valore
    con la distanza maggiore
 *Testare con valori prova che la classifca funzioni correttamente
 * Scrivere algortimo che calcola distanza
*/
#include <stdio.h>
#include<stdlib.h>
#include<math.h>

void TopK(){
}

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

int calcoladist(int dim){
    int distanza=0;
        //algoritmo da fare, anche i paramentri
    return distanza;
}

 struct Grafo{
    int ID;
    int distanza;
};

int trovamassimo(struct Grafo vet[],int dim){   //restituisce la posizione del massimo
    int max=0;
        //da scrivere
    return max;
}

int main() {
    int d,k,c,i,posiz,dist;
    int maxdist=0;
    int id=0;   //usato per contare i grafi inseriti, e quindi ne restituisce l'id
    printf("Inserire d e k:\n");
    scanf("%d %d",&d,&k);   //leggo d e k
    int maxdim=(d*10)+(d-1); //2^32=4.294.967.295, quindi 10 cifre per arco, più d-1 virgole
    char s[maxdim];
    struct Grafo classifica[k]; //la classifica è un vettore di struct
    int matrice[d][d];  //matrice per salvare input

    //leggo un comando
    printf("Inserire un comando (Aggiungigrafo o TopK):\n");
    scanf("%s",s);
    while(s[0]=='A'){   //AggiungiGrafo
        //leggo la matrice  per righe
        for(c=0;c<d;c++){
            scanf("%s",s);
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
        dist=calcoladist(d); //devo passare la matrice
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


        //leggo nuovo comando
        id++;
        scanf("%s",s);
    }
    if(s[0]=='T'){
        //STAMPA LA CLASSIFICA
    }
}
