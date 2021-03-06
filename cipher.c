/*
 * cipher.c - source for the cipher algorithm
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/cipher.h"
#include "lib/hash.h"

void
str_xor(byte_t *half_block, byte_t *hash)
{
    register int i;
    for (i = 0; i < KEY_LENGTH; i ++)
        half_block[i] ^= hash[i];
}

void
encrypt_block(byte_t *block, byte_t **subkeys, int rounds)
{
    byte_t *L, *R, *A;
    byte_t hash[KEY_LENGTH];
    L = block;
    R = &block[KEY_LENGTH];

    register int i;
    for (i = 0; i < rounds; i ++)
    {
        round_function(hash, subkeys[i], L);
        str_xor(R, hash);
        A = R;
        R = L;
        L = A;
    }
}

void
decrypt_block(byte_t *block, byte_t **subkeys, int rounds)
{
    byte_t *L, *R, *A;
    byte_t hash[KEY_LENGTH];
    L = block;
    R = &block[KEY_LENGTH];
    
    register int i;
    for (i = rounds - 1; i >= 0; i --)
    {
        if (rounds & 1)
        {
            round_function(hash, subkeys[i], L);
            str_xor(R, hash);
        }
        else
        {
            round_function(hash, subkeys[i], R);
            str_xor(L, hash);
        }
        A = R;
        R = L;
        L = A;
    }
}

int
rounds_from_key(const byte_t * key)
{
    int i, j, extra_rounds = 0;
    j = 3;
    for (i = 0; i < strlen(key); i++)
    {
        extra_rounds += 1 + key[i] * j;
        j = (j < 7) ? j + 2 : 3;
    }
    extra_rounds %= 17;
    return MIN_ROUNDS + extra_rounds;
}

byte_t **
expand_key(const byte_t *key, int *rounds)
{
    *rounds         = rounds_from_key(key);
    int total_bytes = *rounds * KEY_LENGTH;
    byte_t *subkeys = (byte_t *) calloc(total_bytes, sizeof(byte_t));
    int copy_length = strlen(key);
    if (copy_length > total_bytes)
        copy_length = total_bytes;
    memcpy(subkeys, key, copy_length);
    
    int i, j;
    byte_t h = 0x1F;
    for (i = 0; i < strlen(key); i ++)
    {
        h *= key[i];
        h += key[i];
    }
    for (j = 0; j < copy_length; j ++)
    {
        for (i = 0; i < total_bytes; i ++)
        {
            subkeys[i] ^= RL(subkeys[j] + i, h & 0x7);
            subkeys[j] += RR(subkeys[i] + j, h & 0x7);
            subkeys[i] ^= RL(subkeys[j] + h, j & 0x7);
            subkeys[j] += RR(subkeys[i] + h, i & 0x7);
        }
        h *= subkeys[j];
        h += subkeys[j];
    }
    byte_t **result = calloc(*rounds, sizeof(byte_t *));
    for (i = 0; i < *rounds; i ++)
        result[i] = &subkeys[i * KEY_LENGTH];
    return result;
}

void
destroy_key(byte_t *key)
{
    memset(key, 0, strlen(key));
    free(key);
}

void
destroy_subkeys(byte_t **subkeys, int rounds)
{
    memset(subkeys[0], 0, KEY_LENGTH * rounds);
    free(subkeys[0]);
    free(subkeys);
}