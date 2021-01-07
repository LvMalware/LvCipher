/*
 * hash.c - source for the hash function
 *
 * Copyright (C) 2020 Lucas V. Araujo <lucas.vieira.ar@disroot.org> .
 *
 * This is free software: you are free to change and redistribute it under the
 * terms of GNU GPL license version 3 or, at your choice, any later version.
 * You should have received a copy of the license along with this software. If
 * not, you can read the full text at https://gnu.org/licenses/gpl.html.
 * This software is offered AS IT IS, in hope that it will be usefull, but there
 * is NO WARRANTY, to the extent permitted by law.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "hash.h"

/* Left rotation */
#define ROTL(x, y) ((x << y) & 0xFFFFFFFF) | (x >> (32 - y))
/* Right rotation */
#define ROTR(x, y) (x >> y) | (x << (8 * sizeof(x) - y))

#define UH(A, B) (A << 16) | (B >> 16)
#define LH(A, B) (B << 16) | (A >> 16)

/* Temper functions: W, X, Y, Z */
#define W(A, B, C, D) (A + B) ^ (A + C) ^ (A + D) ^ (B + C) ^ (B + D) ^ (C + D)
#define X(A, B, C, D) (A ^ B) + (A ^ C) + (A ^ D) + (B ^ C) + (B ^ D) + (C ^ D)
#define Y(A, B, C, D) (LH(A, B) ^ UH(A, B)) + (LH(C, D) ^ UH(C, D))
#define Z(A, B, C, D) (LH(A, B) + UH(A, B)) ^ (LH(C, D) + UH(C, D))

#define HASH_ROUND K += A % (i * 1 + 2) + B % (i * 2 + 3) + C % (i * 3 + 5) +\
    D % (i * 4 + 2) + E % (i * 5 + 3) + F % (i * 6 + 5) +\
    G % (i * 7 + 2) + H % (i * 8 + 3);\
    E = E + ROTL(A + LH(E, K), (K * i + 1) % 32);\
    F = F + ROTR(B + LH(F, K), (K * i + 1) % 32);\
    G = G + ROTL(C + LH(G, K), (K * i + 1) % 32);\
    H = H + ROTR(D + LH(H, K), (K * i + 1) % 32);\
    A = A + ROTL(E + UH(A, K), (K * i + 1) % 32);\
    B = B + ROTR(F + UH(B, K), (K * i + 1) % 32);\
    C = C + ROTL(G + UH(C, K), (K * i + 1) % 32);\
    D = D + ROTR(H + UH(D, K), (K * i + 1) % 32);\
    E = W(Z(E, F, G, H), A, B, C);\
    F = X(Y(F, G, H, E), B, C, D);\
    G = Y(X(G, H, E, F), C, D, A);\
    H = Z(W(H, E, F, G), D, A, B);\
    A = W(Z(A, B, C, D), E, F, G);\
    B = X(Y(B, C, D, A), F, G, H);\
    C = Y(X(C, D, A, B), G, H, E);\
    D = Z(W(D, A, B, C), H, E, F);\
    i = i + 1;

void
round_function(byte_t *hash, byte_t *subkey, byte_t *input_block)
{
    int i;
    uint32_t *block = (uint32_t *) input_block;
    uint32_t *sbkey = (uint32_t *) subkey;
    uint32_t A      = block[0];
    uint32_t B      = block[1];
    uint32_t C      = block[2];
    uint32_t D      = block[3];
    uint32_t E      = sbkey[0];
    uint32_t F      = sbkey[1];
    uint32_t G      = sbkey[2];
    uint32_t H      = sbkey[3];
    uint32_t K      = 0;
    
    HASH_ROUND HASH_ROUND HASH_ROUND HASH_ROUND
    HASH_ROUND HASH_ROUND HASH_ROUND HASH_ROUND
    HASH_ROUND HASH_ROUND HASH_ROUND HASH_ROUND 
    HASH_ROUND HASH_ROUND HASH_ROUND HASH_ROUND

    block    = (uint32_t *) hash;
    block[0] = ROTR(A, E % 32);
    block[1] = ROTL(B, F % 32);
    block[2] = ROTR(C, G % 32);
    block[3] = ROTL(D, H % 32);
}