#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
#include "partie4.h"
#include <openssl/sha.h>
#include <dirent.h>

void print_list_protected_file(CellProtected *LCK,FILE *f){
  CellProtected *l=LCK;
  char *chaine;
  if (f){
    while(l){
      chaine=protected_to_str(l->data);
      fprintf(f,"%s\n",chaine);
      l=l->next;
      free(chaine);
    }
  } //fonction pour ecrire une liste de Protected dans un fichier
}


unsigned char * SHA_256(const char *c){       //fonction de Hachage sur 256bits

  unsigned char *hash=SHA256(c,strlen(c),0);                                       // Le hash
  unsigned char* res=malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH*2+1);      //SHA256_DIGEST_LENGTH est la longueur inclus dans la lib
 
 int j=0;
 int i;

 for(i=0;i<SHA256_DIGEST_LENGTH*2;i+=2){          //ecriture dans res pour qu'ensuite on puisse l'ecrire facilement, qu'il soit pas sous forme brute
    sprintf(res+i,"%02x",hash[j]);
    j++;
  }
  res[i]='\0';

  return res;
}


char * block_to_str(Block * block){

  char *key_tmp=key_to_str(block->author);
  char *protect_tmp;
  int taille_dyna=(strlen(key_tmp)+500);   //taille des chaines de char + taille d'un int (32bits)+ espaces
  char *tmp=malloc(sizeof(char)*taille_dyna);
  char n[10]; //buffer pour nonce

  strcpy(tmp,key_tmp);         //copie de la cle dans tmp
 
  strcat(tmp,block->previous_hash);     //concat de previous_hash

  CellProtected *v=block->votes;

  while (v){                            //parcours de la liste de protected pour les transformer en str afin de concat dans tmp

    protect_tmp=protected_to_str(v->data);
    taille_dyna+=strlen(protect_tmp)+2;         //maj de la taille en char 
    tmp=realloc(tmp,taille_dyna);              // reallocation de la place memoire necessaire
    strcat(tmp,protect_tmp);
    free(protect_tmp);
    v=v->next;
  }
  sprintf(n,"%d",block->nonce);    //recuperation de nonce, int to str
  strcat(tmp,n);                   //concat de nonce

  free(key_tmp);                    //liberation de la cle qui a ete copiée

  return tmp;
}


void write_block(Block *b, char* file){    //ecriture d'un block dans un fichier
  FILE * output=fopen(file,"w");
  if (output){

    fprintf(output,"(%lx,%lx)\n",b->author->val1,b->author->val2);
    fprintf(output,"%s\n",b->hash);
    fprintf(output,"%s\n",b->previous_hash);  
    fprintf(output,"%d\n",b->nonce);
    print_list_protected_file(b->votes,output);

  }

  fclose(output);
}


Block * read_block(char *file){

  FILE *input=fopen(file,"r");             //declaration de toutes les variables necessaires
  Block *b=malloc(sizeof(Block));
  char buffer[356];
  Key *k_author;
  char *hash;
  char *previous_hash;
  char buff_hash[500];
  char buff_key[256];
  char *fg=malloc(sizeof(char)*500);
  char *fh=malloc(sizeof(char)*500);
  int nonce;

  if (input){         //lecture des premieres lignes

    fgets(buffer,356,input);
    sscanf(buffer,"%s",buff_key);
    k_author=str_to_key(buff_key);

    fgets(buff_hash,500,input);
    sscanf(buff_hash,"%s",fg);

    fgets(buffer,356,input);
    sscanf(buffer,"%s",fh);

    fgets(buffer,356,input);
    sscanf(buffer,"%d",&nonce);

  }
  

  CellProtected **liste=read_protected_block(input);       //creation de la liste de protected à partir du fichier

  fclose(input);

  //initialisation du block avec toutes les var recuperees
  b->author=k_author;
  b->previous_hash=fh;
  b->votes=*liste;
  b->nonce=nonce;
  b->hash=fg;

  free(liste); //liberation du double pointeur qui ne nous servira a rien ici

  return b;
}


