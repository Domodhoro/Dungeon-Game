#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

// Contador global para o número de alocações de memória.
static int mallocCounter = 0;

// Função que encapsula o 'malloc' e incrementa o contador.
void *myMalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
    	// Incrementa o contador quando a alocação é bem-sucedida.
        mallocCounter++;
    }
    return ptr;
}

// Função para obter o contador de alocações.
int getMallocCount() {
    return mallocCounter;
}

// Função que encapsula o 'free' e decrementa o contador.
void myFree(void *ptr) {
    if (ptr) {
        mallocCounter--; 
        free(ptr);
    }
}

#endif // MEMORY_MANAGER_H