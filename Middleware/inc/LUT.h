/**
 * @author	Artjom Siebert
 * @date	26.12.2014
 *
 * @brief	This code is typical of what might be found in the symbol table management routines
 * 			of a macro processor or a compiler. There are two routines that manipulate the names and replacement texts.
 *			The algorithm is a hash search - the incoming name is converted into a small non-negative integer,
 *			which is then used to index into an array of pointers.
 *
 *			The hashing function, which is used by both lookup and install, adds each character value
 *			in the string to a scrambled combination of previous ones and returns the remainder modulo the array size.
 *			(This is not the best posible hash funtion, but it is short and effective)
 *
 * 			This algorithm is given by Brian W.Kernighan and Dennis M.Ritchie.
 */

#ifndef	LUT_H_
#define LUT_H_
#include <string.h>
#include <stdlib.h>

#define HASHSIZE	101

struct nlist {				/**<table entry*/
	struct nlist *next;		/**<defined name*/
	char *name;				/**<defined name */
	char *defn;				/**<replacement text */
};

static struct nlist *hashtab[HASHSIZE];		/**< pointer table */

unsigned hash(char*);
struct nlist *install(char *, char *);
struct nlist* lookup(char *);

#endif
