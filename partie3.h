#ifndef PARTIE3
#define PARTIE3
#include "partie2.h"

typedef struct cellKey {
    Key *data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected{
    Protected * data;
    struct cellProtected *next;
}CellProtected;

typedef struct hashcell {
  Key *key;
  int val;
} HashCell;

typedef struct hashtable{
  HashCell ** tab;
  int size;
}HashTable;

CellKey * create_cell_key(Key *key);

void append_head_list(CellKey **liste, Key *key);

CellKey ** read_public_keys(char *nomfichier);

void print_list_keys(CellKey *LCK);

void delete_cell_keys(CellKey *c);

void delete_list_keys(CellKey **l);

CellProtected *create_cell_protected(Protected *pr);

void append_head_listp(CellProtected **l,Protected *pr);

CellProtected ** read_protected(char *nomfichier);

CellProtected ** read_protected_block(FILE *nomfichier);

CellProtected ** reverse(CellProtected *liste);

void print_list_protected(CellProtected *LCK);

void delete_cellprotected(CellProtected *c);

void delete_list_protected(CellProtected **l);

void verify_list(CellProtected **liste);

HashCell * create_hashcell(Key *key);

void delete_hashcell(HashCell *h);

int hash_function(Key *key, int size);

int find_position(HashTable *t, Key *key);

int key_equal(Key *key1, Key *key2);

int find_index(HashTable *t, Key *key);

HashTable * create_hashtable(CellKey *keys, int size);

void delete_hashtable(HashTable *t);

Key * compute_winner(CellProtected *decl,CellKey * candidates,CellKey *voters, int sizeC, int sizeV);

#endif