void delete_block(Block *b){   //fonction que les profs veulent qu'on fasse (ca produit des fuites memoires car dans la consigne il ne faut pas liberer le contenu des celleprotected)
  CellProtected *tmp; 
  free(b->hash);
  free(b->previous_hash);
  CellProtected *v=b->votes;
  while(v){
    tmp=v->next;
    free(v);
    v=tmp;
  }
  free(b);
}

void delete_block_all(Block *b){  
  CellProtected *tmp;
  free(b->hash);
  free(b->author);
  free(b->previous_hash);
  CellProtected *v=b->votes;
  while(v){
    tmp=v->next;
    liberer_protected(v->data);
    free(v);
    v=tmp;
  }
  free(b);//fonction qui delete le block en entier car car me perturbe d'avoir des fuites
}


void compute_proof_of_work_b(Block *B, int d){

  if (!B){
    printf("Block vide\n");
    exit(0);
  }

  int j;
  char buffer[50000];
  B->nonce=0;
  char *str=block_to_str(B);
  sprintf(buffer,"%s",str);
  free(str);
  int taille=strlen(buffer);
  unsigned char *hash;
  int i;
  int nb_zero=0;
  B->nonce=-1;
  char n[10];

  while( nb_zero!=d){
    nb_zero=0;
    (B->nonce)++;
    //sprintf(n,"%d",B->nonce);
    //strcpy(buffer+taille-1,n);
    str=block_to_str(B);
    hash=SHA_256(str);
    printf("%s\n",hash);
    for(int i=0;i<=d/2;i++){

      if(hash[i]/16==0){
        nb_zero++;
        if(nb_zero==d){
          break;
        }
      }else{
        break;
      }
      if(hash[i]%16==0){
        nb_zero++;
        if(nb_zero==d){
          break;
        }
      }
      else{
        break;
      }
    }

  }
  /*for(int j=0;j<32;j++){
             printf("%02x",hash[j]);
          }
          printf("\n");
        printf("%d\n",B->nonce);
        printf("%s\n\n\n",buffer);*/
  B->hash=hash; //ancien proof of work ou je manipulais directement les bits qui etait 10X plus rapide

}
void compute_proof_of_work(Block *B, int d){

  if (!B){
    printf("Block vide\n");
    exit(0);
  }

  char *block=block_to_str(B);          //transfo du block en str
  unsigned char *hash=SHA_256(block);   //premier hash du block
  //printf("Debut : %s\n",hash);
  unsigned char a=0;                   // initialisation d'un char qui vaut 0
  int test=0;
  int i=0;

    while (i<d){
      if(hash[i]!=48){           // si le masque OU entre a et hash[i] n'est pas égal à "0" (valeur ascii 48) alors on incremente nonce et on refait un hash du nouveau block avec l'iterateur i=0
        B->nonce++;
        free(hash);
        free(block);
        block=block_to_str(B);
        hash=SHA_256(block);
        i=-1;
      }
      i++;
    }
  free(block);
  B->hash=hash;
  //printf("TROUVE : %s\n",hash);      //proof of work conforme à la consigne qui est beaucoup plus lent mais qui utilise un hash plus maniable car sous forme str
  
}

int verify_block(Block *b, int d){    //on verifie si le block a le bon nombre (d) de 0 au debut 

  char *block=block_to_str(b);
  unsigned char *hash=SHA_256(block);
  int i=0;
  unsigned char a=0;

  while (i<d){
      if((a | hash[i])!=48){
        return 0;
      }
      i++;
  }

  free(hash);
  free(block);
  return 1;
}

CellTree *create_node(Block * b){  //initialisation d'un noeud

  if (!b){
    return NULL;
  }

  CellTree *tree=malloc(sizeof(CellTree));

  if(!tree){return NULL;}
  tree->block=b;
  tree->father=NULL;
  tree->firstChild=NULL;
  tree->nextBro=NULL;
  tree->height=0;
  return tree;
}


int update_height(CellTree *father, CellTree *child){
  int max=0;

  if ((father->height) > child->height){
    return 0;
  }
  father->height=child->height+1;
  return 1;
}


