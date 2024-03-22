#ifndef AVLTree_H
#define AVLTree_H
#include <cassert>
#include <cmath>
#include "Exceptions.h"
#include "StupidArr.h"
#include "Team.h"
#ifndef NDEBUG
#include <vector>
#include <algorithm>
#endif	//NDEBUG
typedef Team* T;
//typedef TeamComparator COMP;


inline int max(int a, int b) { //its better to use std::max
	return a > b ? a : b;
}

//COMP should overload the () operator to compare two Ts
//COMP (T,T) -> int
//Comp(a,b)=-1 if a < b
//Comp(a,b)=0 if a = b
//Comp(a,b)=1 if a > b
 
class AVLTree {
public:
	class  Node {
	public:
		T key;
		int height;
		 Node* left;
		 Node* right;
		 Node(T k) :key(k), height(0), left(nullptr), right(nullptr) {}
	};
	AVLTree() : n(0), root(nullptr), comp() {}

	~AVLTree() {
		destroy(root);
	}

	void insert(T x) {
		root = insertUtil(root, x);
	}

	void remove(T x) {
		root = removeUtil(root, x);
	}
	template <typename K>
	T& search(K x) const {// do i want to return the  Node or the key?
		return searchUtil(root, x)->key;
	}
	template <typename K>
	bool find(K x) const {
		try {
			search(x);
			return true;
		}
		catch (KeyDoesNotExistException&) {
			return false;
		}
	}

	int size() const {
		return n;
	}

	bool empty() const {
		return n == 0;
	}

	 Node* getMax() const {
		 Node* temp = root;
		if (temp == nullptr) {
			throw EmptyTreeException();
		}
		while (temp->right != nullptr) {
			temp = temp->right;
		}
		return temp;
	}

	 Node* getMin() const {
		 Node* temp = root;
		if (temp == nullptr) {
			throw EmptyTreeException();
		}
		while (temp->left != nullptr) {
			temp = temp->left;
		}
		return temp;
	}

	StupidArr<T> treeToArray() const {	//memory should be deleted by the user
		StupidArr<T> result(n);
		treeToArrayUtil(root, result.arr);
		return result;
	}
	/// @brief replaces the current tree with a new tree that has the same elements as the array
	/// @param arr sorted StupidArr of Ts, will not be deleted, will take arr.size Ts from the array
	void arrayToTree(StupidArr<T> arr) {	//if theres an error here, its probably an off by one error
		if(arr.size==0) {return;}
		int height = std::ceil(std::log2(arr.size + 1)) - 1;	//bruh moment indeed
		 Node* newRoot = createFullTree(height);
		assert(newRoot != nullptr);
		newRoot = removeNNodes(treeRemovalUtil(newRoot, std::exp2(height+1)-1-arr.size)).head;	// remove redundant  Nodes
		arrayToTreeUtil(arr.arr, newRoot);	// insert the array into the tree
		n = arr.size;
		destroy(root);
		root = newRoot;
	}

	

	// void unite(AVLTree& other) {	//do i need this?
	// 	StupidArr<T> mergedArr = mergeArrays(treeToArray(), other.treeToArray());
	// 	 Node* newRoot = createFullTree(std::ceil(std::log2(mergedArr.size)));
	// 	//insert the merged array into the tree

	// }
#ifndef NDEBUG //for testing purposes
	bool isCorrect() {
		std::vector<T> v = vectorizedTree();
		bool treeToArrIsCorrect = true;
		StupidArr<T> arr = treeToArray();
		for (int i = 0; i < n; i++) {
			if (v[i] != arr[i]) {
				treeToArrIsCorrect = false;
				break;
			}
		}
		delete[] arr.arr;
		return isBalanced() &&
				 std::is_sorted(v.begin(), v.end()) &&
				 	 v.size() == static_cast<unsigned int>(n) &&
					 	treeToArrIsCorrect;
	}

	 Node* getRoot() const {
		return root;
	}
#endif	//NDEBUG


private:
	//fields
	int n; //number of  Nodes
	Node* root;
	COMP comp;

