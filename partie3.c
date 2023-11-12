#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"

//Partie CELLKEY

CellKey * create_cell_key(Key *key){
    if (key==NULL){
        return NULL;
    }
    CellKey *new_cell=malloc(sizeof(CellKey));
    new_cell->data=key;
    new_cell->next=NULL;
    return new_cell;
}

void append_head_list(CellKey **liste, Key *key){ //par double pointeur pour pointer sur le premier element de la liste
  CellKey *new=create_cell_key(key);
  if (!new){
    printf("Problème allocation mémoire CellKey");
    return;
  }
  if (! *liste){
    (*liste)=new;
  }else{
    new->next=*liste;
    *liste=new;
  }
}


CellKey ** read_public_keys(char *nomfichier){  //par double pointeur pour pointer sur le premier element de la liste
  
  char* nom_fich_test1="candidat.txt";
  char* nom_fich_test2="keys.txt";
  long val1,val2;
  char buffer[256];
  CellKey **liste=malloc(sizeof(CellKey));
  *liste=NULL;
  //CellKey *new;
  Key *cle_pub;
  if (strcmp(nom_fich_test1,nomfichier) && strcmp(nom_fich_test2,nomfichier)){ //securisation par verification des bons fichiers à ouvrir en parametre
    printf("Nom de fichier ne correspond pas \n");                         
    exit(0);
  }
  else{
    FILE * output=fopen(nomfichier,"r");
    fgets(buffer,256,output); //permet de sauter l'en-tête du document à lire
    int i=0;
    while (fgets(buffer,256,output)){
      if (sscanf(buffer,"(%lx,%lx)",&val1,&val2)==2){
        cle_pub=malloc(sizeof(Key));
        init_key(cle_pub,val1,val2);
        append_head_list(liste,cle_pub);
      }else{
        printf("Erreur format read_public_keys\n");
      }
    }
    fclose(output);
  }
  return liste;
}


void print_list_keys(CellKey *LCK){
  CellKey *l=LCK;
  while(l){
    printf("(%lx,%lx)\n",l->data->val1,l->data->val2);
    l=l->next;
  }
  printf("\n");
}

void delete_cell_keys(CellKey *c){
  free(c->data);
  free(c);
}

void delete_list_keys(CellKey **l){
  CellKey *tmp;
  while (*l){
    tmp=(*l)->next;
    delete_cell_keys(*l);
    *l=tmp;
  }
  free(l);
}


//Partie CELLPROTECTED

CellProtected *create_cell_protected(Protected *pr){
  CellProtected *new_cell=malloc(sizeof(CellProtected));
  new_cell->data=pr;
  new_cell->next=NULL;
  return new_cell;
}


void append_head_listp(CellProtected **l,Protected *pr){ //par double pointeur pour pointer sur le premier element de la liste
  CellProtected *new=create_cell_protected(pr);
  if (!new){
    printf("Problème allocation mémoire CellProtected");
    return;
  }
  if (!l){
    *l=new;
  }else{
    new->next=*l;
    *l=new;
  }
}


CellProtected ** read_protected(char *nomfichier){
  char buffer[256];
  long val1,val2,val3,val4;
  char buffer_skey[256];
  char buffer_mess[256];
  char buffer_decla[256];
  Key *skey_tmp;
  Signature *signa_tmp;
  Protected *pr;
  CellProtected **liste=malloc(sizeof(CellProtected));
  *liste=NULL; //si je l'initialise pas à NULL, erreur unitialised value
    
  FILE * output=fopen(nomfichier,"r");
  while (fgets(buffer,256,output)){
    if (sscanf(buffer,"(%lx,%lx);(%lx,%lx);%s\n",&val1,&val2,&val3,&val4,buffer_decla)==5){ //formatage
      sprintf(buffer_skey,"(%lx,%lx)",val1,val2);
      skey_tmp=str_to_key(buffer_skey);  //str -> cle
      signa_tmp=str_to_signature(buffer_decla); // str -> signature
      sprintf(buffer_mess,"(%lx,%lx)",val3,val4);
      pr=init_protected(skey_tmp,buffer_mess,signa_tmp); //initalisation de pr pour inserer dans la liste chainee
      append_head_listp(liste,pr); //insertion dans la liste
    }
  }
  fclose(output);

  return liste;
}

