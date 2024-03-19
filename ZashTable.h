//Zohar's Hash Table
#ifndef ZASHTABLE_H_
#define ZASHTABLE_H_
#include "Team.h"
class ZashTable {
	public:
		ZashTable();
		~ZashTable();
		void insert(Team* item);
		Team* remove(Team* item);
		bool find(Team* item);
	private:
		class Node{
			public:
				Team* data;
				Node* next;
				Node(Team* data, Node* next):data(data),next(next){}
		};
		void deleteList(Node* head);

		static const float LOAD_FACTOR = 0.5;	//expand when size/capacity > LOAD_FACTOR, shrink when 2*size/capacity < LOAD_FACTOR
		int size;
		int capacity;
		Node** table;

		void expand();
		void shrink();
		int hash(Team* item);
		int hash(int teamID);
};
#endif // ZASHTABLE_H_