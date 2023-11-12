#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "partie1.h"

int is_prime_naive(long p){
    //renvoie 1 si p est premier, 0 sinon
    if (p%2==0){
        return 0;
    }
    for (int i=3;i<p;i=i+2){
        if (p%i==0){
            return 0;
        }
    }
    return 1;
}

long modpow_naive(long a,long m,long n){
    int i;
    long val=a%n;
    for (i=1;i<m;i++){
        val*=a;
        val=val%n;
    }
    return val;
}

long modpow(long a,long m,long n){
    if (m<0){
        printf("La valeur de m doit être positive");
        return -1;
    }
    if (m==0){
        return 1%n;
    }
    if (m==1){
        return a%n;
    }
    if (m%2){ //cas m impair
        return (a*modpow((a*a)%n,(m-1)/2,n))%n; //on fait le modulo dans l'appel
    }else{ //cas m pair
        return (modpow((a*a)%n,m/2,n));
    }
}

int witness(long a, long b, long d, long p)
{
    long x = modpow(a, d, p);
    if (x == 1)
    {
        return 0;
    }
    for (long i = 0; i < b; i++)
    {
        if (x == p - 1)
        {
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up)
{
    return rand() % (up - low + 1) + low;
}

int is_prime_miller(long p, int k)
{
    if (p == 2)
    {
        return 1;
    }
    if (!(p & 1) || p <= 1)
    { // on verifie que p est impair et different de 1 
        return 0;
    }
    // on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1))
    { // tant que d n’est pas impair
        d = d / 2;
        b = b + 1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a;
    int i;
    for (i = 0; i < k; i++)
    {
        a = rand_long(2, p - 1);
        if (witness(a, b, d, p))
        {
            return 0;
        }
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k){
    long rand;
    do{
        rand=rand_long(pow(2,low_size-1),pow(2,up_size)-1);
    }while(!is_prime_miller(rand,k));
    return rand;
}

long extended_gcd(long s, long t, long *u, long *v)
{
    if (s == 0)
    {
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim - (t/s)*uPrim;
    *v = uPrim;
    return gcd;
}

void generate_key_values(long p,long q,long* n, long* s, long* u){
    *n=p*q;
    long gcd, v, t= (p-1)*(q-1);
    do{
        *s=rand_long(1,t-1); //génération nb alétoire inférieur à t
        gcd=extended_gcd(*s,t,u,&v);
    }while(gcd != 1);
    if (*u < 0)
    {
        long r = (p - 1) * (q - 1);
        *u = *u + r; // on aura toujours s*u mod r = 1
    }
}

long* encrypt(char* chaine, long s, long n){
    int len = strlen(chaine);
    long* tab_encrypt = malloc(len*sizeof(long));
    for (int i=0;i<len;i++){
        tab_encrypt[i]=modpow((long)chaine[i], s, n);
    }
    return tab_encrypt;
}

char* decrypt(long* crypted, int size, long u, long n){
    char* chaine_decrpyt = malloc((size+1)*sizeof(char)); //+1 pour rajouter \0 à la fin
    for (int i=0;i<size;i++){
        chaine_decrpyt[i]=(char)modpow(crypted[i],u,n);
    }
    chaine_decrpyt[size]='\0';
    return chaine_decrpyt;
}

void print_long_vector(long *result, int size)
{
    printf("Vector: [");
    for (int i = 0; i < size; i++)
    {
        printf("%lx \t", result[i]);
    }
    printf("]\n");
}