/*
 * hash.h - header file for the hash function
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

#ifndef  _HASH_H
#define  _HASH_H

typedef unsigned char byte_t;

byte_t *
round_function(byte_t *subkey, byte_t *input_block);

#endif /*_HASH_H*/