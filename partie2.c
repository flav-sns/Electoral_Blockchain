#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "partie1.h"
#include "partie2.h"

void init_key(Key *key, long val, long n){
    key->val1=val;
    key->val2=n;
}

void init_pair_keys(Key* pKey, Key* sKey,long low_size, long up_size){
    long p=random_prime_number(low_size,up_size,5000);
    long q=random_prime_number(low_size,up_size,5000);
    while (p==q){
        q=random_prime_number(low_size,up_size,5000);
    }
    long n,s,u;
    generate_key_values(p,q,&n,&s,&u);
    init_key(pKey,s,n);
    init_key(sKey,u,n);
}

char * key_to_str (Key* key){
    char *str=malloc(sizeof(char)*42);
    //un int sur 64bits peut atteindre une valeur n'excedant pas 19 chiffres en base 10
    //Même si on utilise la base exadécimale, on laisse 19 pour modifier plus facilement.
    // 19+19+ 2 parenthèses + 1 virgule + \0 = 42
    sprintf(str,"(%lx,%lx)",key->val1,key->val2);
    return str;
}

Key *str_to_key(char *str){
    Key *k=malloc(sizeof(Key));
    long val1,val2;
    if (sscanf(str,"(%lx,%lx)",&val1,&val2)!=2){
        printf("Erreur format str_to_key\n");
    }
    init_key(k,val1,val2);

    return k;
}

void liberer_signa(Signature *signa){  //fonction pour liberer signature
    free(signa->tab);
    free(signa);
}

Signature* init_signature(long * content, int size){
    Signature *sign=malloc(sizeof(Signature));
    sign->tab=content; //content déjà alloué et initialisé
    sign->taille=size;
    return sign;
}

Signature* sign(char* mess, Key * sKey){
    int size=strlen(mess);
    long u=sKey->val1;
    long n=sKey->val2;
    long *tab=encrypt(mess,u,n);
    return init_signature(tab,size);
}

char * signature_to_str(Signature *sgn){
    char * result=malloc(10*sgn->taille*sizeof(char));
    result[0]='#';
    int pos=1;
    char buffer[156];
    for(int i=0;i<sgn->taille;i++){
        sprintf(buffer,"%lx",sgn->tab[i]);
        for(int j=0;j<strlen(buffer);j++){
            result[pos]=buffer[j];
            pos=pos+1;
        }
        result[pos]='#';
        pos=pos+1;
    }
    result[pos]='\0';
    result=realloc(result,(pos+1)*sizeof(char));
    return result;
}

Signature* str_to_signature(char *str){
    int len = strlen(str);
    long *content=(long*)malloc(sizeof(long)*len);
    int num=0;
    char buffer[256];
    int pos=0;
    for(int i=0;i<len;i++){
        if (str[i]!='#'){
            buffer[pos]=str[i];
            pos=pos+1;
        }
        else{
            if (pos!=0){
                buffer[pos]='\0';
                if (sscanf(buffer,"%lx",&(content[num]))!=1){
                    printf("Erreur format str_to_signature\n");
                }
                num=num+1;
                pos=0;
            }
        }
    }
    content=realloc(content,num*sizeof(long));
    return init_signature(content,num);
}

Protected *init_protected(Key *pKey, char *mess, Signature *sgn){
    Protected *protect=malloc(sizeof(Protected));
    protect->cle_pub=pKey;
    protect->mess=strdup(mess);
    protect->sign=sgn;
    return protect;
}

void liberer_protected(Protected *pr){
    free(pr->cle_pub);
    free(pr->mess);
    liberer_signa(pr->sign);
    free(pr);
}

int verify(Protected *pr){
    char *decr=decrypt(pr->sign->tab,pr->sign->taille,pr->cle_pub->val1,pr->cle_pub->val2);
    if (strcmp(decr,pr->mess)==0){
        free(decr);
        return 1;
    }
    free(decr);
    return 0;
}

char * protected_to_str(Protected *pr){
    char *cle=key_to_str(pr->cle_pub);
    char *mess=strdup(pr->mess);
    char *sign= signature_to_str(pr->sign);
    char *res=malloc(sizeof(char)*(strlen(cle)+strlen(mess)+strlen(sign)+10)); //place pour les espaces, le message, la signature et la cle en char
    sprintf(res,"%s %s %s",cle,mess,sign);
    free(cle);
    free(mess);
    free(sign);
    return res;
}

Protected * str_to_protected(char *str){
    long val1,val2;
    char mess[356];
    char str1[356];
    if (sscanf(str,"(%lx,%lx);%s;%s",&val1,&val2,mess,str1)!=4){ //mauvais formatage si je met val1 et val2 en char et que je remplace %lx par %s
    //donc comme ca ca marche très bien, je ne peux juste pas utiliser directement str_to_key
        printf("Erreur format str_to_protected\n");
    }
    Signature *signa=str_to_signature(str1);
    Key *k=malloc(sizeof(Key));  
    init_key(k,val1,val2);
    Protected *pro=init_protected(k,mess,signa);
    return pro;
}