void add_child(CellTree *father, CellTree *child){  //rajoute un enfant au noeud passe en parametre 
  if (! father->firstChild){
    //printf("ajout_first\n");
    father->firstChild=child;
    child->father=father;
  }
  else{
    CellTree *courant=father->firstChild;

    while(1){
      if(courant->nextBro==NULL){
        //printf("Ajout bro\n");
        courant->nextBro=child;
        child->father=father;
        break;
      }
      courant=courant->nextBro;
    }
  }
  
  CellTree *courant_fath=father;
  CellTree *courant_child=child;
  while (courant_fath){
    update_height(courant_fath,courant_child);
    courant_child=courant_fath;
    courant_fath=courant_fath->father;
  }
}



void print_tree(CellTree *father){
  char *k_tmp;
  CellTree *courant=father;
  while (courant){
    k_tmp=key_to_str(courant->block->author);
    printf("%s------%d\n",k_tmp,courant->height);
    free(k_tmp);
    if (courant->nextBro){
      print_tree(courant->nextBro);
    }

    courant=courant->firstChild;
  }//je print les cles de l'author avec la profondeur a cote mais libre à toi d'en decider autrement si tu veux un truc plus esthetique
}



void delete_node(CellTree *node){
  delete_block_all(node->block);
  free(node);//supprimer un noeud
}


void delete_tree(CellTree *father){   //supprimer l'arbre 
  CellTree *tmp;
  CellTree *tmp2;

  while (father){

    tmp2=father->firstChild;
    tmp=father->nextBro;
    delete_node(father);

    if (tmp){
      delete_tree(tmp);
    }

    father=tmp2;
  }
}



CellTree *hightest_child(CellTree *cell){ //renvoie l'enfant le plus haut 

  if (!cell){
    return NULL;
  } 

  CellTree *max_t;
  int max=0;
  CellTree *courant=cell;
  
  while (courant){

    if (courant->height>max){
      max=courant->height;
      max_t=courant;
    }

    if(courant->nextBro){
      hightest_child(courant->nextBro);
    }

    courant=courant->firstChild;
  }
  return max_t;
}



CellTree *last_node(CellTree *tree){

  if (!tree){
    return NULL;
  }

  CellTree *chaine=hightest_child(tree);
  CellTree *courant=chaine->firstChild;

  while(1){
    if (courant->nextBro==NULL){
      return courant;
    }
    courant=courant->nextBro;
  }
  return NULL;//normalement fonctionelle mais je l'utiliserai pas car rajoute une complexite O(n) 
}


//reponse à la question posée : La complexité est en o(n), pour avoir une complexite o(1) il faudrait une liste doublement chainee
CellProtected * fusion(CellProtected *liste1, CellProtected *liste2){
  if(!liste1){
    return liste2;
  }
  else{
    CellProtected *courant=liste1;
    while (courant){
      if (courant->next==NULL){
        courant->next=liste2;
        break;
      }
      courant=courant->next;
    }
    return liste1;
  }
}


CellProtected *fusion_liste(CellTree *tree,CellProtected *liste_courante){ 
  CellProtected *lst;
  CellTree *father=tree;
  while (father){
    lst=fusion(liste_courante,father->block->votes);
    liste_courante=lst;
    if (father->nextBro){
      fusion_liste(father->nextBro,liste_courante);
    }

    father=father->firstChild;
  }
  return liste_courante;
}


void submit_vote(Protected *p){ //fonction pour ecrire la declaration dans un fichier

  FILE *fich=fopen("Pending_votes.txt","a");  //a pour append en fin de fichier
  char *pr=protected_to_str(p);
  if (fich){

    fprintf(fich,"%s\n",pr);
  }
  free(pr);
  fclose(fich);
}



