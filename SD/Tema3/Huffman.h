#include <stdio.h>

typedef struct tagSerialHuffmanNode 
{
	uint8_t isTerminal;
	union 
	{
		uint8_t colorIndex;
        	struct 
		{
			uint16_t leftChld;
			uint16_t rightChld;
		} __attribute__((__packed__)) chldData;
	} __attribute__((__packed__));
} __attribute__((__packed__)) SerialHuffmanNode, *PSerialHuffmanNode;

// structura asemanatoare lui tagSerialHuffmanNode cu cateva campuri in plus
typedef struct tagHuffmanDerivat 
{
	uint8_t isTerminal;
	uint32_t frecv; // frecventa de aparitie
	uint16_t index; // indicele nodului in vectorul de structuri Huffman
	uint16_t parinte; // indicele parintelui nodului curent
	uint8_t tipnod; // retine 0 sau 1 in functie de ce fel de fiu e nodul
			// curent pentru parintele sau
	union 
	{
		uint8_t colorIndex;
        	struct 
		{
			uint16_t leftChld;
			uint16_t rightChld;
		} __attribute__((__packed__)) chldData;
	} __attribute__((__packed__));
} __attribute__((__packed__)) HuffmanDerivat, *PHuffmanDerivat;

// structura care asociaza fiecarui pixel unic care apare in sirul de pixel 
// reprezentat de indicele vectorului de structuri de acest tip cu dim 256,
// frecventa sa si codul binar obtinut parcurgand arborele de la frunza la
// radacina
typedef struct tagcodificare
{
	uint32_t frecv;
	char cod[100];
} __attribute__((__packed__)) codificare, *Pcodificare;
