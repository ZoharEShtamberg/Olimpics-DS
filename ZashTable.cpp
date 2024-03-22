#include "ZashTable.h"
ZashTable::ZashTable(): size(0), capacity(1), table(new Node*[1]) {
	table[0] = nullptr;
}
ZashTable::~ZashTable() {
	for(int i = 0; i < capacity; i++) {
		if(table[i] != nullptr) {
			deleteList(table[i]);
		}
		delete[] table;
	}
}
void ZashTable::deleteList(Node*  Node) {
	if( Node->next != nullptr) {
		deleteList( Node->next);
	}
	delete  Node;
}

void ZashTable::insert(Team* team) {
}