	//recursive utility functions
	 Node* insertUtil( Node* head, T key);
	 Node* removeUtil( Node* head, T key);
	template <typename K>
	 Node* searchUtil( Node* head, K key) const;
	void destroy( Node* head);
	T* treeToArrayUtil( Node* head, T* result) const;	
	 Node* createFullTree(int height);
	struct treeRemovalUtil{
		 Node* head;
		int remove;
		treeRemovalUtil( Node* head, int remove) : head(head), remove(remove) {}
	};
	treeRemovalUtil removeNNodes(treeRemovalUtil tree);
	T* arrayToTreeUtil(T* arr,  Node* head);

	//general utility functions
	int height( Node* head) const {
		return (head == nullptr) ? -1 : head->height;
	}
	int balanceFactor( Node* head) const {
		return (head == nullptr) ? 0 : height(head->left) - height(head->right);
	}
	 Node* balanceTree( Node* head, int balanceFactor);

	// Roll Functions
	 Node* rollLeftLeft( Node*  Node);
	 Node* rollLeftRight( Node*  Node);
	 Node* rollRightLeft( Node*  Node);
	 Node* rollRightRight( Node*  Node);

	//--------------for testing purposes--------------//
#ifndef NDEBUG
	std::vector<T> vectorizedTree();
	void inOrderUtil_test(std::vector<T>& result,  Node* head);
	bool isBalanced();
	bool isBalancedUtil( Node* head);

#endif	//NDEBUG

};
//--------------recursive utility functions--------------//
 
typename AVLTree::Node* AVLTree::insertUtil(AVLTree:: Node* head, T key) {
	if (head == nullptr) {
		Node* newNode = new Node(key);
		n++; //alloc error wont change n
		return newNode;
	}
	if (comp(key, head->key) == LESS) {
		head->left = insertUtil(head->left, key);
	}
	else if (comp(key, head->key) == GREATER) {
		head->right = insertUtil(head->right, key);
	}
	else {
		throw KeyAlreadyExistsException();
	}
	head->height = 1 + max(height(head->left), height(head->right));
	return balanceTree(head, balanceFactor(head));
}


 
void AVLTree::destroy(AVLTree:: Node* head) {
	if (head == nullptr) return;
	destroy(head->left);
	destroy(head->right);
	delete head;
}

 
typename AVLTree:: Node* AVLTree::removeUtil(AVLTree:: Node* head, T key) {
	if (head == nullptr) {
		throw KeyDoesNotExistException();
	}
	if (comp(key, head->key) == LESS) {
		head->left = removeUtil(head->left, key);
	}
	if (comp(key, head->key) == GREATER) {
		head->right = removeUtil(head->right, key);
	}
	if(comp(key, head->key) == EQUAL){
		//key found
		if (head->left == nullptr || head->right == nullptr) {	// Node has one or no children
			Node* temp = (head->left != nullptr) ? head->left : head->right;
			if (temp == nullptr) { 		// Node has no children
				temp = head;
				head = nullptr;
			}
			else {	// Node has one child
				*head = *temp;
			}
			delete temp;
			n--;
		}
		else {  // Node has two children
			 Node* temp = head->right;
			while (temp->left != nullptr) {
				temp = temp->left;
			}
			head->key = temp->key;
			head->right = removeUtil(head->right, temp->key);
		}

	}

	if (head != nullptr) {
		head->height = 1 + max(height(head->left), height(head->right));
		head = balanceTree(head, balanceFactor(head));
	}
	return head;
}

 
template<typename K>
typename AVLTree:: Node* AVLTree::searchUtil(AVLTree:: Node* head, K key) const {
	if (head == nullptr) {
		throw KeyDoesNotExistException();
	}
	if (comp(key, head->key) == LESS) {
		return searchUtil(head->left, key);
	}
	else if (comp(key, head->key) == GREATER) {
		return searchUtil(head->right, key);
	}
	else {
		return head;
	}
}
 
T* AVLTree::treeToArrayUtil(AVLTree:: Node* head, T* result) const{
	if (head == nullptr) return result;
	result = treeToArrayUtil(head->left, result);
	*result = head->key;
	result++;
	result = treeToArrayUtil(head->right, result);
	return result;
}
 
