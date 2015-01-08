/**
 * @author	Artjom Siebert
 * @date	26.12.2014
 */

#include "LUT.h"

/*hash: form hash value for string s*/

unsigned hash(char* s) {
	unsigned hashVal;

	for (hashVal = 0; *s != '\0'; s++)
		hashVal = *s + 31 * hashVal;
	return hashVal % HASHSIZE;
}

/*lookup: look for s in hashtab */
struct nlist* lookup(char *s) {
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0){
			return np; /*found*/
		}

	}
	return NULL; /*not found*/

}

/*install: put(name, defn) in hashtab*/
struct nlist *install(char *name, char *defn) {
	struct nlist *np;
	unsigned hashVal;

	if ((np = lookup(name)) == NULL) { /*Not found*/
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashVal = hash(name);
		np->next = hashtab[hashVal];
		hashtab[hashVal] = np;

	} else
		/*already exists*/
		free((void*) np->defn); /*free previous defn*/
	if ((np->defn = strdup(defn)) == NULL)
		return NULL;

	return np;

}