CellTree * create_block(CellTree *tree, Key * author,int d){

  //CellTree *last=last_node(tree);  //recherche du dernier bloc, on utilisera pas ici

  //initialisation du bloc
  Block *new_block=malloc(sizeof(Block));
  new_block->author=author;       
  new_block->nonce=0;
  CellProtected **votes=read_protected("Pending_votes.txt"); 
  new_block->votes=*votes;

  //creation du noeud
  CellTree *new_node=create_node(new_block);


  if (tree!=NULL){

    //calcul du hash du block precedent pour le mettre dans previous_hash
    char *str=block_to_str(tree->block);
    new_block->previous_hash=SHA_256(str);
    
    //ajout du fils
    add_child(tree,new_node);
    free(str);

  }else{

    new_block->previous_hash=SHA_256("firstblock");
    tree=new_node;
  }

  compute_proof_of_work(new_block,d);  //calcul du hash

  if(remove("Pending_votes.txt")!=0){
    printf("impossible de supprimer le fichier");
    exit(0);
  }

  write_block(new_block,"Pending_block.txt");     //ecriture du block dans pending_block.txt

  return new_node;
}


void add_block(int d, char* name){

  char buffer[256];
  Block *b=read_block("Pending_block.txt"); //lecture du block dans pending_block
  CellProtected **new_liste=reverse(b->votes); //comme la lecture de la liste se fait avec ajout en tete elle est inversee, donc je la reverse pour qu'elle soit dans le bon ordre
  free(b->votes);
  b->votes=*new_liste;


  if (verify_block(b,d)){              //si bloc valide

      FILE *f=fopen(name,"w");

      if (f){
        write_block(b,name);     //ecriture du bloc
      }
      fclose(f);

    delete_block_all(b);        //suppression du bloc car plus besoin une fois ecrit

    strcpy(buffer,"Blockchain/"); //construction du chemin pour deplacer le fichier Blockchain/name
    strcat(buffer,name);

    if(rename(name,buffer)!=0){     //changement de repertoire
      printf("impossible de déplacer");
      exit(0);
    }

    if(remove("Pending_block.txt")!=0){    //suppression de pending_block
      printf("impossible de supprimer le fichier");
      exit(0);
    }
  }
}


CellTree *read_tree(){
    //declaration variables necessaires
    CellTree **tab=malloc(sizeof(CellTree*));
    int i=0;
    CellTree *tree_courant;
    Block *b_courant;
    char *db;
    DIR *rep =opendir("Blockchain/");
    char chemin[356];

    //ouverture repertoire
    if(rep!=NULL){
      struct dirent *dir;
      while((dir=readdir(rep))){

        if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){  //tant qu'il y'a des fichiers presents

          strcpy(chemin,"Blockchain/");  //creation du chemin
          strcat(chemin,dir->d_name);

          b_courant=read_block(chemin); //lecture et creation du bloc

          tree_courant=create_node(b_courant);  //creation du noeud avec le bloc

          tab=realloc(tab,sizeof(CellTree*)*(i+1)); //reallocation de la place necessaire pour chaque noeud vu qu'on ne connait pas leur nombre à l'avance

          tab[i]=tree_courant;                //noeud dans tab

          i++;
        }

      }
      closedir(rep);  //fermeture repertoire
    }

    int taille=i;
    int j;
    
    //liaison de tous les pere et fils
    for(i=0;i<taille;i++){
      for(j=i+1;j<taille;j++){
          if (strcmp(tab[i]->block->hash,tab[j]->block->previous_hash)==0){  //si previous_hash==hash alors ils ont un lien de parente
              add_child(tab[i],tab[j]);
          }
      }
    }

    //trouver la racine pour la renvoyer
    for(i=0;i<taille;i++){
      if(tab[i]->father==NULL){
        return tab[i];
      }
    }

}


Key *compute_winner_BT(CellTree * tree, CellKey* candidates, CellKey *voters, int sizeC, int sizeV){  //election du president

  CellProtected *l=NULL; //initialisation de la liste necessaire pour fusion_liste
  CellProtected *liste_courante=fusion_liste(tree,l);
  verify_list(&liste_courante);
  
  Key *k=compute_winner(liste_courante,candidates,voters,sizeC,sizeV);   //cle gagnante
  return k;
}