typename AVLTree:: Node* AVLTree::createFullTree(int height) {
	if (height == -1) return nullptr;
	 Node* head = new  Node(0);
	head->left = createFullTree(height - 1);
	head->right = createFullTree(height - 1);
	head->height = height;
	return head;
}

 
T* AVLTree::arrayToTreeUtil(T* arr, AVLTree:: Node* head) {
	if (head == nullptr) return arr;
	arr = arrayToTreeUtil(arr, head->left);
	head->key = *arr;
	arr++;
	arr = arrayToTreeUtil(arr, head->right);
	return arr;
}
//--------------general utility functions--------------//
 
typename AVLTree:: Node* AVLTree::balanceTree(AVLTree:: Node* head, int bf) {
	if (bf > 1) {
		if (balanceFactor(head->left) > -1) {
			return rollLeftLeft(head);
		}
		else {
			return rollLeftRight(head);
		}
	}
	else if (bf < -1) {
		if (balanceFactor(head->right) > -1) {
			return rollRightLeft(head);
		}
		else {
			return rollRightRight(head);
		}
	}
	return head;
}



 
typename AVLTree::treeRemovalUtil AVLTree::removeNNodes(AVLTree::treeRemovalUtil head) {
	if (head.remove == 0) return head;
	if (head.head==nullptr) return head;
	treeRemovalUtil right = removeNNodes(treeRemovalUtil(head.head->right, head.remove));
	head.head->right = right.head;
	head.remove = right.remove;
	head.head->height = 1 + max(height(head.head->left), height(head.head->right));
	if(head.remove==0) {return head;}
	if(head.head->left==nullptr && head.head->right==nullptr){
		delete head.head;
		head.head=nullptr;
		head.remove--;
		return head;
	}
	treeRemovalUtil left = removeNNodes(treeRemovalUtil(head.head->left, head.remove));
	head.head->left = left.head;
	head.remove = left.remove;
	head.head->height = 1 + max(height(head.head->left), height(head.head->right));
	return head;		
}
//--------------Roll Functions--------------//
 
typename AVLTree:: Node* AVLTree::rollLeftLeft(AVLTree:: Node* head) {
	assert(head != nullptr && head->left != nullptr);
	 Node* newHead = head->left;
	head->left = newHead->right;
	newHead->right = head;
	head->height = 1 + max(height(head->left), height(head->right));
	newHead->height = 1 + max(height(newHead->left), height(newHead->right));
	return newHead;
}
 
typename AVLTree:: Node* AVLTree::rollRightRight(AVLTree:: Node* head) {
	assert(head != nullptr && head->right != nullptr);
	 Node* newHead = head->right;
	head->right = newHead->left;
	newHead->left = head;
	head->height = 1 + max(height(head->left), height(head->right));
	newHead->height = 1 + max(height(newHead->left), height(newHead->right));
	return newHead;
}

 
typename AVLTree:: Node* AVLTree::rollLeftRight(AVLTree:: Node* head) {
	assert(head != nullptr && head->left != nullptr);
	head->left = rollRightRight(head->left);
	return rollLeftLeft(head);
}

 
typename AVLTree:: Node* AVLTree::rollRightLeft(AVLTree:: Node* head) {
	assert(head != nullptr && head->right != nullptr);
	head->right = rollLeftLeft(head->right);
	return rollRightRight(head);
}

#ifndef NDEBUG
//--------------for testing purposes--------------//

 
std::vector<T> AVLTree::vectorizedTree() {
	std::vector<T> result;
	inOrderUtil_test(result, root);
	return result;
}

 
void AVLTree::inOrderUtil_test(std::vector<T>& result, typename AVLTree:: Node* head) {
	if (head == nullptr) return;
	inOrderUtil_test(result, head->left);
	result.push_back(head->key);
	inOrderUtil_test(result, head->right);
}

 
bool AVLTree::isBalanced() {
	return isBalancedUtil(root);
}

 
bool AVLTree::isBalancedUtil(typename AVLTree:: Node* head) {
	if (head == nullptr) return true;
	int bf = balanceFactor(head);
	if (bf > 1 || bf < -1) return false;
	return isBalancedUtil(head->left) && isBalancedUtil(head->right);
}
#endif  //NDEBUG
#endif // !AVLTree_H
