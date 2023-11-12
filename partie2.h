#ifndef PARTIE2
#define PARTIE2

typedef struct key {
    long val1;
    long val2;
} Key;

typedef struct signature{
    long *tab;
    int taille;
} Signature;

typedef struct protected {
    Key* cle_pub;
    char* mess;
    Signature* sign;
} Protected;

void init_key(Key *key, long val, long n);

void init_pair_keys(Key* pKey, Key* sKey,long low_size, long up_size);

char* key_to_str (Key* key);

Key* str_to_key(char *str);

void liberer_signa(Signature *signa);

Signature* init_signature(long * content, int size);

Signature* sign(char* mess, Key * sKey);

char * signature_to_str(Signature *sgn);

Signature* str_to_signature(char *str);

Protected *init_protected(Key *pKey, char *mess, Signature *sgn);

void liberer_protected(Protected *pr);

int verify(Protected *pr);

char* protected_to_str(Protected *pr);

Protected * str_to_protected(char *str);

void initaliser_tableau(int *tab, int taille);

int est_present(int *tab,int a, int taille);

void liberer_signa(Signature *signa);

void generate_random_data(int nv,int nc);

#endif