#include "TeamTree.h"
TeamTree::TeamTree() : root(nullptr), n(0) {}
TeamTree::~TeamTree(){
	deleteTree(root);
}
void TeamTree::insert(Team* team){ 
	root = insertUtil(root, team);
}
void TeamTree::remove(Team* team){
	root = removeUtil(root, team);
}
Team* TeamTree::find(int teamId){
	Node* node = findUtil(root, teamId);
	if(node == nullptr) return nullptr;
	return node->team;
}
bool TeamTree::search(int teamId){
	return find(teamId) != nullptr;
}

bool TeamTree::isEmpty() const{
	return n == 0;
}
int TeamTree::size() const{
	return n;
}
int TeamTree::getMaxRank() const{
	if(root == nullptr) return 0;
	return root->maxRank;
}

int TeamTree::getTeamStrength(int teamId) const{
	return getTeamStrengthUtil(root, teamId, 0);
}

void TeamTree::insertUtil(Node* head, Team* team){
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
	