CellProtected ** read_protected_block(FILE *nomfichier){
  long val1,val2;
  char buffer[256];
  char buffer_skey[256];
  char buffer_mess[256];
  char buffer_decla[256];
  Key *skey_tmp;
  Signature *signa_tmp;
  Protected *pr;
  CellProtected **liste=malloc(sizeof(CellProtected));
  *liste=NULL; //si je l'initialise pas à NULL, erreur unitialised value
  fgets(buffer,256,nomfichier);
  while (fgets(buffer,256,nomfichier)){
    sscanf(buffer,"%s %s %s",buffer_skey,buffer_mess,buffer_decla);
    skey_tmp=str_to_key(buffer_skey);  //str -> cle
    signa_tmp=str_to_signature(buffer_decla); // str -> signature
    pr=init_protected(skey_tmp,buffer_mess,signa_tmp); //initalisation de pr pour inserer dans la liste chainee
    append_head_listp(liste,pr); //insertion dans la liste
  }

  return liste;
}

CellProtected ** reverse(CellProtected *liste){
  CellProtected **liste_retour=malloc(sizeof(CellProtected));
  *liste_retour=NULL;

  CellProtected *courant=liste;
  while(courant){
    append_head_listp(liste_retour,courant->data);
    courant=courant->next;
  }
  return liste_retour;
}

void print_list_protected(CellProtected *LCK){
  CellProtected *l=LCK;
  char *chaine;
  while(l){
    chaine=protected_to_str(l->data);
    printf("%s\n",chaine);
    l=l->next;
    free(chaine);
  }
  printf("\n");
}


void delete_cellprotected(CellProtected *c){
  liberer_protected(c->data);
  free(c);
}


void delete_list_protected(CellProtected **l){ 
  CellProtected *tmp;
  while (*l){
    tmp=(*l)->next;
    delete_cellprotected(*l);
    *l=tmp;
  }
  free(l);
}


void verify_list(CellProtected **liste){
  CellProtected *l=*liste;
  CellProtected *prec=NULL;
  CellProtected *tmp;
  while (l){
    if (!verify(l->data)){
      if (prec==NULL){
        tmp=l->next;
        delete_cellprotected(l);
        l=tmp;
        *liste=tmp;
      }else{
        prec->next=l->next;
        delete_cellprotected(l);
        l=prec;
      }
    }
    prec=l;
    l=l->next;
  }
}

HashCell * create_hashcell(Key *key){
  if (key==NULL){
    return NULL;
  }
  HashCell *hash=malloc(sizeof(HashCell));
  hash->key=malloc(sizeof(Key));                //on crée les propres key de la table de hachage pour qu'elle soit independante de la liste de keys
  init_key(hash->key,key->val1,key->val2);
  hash->val=0;
  return hash;
}

void delete_hashcell(HashCell *h){
  free(h->key);
  free(h);
}

int hash_function(Key *key, int size){
  int res= (key->val1) + (key->val2);
  double a=(sqrt(5)-1)/2;            //hash vu dans le mini-projet
  return floor(size*(res*a-floor(res*a)));
}

int find_position(HashTable *t, Key *key){
  int indice=hash_function(key,t->size);
  if (t->tab[indice]==NULL){
    return indice;                //si la case du tableau d'indice du hash est libre on retourne le hash 
  }
  else{
    int i=indice+1;
    while(t->tab[i]!=NULL && i<(t->size-1)){
      i++;                               // tant qu'on ne trouve pas de case libre et tant que i plus petit que la taille du tableau
    }
    if (i==(t->size-1) && t->tab[t->size-1]!=NULL){   //si la derniere case du tableau n'est pas libre, et dans le cas ou aucune case n'etait libre
      i=0; 
      while(t->tab[i]!=NULL && i<indice){     //on recommence à parcourir mais de 0 jusqu'a indice
        i++;                        
      }
    }
    return i;
  }
}

