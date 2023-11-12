#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "partie1.h"
#include "partie2.h"
#include "partie3.h"
#include "partie4.h"
#define SIZE_MAIN 256
#define UNITE_TEMPS 1000 /*Permet de modifier l'unité temporelle pour
les relevés en multipliant le résultat.
Donc 1 : secondes, 100 : centièmes, 1000 : millièmes, etc...*/
#define INPUT 0 //INPUT vaut 0 pour utiliser stdin pour communiquer
// ou INPUT vaut 1 pour passer les commandes via un fichier nommé "INSTRUCTIONS"

//Aucune fuite mémoire

void menu(){
    printf("FONCTIONS DU PROGRAMME:\n\
    0-Sortie du programme\n\
    1-Test de primalité (avec la version naïve)\n\
    2-Plus grand nombre testable avec is_prime_naive\n\
    3-Calcul exponentation modulaire O(m) (naïve)\n\
    4-Calcul exponentation modulaire O(log2(m))\n\
    5-Génerateur de nombre premier aléatoire\n\
    6-Encodage puis décryptage d'un message\n\
    7-Tests fonctions Key, Signature et Protected\n\
    8-Test generate_random_data\n\
    9-Tests read_public_Keys - Protected - Candidat\n\
    10-Tests partie 3\n\
    11-Test élection\n\
    12-Test compute_proof_of_wor\n\
    ");
}

