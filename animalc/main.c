/*
 * Jogo dos Animais
 * Demonstracao de aplicacao de conceito Objeto em C puro.
 * Logica do jogo dos animais (desconsiderando input e construcao de objetos) composta apenas 
 * de um loop e sem o uso de ifs,switchs.
 *
 * Autor: Danny Angelo Carminati Grein
 * Tamanho: 7.168 bytes o binario puro e 5.632 bytes o binario compactado.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Input/Output
void pause(const char *);
void readline(unsigned char *, unsigned int);
unsigned short pausesn(const char *);

// exemplo de definicao do objeto
typedef struct node {
	char *property;
	struct node *noyes[2];
	struct node *root, **refeer;

	void (*setYes)(struct node *, struct node *);
	void (*setNo)(struct node *, struct node *);

	struct node *(*getYes)(struct node *);
	struct node *(*getNo)(struct node *);

	struct node *(*ask)(struct node *);
	struct node *(*create)(struct node *);

	struct Getters{	struct node *(*func)(struct node *); } getters[2];
} t_node;

t_node *node_ctor(const char *str);
t_node *link_ctor(const char *str);
t_node *animal_ctor(const char *str, t_node *root);

// metodos
void node_setYes(t_node *this, t_node *who)
{
	this->noyes[1] = who;
	who->refeer = &this->noyes[1];
}

void node_setNo(t_node *this, t_node *who)
{
	this->noyes[0] = who;
	who->refeer = &this->noyes[0];
}

t_node *node_getYes(t_node *this)
{
	return this->noyes[1];
}

t_node *node_getNo(t_node *this)
{
	return this->noyes[0];
}

t_node *node_ask(t_node *this)
{
	fprintf(stdout, "%s\n", this->property);
	return this->getters[pausesn("[S/s]im ou [N/n]ao?\n")].func(this);
}

// Objeto Animal
t_node *animal_getNo(t_node *this)
{
	t_node *tmp;

	tmp = this->create(this);
	*this->refeer = tmp;
	tmp->setNo(tmp, this);

	return this->noyes[1];
}

t_node *animal_ask(t_node *this)
{
	fprintf(stdout, "Eh um %s?\n", this->property);
	return this->getters[pausesn("[S/s]im ou [N/n]ao?\n")].func(this);
}

t_node *animal_create(t_node *this)
{
	unsigned char str[1024];
	t_node *q, *a;

	fprintf(stdout, "Desisto. Faca uma pergunta sobre o animal no qual voce pensou: \n");
	readline(str, 1024);

	q = node_ctor((const char*)str);

	fprintf(stdout, "Qual eh o animal?\n");
	readline(str, 1024);

	a = animal_ctor((const char *)str, this->root); 
	q->setYes(q, a);

	return q;
}

// Objeto Link
t_node *link_ask(t_node *this)
{
	fprintf(stdout, "%s\n", this->property);
	pause("");

	return this->noyes[1];
}

// Contructors
t_node *base_ctor(const char *str)
{
	t_node *this = (t_node *)malloc(sizeof(t_node));
	this->setNo = node_setNo;
	this->setYes = node_setYes;
	this->create = NULL;
	this->noyes[0] = NULL;
	this->noyes[1] = NULL;
	this->refeer = NULL;
	this->root = NULL;
	this->property = strdup(str);
	this->getters[0].func = node_getNo;
	this->getters[1].func = node_getYes;

	return this;
}

t_node *node_ctor(const char *str)
{
	t_node *n;

	n = base_ctor(str);      
	n->ask = node_ask;

	return n;   
}

t_node *link_ctor(const char *str)
{
	t_node *n;

	n = base_ctor(str);
	n->ask = link_ask;      

	return n;   
}

t_node *animal_ctor(const char *str, t_node *root)
{
	t_node *n;

	n = base_ctor(str);
	n->root = root;

	n->create = animal_create;

	// Override dos metodos
	n->ask = animal_ask;
	n->getters[0].func = animal_getNo;

	n->noyes[1] = node_ctor("Aha! Descobri! Deseja continuar?");
	n->noyes[1]->setYes(n->noyes[1], root);

	return n;
}

// Base destructor
void base_dtor(t_node *this, t_node *root)
{
	if (this == NULL || this == root)
		return;

	base_dtor(this->noyes[1], root);
	base_dtor(this->noyes[0], root);
	free(this->property);
	free(this);

	this = NULL;
}

// Link destructor
void link_dtor(t_node *this)
{
	base_dtor(this->noyes[1], this);
	free(this->property);
	free(this);
}

int main(int argc, char *argv[])
{
	t_node *first, *elefante, *tubarao, *curr, *root;

	root = link_ctor("Pense em um animal e pressione enter para continuar.");   

	// Criacao dos elementos iniciais da arvore.
	first = node_ctor("Eh mamifero?");
	elefante = animal_ctor("Elefante", root);
	tubarao = animal_ctor("Tubarao", root);

	// Construcao da arvore inicial.
	first->setYes(first, elefante);
	first->setNo(first, tubarao);
	root->setYes(root, first);

	curr = root;
	while (curr != NULL) curr = curr->ask(curr);
	link_dtor(root);

	return 0;
}

// Input/Output bem basico..
void pause(const char *ask)
{
	unsigned char ch;

	fprintf(stdout, "%s", ask);
	fflush(stdin);
	ch = fgetc(stdin);
	fflush(stdin);
}

void readline(unsigned char *buf, unsigned int size)
{
	unsigned char ch = 0;
	unsigned int i = 0;

	memset(buf, '\0', size);
	while (ch != 10 && i < size-1)
	{
		fflush(stdin);
		ch = fgetc(stdin);
		buf[i++] = ch;
	}

	buf[--i] = '\0';
	fflush(stdin);
}

unsigned short pausesn(const char *ask)
{
	unsigned char ch = 0;

	while (ch != 's' && ch != 'n')
	{
		fprintf(stdout, "%s", ask);
		fflush(stdin);
		ch = fgetc(stdin);
		fflush(stdin);

		ch = tolower(ch);
	}

	return ch&0x01;
}
