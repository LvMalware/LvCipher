/*
 * hash.h - header file for the hash function
 *
 * Copyright (C) 2021 Lucas V. Araujo <lucas.vieira.ar@disroot.org> .
 *
 * This is free software: you are free to change and redistribute it under the
 * terms of GNU GPL license version 3 or, at your choice, any later version.
 * You should have received a copy of the license along with this software. If
 * not, you can read the full text at https://gnu.org/licenses/gpl.html.
 * This software is offered AS IT IS, in hope that it will be usefull, but there
 * is NO WARRANTY, to the extent permitted by law.
 */

#ifndef  _HASH_H
#define  _HASH_H

#include <stdint.h>

typedef unsigned char byte_t;

/* Left rotation */
#define ROTL(x, y) ((x << y) & 0xFFFFFFFF) | (x >> (32 - y))
/* Right rotation */
#define ROTR(x, y) (x >> y) | (x << (32 - y))
/* Rotation with direction depending on a number K */
#define ROTK(K, x, y) (K & 1) ? ROTL(x, y) : ROTR(x, y);

#define UH(A, B) (A << 16) | (B >> 16)
#define LH(A, B) (B << 16) | (A >> 16)

/* Temper functions: W and X */
#define W(A, B, C, D) (LH(A, B) ^ UH(A, B)) + (LH(C, D) ^ UH(C, D))
#define X(A, B, C, D) (LH(A, B) + UH(A, B)) ^ (LH(C, D) + UH(C, D))

/* A round of the hashing function consists on the following: */
#define HASH_ROUND \
    K += W(A, B, C, D) & X(E, F, G, H);\
    A += ROTK(K, W(A, E, B, F), G & 31);\
    B += ROTK(A, X(B, F, C, G), H & 31);\
    C += ROTK(B, W(C, G, D, H), E & 31);\
    D += ROTK(C, X(D, H, A, E), F & 31);\
    E += ROTK(D, W(E, A, F, B), C & 31);\
    F += ROTK(E, X(F, B, G, C), D & 31);\
    G += ROTK(F, W(G, C, H, D), A & 31);\
    H += ROTK(G, X(H, D, E, A), B & 31);

static inline void
round_function(byte_t *hash, byte_t *subkey, byte_t *input_block)
{
    uint32_t *block = (uint32_t *) input_block;
    uint32_t *sbkey = (uint32_t *) subkey;
    uint32_t *uhash = (uint32_t *) hash;
    uint32_t A      = block[0];
    uint32_t B      = block[1];
    uint32_t C      = block[2];
    uint32_t D      = block[3];
    uint32_t E      = sbkey[0];
    uint32_t F      = sbkey[1];
    uint32_t G      = sbkey[2];
    uint32_t H      = sbkey[3];
    uint32_t K      = 0;
    
    /* 8 rounds should be enough... */

    HASH_ROUND HASH_ROUND
    HASH_ROUND HASH_ROUND
    HASH_ROUND HASH_ROUND
    HASH_ROUND HASH_ROUND
    
    uhash[0]        = A;
    uhash[1]        = B;
    uhash[2]        = C;
    uhash[3]        = D;
}

#endif /*_HASH_H*/