int actionUtilisateur(){
    int rep,rep_prec=0;
    char saisie[10];
    char buffer[SIZE_MAIN];
    double temps;
    clock_t t;
    srand(time(NULL)); //initialise la graine pour la génération des nombres pseudo aléatoires.
    FILE* f = fopen("output.csv", "w");
    if (f==NULL){
        printf("Problème lors de l'ouverture du fichier\n");
        return 1;
    }
    /*Le test suivant permet de choisir entre stdin et un ficher "INSTRUCTIONS" pour communiquer les
    commandes au programme. Pour passer de l'un à l'autre : modifier la constante INPUT*/
    FILE* input;
    if (INPUT == 1){
        input = fopen("INSTRUCTIONS", "r");
        if (input==NULL){
            printf("Problème lors de l'ouverture du fichier\n");
            return 1;
        }
    }else{
        input = stdin;
    }
    do{
        menu();
        fgets(saisie,10, input);
        rep=atoi(saisie);
        switch(rep){

            case 1:;
            printf("Veuillez entrer l'entier à tester:\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                long p;
                int r;
                if (sscanf(buffer,"%ld \n",&p)==1){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Nombre testé;Durée du test de primalité (naïf);Résultat\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();
                    r=is_prime_naive(p);
                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC); 
                    if(r){
                        printf("%ld est un nombre premier\n",p);
                    }else{
                        printf("%ld n'est pas premier\n",p);
                    }
                    fprintf(f,"%ld;%.2lf;%d\n",p,temps,r);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 2:;
            printf("Nous voulons trouver quel est le plus grand nombre premier testable en moins de n millisecondes.\n\
            Veuillez écrire ce réel n:\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                double saisie_d;
                if (sscanf(buffer,"%lf \n",&saisie_d)==1){
                    int r=0;
                    int p;
                    int i=3;
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Nombre premier testé;Durée du test\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    do{
                        t=clock();
                        r=is_prime_naive(i);
                        t=clock()-t;
                        if(r){
                            temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                            fprintf(f,"%d;",i);
                            fprintf(f,"%.2lf\n",temps);
                            if (temps<saisie_d){
                                p=i;
                            }
                            i+=2;
                            //Solution possible pour tester la durée d'execution en secondes au lieu de millisecondes :
                            /*Comme il est extrêmement long de tester la fonction pour tous les nombres, dès qu'on
                            trouve un nombre premier, on l'incrémente d'un très grand nombre afin de pouvoir
                            se faire une idée du temps d'execution pour chaque ordre de grandeur. Le résultat précis
                            importe peu, d'autant plus que le temps d'éxécution n'est pas constant, cette solution
                            permet d'avoir rapidement une idée de la rapidité de la fonction à tester.*/
                        }else{
                            i+=2;
                        }
                    }while (1000*((double)t/CLOCKS_PER_SEC)<saisie_d);
                    printf("Le plus grand nombre premier testable en moins de %.3f millisecondes est %d.\n",saisie_d,p);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 3:;
            printf("L'exponentiation modulaire est de la forme a^m mod n.\n\
            Veuillez entrer les entiers a, m et n dans cet ordre.\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                long a;
                long m;
                long n;
                if (sscanf(buffer,"%ld %ld %ld \n",&a,&m,&n)==3){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Exposant m;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();
                    printf("%ld\n",modpow_naive(a,m,n));
                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%ld;%.2lf\n",m,temps);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 4:;
            printf("L'exponentiation modulaire est de la forme a^m mod n.\n\
            Veuillez entrer les entiers a, m et n dans cet ordre.\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                long a,m,n;
                if (sscanf(buffer,"%ld %ld %ld \n",&a,&m,&n)==3){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Exposant m;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();
                    printf("%ld\n",modpow(a,m,n));
                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%ld;%.2lf\n",m,temps);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 5:;
            printf("Veuillez entrer la taille (en bit) minimale, la taille maximale ainsi que le nombre k de tests de Miller-Rabin à effectuer:\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                long borne_inf,borne_sup,nb_p;
                int k;
                if (sscanf(buffer,"%ld %ld %d \n",&borne_inf,&borne_sup,&k)==3){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Nombre premier généré;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();
                    nb_p=random_prime_number(borne_inf,borne_sup,k);
                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%ld;%.2lf\n",nb_p,temps);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 6:;
            printf("Veuillez entrer le message à encoder et décoder :\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                char mess[SIZE_MAIN];
                if (sscanf(buffer,"%[^\n]\n",mess)==1){ //Permet de tout prendre en compte, même les espaces, jusqu'à la fin de la ligne.
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Taille du message;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();
                    // Generation de cle :
                    long p = random_prime_number(3, 7, 5000);
                    long q = random_prime_number(3, 7, 5000);
                    while (p == q)
                    {
                        q = random_prime_number(3, 7, 5000);
                    }
                    long n, s, u;
                    generate_key_values(p, q, &n, &s, &u);
                    // Afichage des cles en hexadecimal
                    printf("cle publique = (%lx, %lx) \n", s, n);
                    printf("cle privee = (%lx, %lx) \n", u, n);
                    // Chiffrement:
                    int len = strlen(mess);
                    long* crypted = encrypt(mess, s, n);
                    printf("Initial message: %s \n", mess);
                    printf("Encoded representation : \n");
                    print_long_vector(crypted, len);
                    // Dechiffrement
                    char* decoded = decrypt(crypted, len, u, n);
                    printf("Decoded : %s \n", decoded);
                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%d;%.2lf\n",len,temps);
                    free(crypted);
                    free(decoded);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 7:;
            //testing init keys
            Key* pKey=malloc(sizeof(Key));
            Key *sKey=malloc(sizeof(Key));
            init_pair_keys(pKey,sKey,3,7);
            printf("pKey : (%lx,%lx) \n",pKey->val1,pKey->val2);
            printf("sKey : (%lx,%lx) \n",sKey->val1, sKey->val2);

            //Testing Key Serialization
            char * chaine=key_to_str(pKey);
            printf("key_to_str : %s \n",chaine);
            Key *k = str_to_key(chaine);
            printf("str_to_key : (%lx,%lx) \n",k->val1,k->val2);
            
            //Testing signature
            //Candidate keys
            Key * pKeyC = malloc(sizeof(Key));
            Key * sKeyC = malloc(sizeof(Key));
            init_pair_keys(pKeyC,sKeyC,3,7);

            //Declaration
            char *messC = key_to_str(pKeyC);
            char *messE=key_to_str(pKey);
            printf("%s vote pour %s \n",messE,messC);
            Signature * sgn=sign(messC,sKey);
            printf("signature : ");
            print_long_vector(sgn->tab,sgn->taille);
            printf("signature_to_str : ");
            free(chaine);
            chaine =signature_to_str(sgn);
            printf("%s\n",chaine);
            printf("str_to_signature : ");
            liberer_signa(sgn);
            sgn=str_to_signature(chaine);
            print_long_vector(sgn->tab,sgn->taille);

            //Testing protected
            Protected * pr= init_protected(pKey,messC,sgn);

            //Verification
            if (verify(pr)){
                printf("Signature valide \n");
            }else{
                printf("Signature non valide \n");
            }
            free(chaine);
            chaine= protected_to_str(pr);
            printf("protected_to_str : %s \n",chaine);
            liberer_protected(pr);
            pr=str_to_protected(chaine);
            char *ktostr=key_to_str(pr->cle_pub);
            char *stostr=signature_to_str(pr->sign);
            printf("str_to_protected : %s %s %s \n",ktostr,pr->mess,stostr);

            liberer_protected(pr);
            free(messE);
            free(pKeyC);
            free(sKeyC);
            free(messC);
            free(chaine);
            free(k);
            free(sKey);
            free(ktostr);
            free(stostr);
            printf("\n");
            break;

            case 8:; 
            printf("Veuillez entrer le nombre d'électeurs ainsi que le nombre de candidats :\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                int nbE,nbC;
                if (sscanf(buffer,"%d %d\n",&nbE,&nbC)==2){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Nombre d'electeurs;Nombre de candidats;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();
                    generate_random_data(nbE,nbC);
                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%d;%d;%.2lf\n",nbE,nbC,temps);
                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 9:;
            generate_random_data(100,5);
            printf("Candidats :\n");
            CellKey **lk1=read_public_keys("candidat.txt");
            print_list_keys(*lk1);
            delete_list_keys(lk1);
            printf("Keys :\n");
            CellKey **lk=read_public_keys("keys.txt");
            print_list_keys(*lk);
            delete_list_keys(lk);
            printf("Declaration :\n");
            CellProtected **l=read_protected("declaration.txt");
            print_list_protected(*l);
            printf("Verify\n");
            verify_list(l); //verifie la validite des signatures et supprime chaque cellule invalide
            print_list_protected(*l);
            delete_list_protected(l);
            break;

            case 10:;
            CellKey **lk11=read_public_keys("keys.txt");
            CellKey **lk12=read_public_keys("candidat.txt");
            CellProtected **l4=read_protected("declaration.txt");
            Key *gagnant=compute_winner(*l4,*lk12,*lk11,12,6008);
            printf("gagnant : (%lx,%lx)\n",gagnant->val1,gagnant->val2);
            delete_list_keys(lk11);
            delete_list_keys(lk12);
            delete_list_protected(l4);
            free(gagnant);
            break;

            case 11:; //Election complete derniere partie
            printf("Veuillez entrer le nombre d'électeurs ainsi que le nombre de candidats :\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                int nbE,nbC;
                if (sscanf(buffer,"%d %d\n",&nbE,&nbC)==2){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Nombre d'electeurs;Nombre de candidats;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }
                    t=clock();

                    generate_random_data(nbE,nbC);

                    CellProtected **liste_pr=read_protected("declaration.txt");
                    CellKey **liste_k=read_public_keys("keys.txt");
                    CellKey **liste_c=read_public_keys("candidat.txt");
                    Key *k_author=(*liste_k)->data;
                    CellTree *tree=NULL;
                    CellTree *tree2=NULL;
                    char name[256];
                    char id_block[20];
                    int cpt=0;
                    CellProtected *courant=*liste_pr;

                    while(courant){
                    submit_vote(courant->data);
                    if (cpt%10==0){
                        tree2=create_block(tree,k_author,2);
                        sprintf(id_block,"%d",cpt/10);
                        strcpy(name,"Block");
                        strcat(name,id_block);
                        add_block(2,name);
                        tree=tree2;
                    }
                    cpt++;
                    courant=courant->next;
                    }
                    CellTree *final_tree=read_tree();
                    print_tree(final_tree);

                    /* for(int k=0;k<=cpt;k++){     //pour supprimer tous les blocks de ton repertoire si tu as la flemme de le faire à la main
                    if (k%10==0){
                        sprintf(id_block,"%d",k/10);
                        strcpy(name,"Blockchain/Block");
                        strcat(name,id_block);
                        remove(name);
                    }
                    }*/

                    Key *president=compute_winner_BT(final_tree,*liste_c,*liste_k,12,500);
                    printf("\n\nPRESIDENT (%lx,%lx)\n",president->val1,president->val2);

                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%d;%d;%.2lf\n",nbE,nbC,temps);

                    free(president);
                    delete_list_protected(liste_pr);
                    delete_list_keys(liste_k);
                    delete_list_keys(liste_c);
                    delete_tree(final_tree);

                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

            case 12:;
            printf("Veuillez entrer l'entier d (nombre de zéros au début du hash du bloc):\n\
            Attention à ne pas prendre une valeur trop grande (> 6-7), c'est rapidement très long.\n");
            if (fgets(buffer,SIZE_MAIN, input)){
                int d;
                if (sscanf(buffer,"%d\n",&d)==1){
                    if (rep!=rep_prec){ /*On regarde si l'action appelée est la même 
                        que la précédente (si différent on rajoute un en tête)*/
                        fprintf(f,"Entier d;Durée d'exécution\n");
                        //On ajoute l'en-tête des colonnes
                    }

                    Block* B = read_block("Pending_block.txt");
                    t=clock();

                    compute_proof_of_work(B,d);

                    t=clock()-t;
                    temps = UNITE_TEMPS*((double)t/CLOCKS_PER_SEC);
                    fprintf(f,"%d;%.2lf\n",d,temps);

                    free(B);

                }else{
                    printf("Erreur format\n");
                }
            }else{
                printf("Aucun argument\n");
            }
            break;

        }
        rep_prec=rep; /*rep_prec permet de voir si l'option appelée est différente de la précédente.
        Si c'est le cas, rajoute un nouvel en tête dans le fichier csv pour ne pas mélanger les valeurs.*/
    }while(rep!=0);
    printf("Merci, et au revoir\n");
    fclose(f);
    if(INPUT==1){
        fclose(input);
    }
    return 0;
}

int main(){
    actionUtilisateur();
    return 0;
}