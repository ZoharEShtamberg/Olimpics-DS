//Zohar's Hash Table
#ifndef ZASHTABLE_H_
#define ZASHTABLE_H_
template <class T, class HashFunction>
class ZashTable {
	public:
		ZashTable(int size);
		~ZashTable();
		void insert(T* item);
		T* remove(T* item);
		bool find(T* item);
	private:
		static const float LOAD_FACTOR = 0.5;	//expand when size/capacity > LOAD_FACTOR, shrink when 2*size/capacity < LOAD_FACTOR
		T* table;
		int size;
		int capacity;
		HashFunction hash; // hash: T -> int
		
};
#endif // ZASHTABLE_H_