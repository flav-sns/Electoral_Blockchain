#ifndef PARTIE4
#define PARTIE4
#include "partie2.h"
#include "partie3.h"

typedef struct block {
    Key *author;
    CellProtected *votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

typedef struct block_tree_cell{
  Block *block;
  struct block_tree_cell* father;
  struct block_tree_cell* firstChild;
  struct block_tree_cell* nextBro;
  int height;
}CellTree;

void print_list_protected_file(CellProtected *LCK,FILE *f);

unsigned char * SHA_256(const char *c);

char * block_to_str(Block * block);

void write_block(Block *b, char* file);

Block * read_block(char *file);

void delete_block(Block *b);

void delete_block_all(Block *b);

void compute_proof_of_work_bis(Block *B, int d);

void compute_proof_of_work(Block *B, int d);

int verify_block(Block *b, int d);

CellTree *create_node(Block * b);

int update_height(CellTree *father, CellTree *child);

void add_child(CellTree *father, CellTree *child);

void print_tree(CellTree *father);

void delete_node(CellTree *node);

void delete_tree(CellTree *father);

CellTree *hightest_child(CellTree *cell);

CellTree *last_node(CellTree *tree);

CellProtected * fusion(CellProtected *liste1, CellProtected *liste2);

CellProtected *fusion_liste(CellTree *tree,CellProtected *liste_courante);

void submit_vote(Protected *p);

CellTree * create_block(CellTree *tree, Key * author,int d);

void add_block(int d, char* name);

CellTree *read_tree();

Key *compute_winner_BT(CellTree * tree, CellKey* candidates, CellKey *voters, int sizeC, int sizeV);



#endif