void initaliser_tableau(int *tab, int taille){   //initialise les cases du tableau a 0;
    int i=0;
    while(i<taille){
        tab[i]=0;
        i++;
    }
}

int est_present(int *tab,int a, int taille){  //fonction pour savoir si une valeur a est presente dans un tableau de int
    int i=0;
    while(i<taille){
        if(tab[i]==a){
            return 1;
        }
        i++;
    }
    return 0;
}

void generate_random_data(int nv,int nc){
    //nv nombre de citoyens (votants), nc nombre de candidats.
    //initalisation variables necessaires
    Key *skey=malloc(sizeof(Key));
    Key *pkey=malloc(sizeof(Key));
    char *mess;
    char *mess2;
    Signature *decla;
    char *buffer_sign;
    long val1,val2,val3,val4;
    int alea;
    int cpt=0;
    Key *tab=malloc(sizeof(Key)*nc);
    int tab_alea[nc];
    Key *pkey_candidat=malloc(sizeof(Key));
    Key *pkey_electeur=malloc(sizeof(Key));
    Key *skey_electeur=malloc(sizeof(Key));
    initaliser_tableau(tab_alea,nc);
    int i;
//On tire des nombres aléatoirement pour désigner les candidats
    //initialisation tableau (de taille nc) de nombres aleatoire compris entre 0 et le nombre de citoyens (nv)
    for (i=0;i<nc;i++){
        alea=(int)(rand()%nv);
        while(est_present(tab_alea,alea,nc)){
            alea=(int)(rand()%nv);
        }
        tab_alea[i]=alea;
    }
    //ouverture des fichiers
    FILE * fkeys=fopen("keys.txt","w+");
    fprintf(fkeys,"Cle publique;Cle secrete\n");
    FILE * fcandidats=fopen("candidat.txt","w");
    fprintf(fcandidats,"Clé publique\n");
    FILE * fdeclarations=fopen("declaration.txt","w");
    fprintf(fdeclarations,"Clé publique;Message;Signature\n");
    
//création des clés des électeurs
    if (fkeys && fcandidats && fdeclarations){
        //generation d'une paire de cles par citoyens
        for (i=0;i<nv;i++){
            init_pair_keys(pkey,skey,3,7);
            //ecriture paire de cle par citoyen
            fprintf(fkeys,"(%lx,%lx);(%lx,%lx)\n",pkey->val1,pkey->val2,skey->val1,skey->val2);
        }

        fseek(fkeys,0,SEEK_SET); //On repositionne le pointeur au début du fichier pour le lire.
        i=0;
        //selection aleatoire de cle publique de citoyens
        char buffer[256];
        fgets(buffer,256,fkeys); //On déplace le pointeur après l'en tête.
//on récupère la clé publique des candidats tirés au sort
        while (fgets(buffer,256,fkeys)){
            //parcours du tableau avec i le numero de ligne
            //test si i est dans le tableau de nombre aleatoire, si c'est le cas, ajoute ce citoyen comme candidat
            if (cpt==nc){
                break;
            }
            if (est_present(tab_alea,i,nc)){
                if (sscanf(buffer,"(%lx,%lx);(%lx,%lx)\n",&val1,&val2,&val3,&val4)!=4){
                    printf("Erreur format generate_random_data\n");
                }
                fprintf(fcandidats,"(%lx,%lx)\n",val1,val2);
                init_key(&tab[cpt],val1,val2);
                cpt++;
            }
            i++;
        }
//Constitution des déclarations de vote
        fseek(fkeys,0,SEEK_SET); //On repositionne le pointeur au début du fichier pour le lire.
        fgets(buffer,256,fkeys); //On déplace le pointeur après l'en tête.
        while (fgets(buffer,256,fkeys)){
            if (sscanf(buffer,"(%lx,%lx);(%lx,%lx)\n",&val1,&val2,&val3,&val4)==4){;
                //construction de la declaration signée du citoyen
                alea=(int)(rand()%nc);
                *pkey_candidat=tab[alea];
                init_key(pkey_electeur,val1,val2);
                init_key(skey_electeur,val3,val4);
                mess=key_to_str(pkey_candidat);
                mess2=key_to_str(pkey_electeur);
                decla=sign(mess,skey_electeur);
                buffer_sign=signature_to_str(decla);
                //ecriture declaration par citoyen
                fprintf(fdeclarations,"%s %s %s\n",mess2,mess,buffer_sign);
                //liberation à chaque tour de boucle des variables temporaires
                free(mess);
                free(mess2);
                free(buffer_sign);
                liberer_signa(decla);
            }else{
                printf("Erreur format generate_random_data\n");
            }
        }
        fclose(fkeys);
    }else{
        printf("Problème à l'ouvrture des fichiers\n");
    }
    //liberation et fermeture de fichiers
    fclose(fdeclarations);
    fclose(fcandidats);
    free(skey);
    free(pkey);
    free(skey_electeur);
    free(pkey_electeur);
    free(pkey_candidat);
    free(tab);

}