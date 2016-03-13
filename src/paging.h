#ifndef PAGING_H
#define PAGING_H

#include "isr.h"
#include "common.h"

typedef struct page
{
	unsigned int present 	: 1;	//If the page is present in the memory
	unsigned int rw			: 1; 	//0:Read-only 1:Read-write
	unsigned int user		: 1; 	//0:Supervisor level only
	unsigned int accessed	: 1; 	//If the page has been accessed since refresh
	unsigned int dirty		: 1; 	//If the page has been written to since refresh
	unsigned int unused		: 1; 	//Reserved and unused bits
	unsigned int frame		: 20;	//Frame address	
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t* tables[1024];
	unsigned int tablesPhysical[1024];
	unsigned int physicalAddr;
} page_directory_t;

void initialize_paging();

void switch_page_directory(page_directory_t* new);

page_t* get_page(unsigned int address, bool make, page_directory_t* dir);

void page_fault(registers_t);

#endif