int key_equal(Key *key1, Key *key2){                      //fonction pour voir si 2 cles sont egales par leur valeur
  if (key1->val1==key2->val1 && key1->val2==key2->val2){
    return 1;
  }
  return 0;
}


int find_index(HashTable *t, Key *key){      //fonction qui cherche la presence de la cle passé en parametre dans la table de hachage, retourne -1 si pas trouvée
  int indice=hash_function(key,t->size);

  if (key_equal(t->tab[indice]->key,key)==1){
    return indice;                              //un peu le meme systeme que find_position mais sans chercher de case libre
  }
  else{
    int i=indice+1;
    while(i<(t->size-1)){
      if (key_equal(t->tab[i]->key,key)==1){
        return i;
      }
      i++;
    }
    i=0;
    while(i<indice){
      if (key_equal(t->tab[i]->key,key)==1){
        return i;
      }
      i++;
    }
    return -1;
  }
}



HashTable * create_hashtable(CellKey *keys, int size){
  HashTable *hasht =malloc(sizeof(HashTable));
  hasht->size=size;
  hasht->tab=malloc(sizeof(HashCell)*size);

  for (int j=0;j<size;j++){       //initialiser toutes les cases à NULL
    hasht->tab[j]=NULL;
  }

  HashCell *cl;
  int indice;

  for (int i=0;i<size;i++){
    if (!keys){               //possibilité que la liste de clé soit plus petite que la table de hachage dans certains cas
      break;
    }
    indice=find_position(hasht,keys->data);  //on cherche une case libre
    //printf("indice fonction %d\n",indice);
    cl=create_hashcell(keys->data);
    hasht->tab[indice]=cl;      
    //hasht->tab[i]=cl;
    keys=keys->next;
  }
  return hasht;
}

void delete_hashtable(HashTable *t){
  for (int i=0;i<t->size;i++){
    if (t->tab[i]!=NULL){
      delete_hashcell(t->tab[i]);
    }
  }
  free(t->tab);
  free(t);
}


Key * compute_winner(CellProtected *decl,CellKey * candidates,CellKey *voters, int sizeC, int sizeV){
  HashTable *table_candidats=create_hashtable(candidates,sizeC);
  HashTable *table_voters=create_hashtable(voters,sizeV);            //creation des tables de hachages

  CellProtected *decl2=decl;
  int indice_voter=0;
  int indice_candidat=0;
  Key *key_tmp_candidat;
  int max=0;
  Key *gagnant=malloc(sizeof(Key));

  while (decl2){
    
    indice_voter=find_index(table_voters,decl2->data->cle_pub); //rechercher si clé presente dans la table hachage
    key_tmp_candidat=str_to_key(decl2->data->mess);  //clé temporaire pour find_index
    indice_candidat=find_index(table_candidats,key_tmp_candidat); //rechercher si clé presente dans la table hachage

    if (indice_voter!=-1 && indice_candidat!=-1){        //si clé presente dans table de hachage
      if (table_voters->tab[indice_voter]->val==0){     //si le votant n'a pas deja voté
        table_candidats->tab [indice_candidat]->val++;    //augmenter le candidat d'une voix
        table_voters->tab[indice_voter]->val=1;           //indiquer que le votant a voté
      }
    }
    free(key_tmp_candidat);
    decl2=decl2->next;
  }

  for (int i=0;i<sizeC;i++){               //parcours du tableau de candidat pour avoir le gagnant
    if (table_candidats->tab[i]!=NULL){ 
      if (table_candidats->tab[i]->val>max){
        //printf("cle (%lx,%lx)\n",table_candidats->tab[i]->key->val1,table_candidats->tab[i]->key->val2);
        max=table_candidats->tab[i]->val;
        init_key(gagnant,table_candidats->tab[i]->key->val1,table_candidats->tab[i]->key->val2);  //initialisation d'une key independante pour designer le gagnant qui sera free dans le main
      }
    }
  }
  delete_hashtable(table_candidats);
  delete_hashtable(table_voters);
  return gagnant;
}