/*
 * cipher.h - header file for the cipher algorithm
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

#ifndef  _CIPHER_H
#define  _CIPHER_H

#define BLOCK_SIZE 0x20 /* 32 bytes (256 bits) */
#define MIN_ROUNDS 0x10 /* minimum of 16 rounds */
#define KEY_LENGTH BLOCK_SIZE / 2

#define RL(x, y) (x << y) | (x >> (8 - y))
#define RR(x, y) (x >> y) | (x << (8 - y))

typedef unsigned char byte_t;

void
encrypt_block(byte_t *, byte_t **, int);

void
decrypt_block(byte_t *, byte_t **, int);

byte_t **
expand_key(const byte_t *, int *);

void
hex_print(byte_t *buffer, int size);

int
rounds_from_key(const byte_t * key);

void
destroy_subkeys(byte_t **subkeys, int rounds);

void
destroy_key(byte_t *key);

#endif /*_CIPHER_H*/