#include "TeamTree.h"
#include "Exceptions.h"
#include <cassert>
inline static int max(int a, int b) {
	return (a > b) ? a : b;
}
TeamTree::TeamTree() : root(nullptr), n(0) {}
TeamTree::~TeamTree() {
	deleteTree(root);
}
//public functions
void TeamTree::insert(Team* team) {
	assert(team != nullptr);
	root = insertUtil(root, team, 0);
}
void TeamTree::remove(Team* team) {
	root = removeUtil(root, team, 0);
}
Team* TeamTree::find(int teamId) {
	Node* node = findUtil(root, teamId);
	if (node == nullptr) return nullptr;
	return node->team;
}
bool TeamTree::search(int teamId) {
	return find(teamId) != nullptr;
}

bool TeamTree::isEmpty() const {
	return n == 0;
}
int TeamTree::size() const {
	return n;
}
int TeamTree::getMaxRank() const {
	if (root == nullptr) return 0;
	return root->maxRank;
}

int TeamTree::getTeamStrength(int teamId) const {
	return getTeamStrengthUtil(root, teamId, 0);
}

//recursive utility functions
TeamTree::Node* TeamTree::insertUtil(Node* head, Team* team, int removeMedals) {
	if (head == nullptr) {
		Node* newNode = new Node(team);
		team->addMedals(-removeMedals); //minus sign
		n++; //alloc error wont change n
		return newNode;
	}
	if (*team < *head->team) {
		head->left = insertUtil(head->left, team, removeMedals + head->addMedals);
	}
	else if (*team > *head->team) {
		head->right = insertUtil(head->right, team, removeMedals + head->addMedals);
	}
	else {
		throw KeyAlreadyExistsException();
	}
	head->height = 1 + max(height(head->left), height(head->right));
	head->NodesInSubtree = 1 + getNodesInSubtree(head->left)+ getNodesInSubtree(head->right);
	head->maxRank = max(head->team->getStrength(), max(getMaxRank(head->left), getMaxRank(head->right))); 	//TODO: fix this maxrank
	return balanceTree(head, balanceFactor(head), removeMedals + head->addMedals);
}

TeamTree::Node* TeamTree::removeUtil(TeamTree::Node* root, Team* team, int addMedals) {
	if (root == nullptr) {
		throw KeyDoesNotExistException();
	}
	if (*team < *root->team) {
		root->left = removeUtil(root->left, team, addMedals + root->addMedals);
	}
	else if (*team > *root->team) {
		root->right = removeUtil(root->right, team, addMedals + root->addMedals);
	}
	else {
		if (root->left == nullptr || root->right == nullptr) {	//one or no child
			Node* temp = (root->left == nullptr) ? root->right : root->left;
			if (temp == nullptr) {	//no children
				temp = root;
				root = nullptr;
			}
			else {
				temp->addMedals += getAddMedals(root);
				root = temp;
			}
		
		}
		else{} {	//two children

		}//TODO: fix this maxrank
		root->height = 1 + max(height(root->left), height(root->right));
		root->NodesInSubtree = 1 + getNodesInSubtree(root->left)+ getNodesInSubtree(root->right);
		root->maxRank = max(root->team->getStrength() + root->team->getMedals(), max(getMaxRank(root->left), getMaxRank(root->right)));
		return balanceTree(root, balanceFactor(root), addMedals);
	}
}
	//auxiliary functions
	int TeamTree::height(TeamTree::Node * root) const {
		return (root == nullptr) ? -1 : root->height;
	}

	int TeamTree::balanceFactor(TeamTree::Node * root) const {
		return (root == nullptr) ? 0 : height(root->left) - height(root->right);
	}

	TeamTree::Node* TeamTree::balanceTree(TeamTree::Node * root, int bf, int AddMedals) {
		if (bf > 1) {
			if (balanceFactor(root->left) > -1) {
				return rollLeftLeft(root, AddMedals);
			}
			else {
				return rollLeftRight(root, AddMedals);
			}
		}
		else if (bf < -1) {
			if (balanceFactor(root->right) > -1) {
				return rollRightLeft(root, AddMedals);
			}
			else {
				return rollRightRight(root, AddMedals);
			}
		}
		return root;
	}

	//Roll functions
	//in all roll functions, AddMedals is the medals that are added to root, including root->addMedals
	TeamTree::Node* TeamTree::rollLeftLeft(TeamTree::Node * root, int AddMedals) {
		int rootRank = root->team->getStrength() + root->team->getMedals() + AddMedals;
		int newRootRank = root->left->team->getStrength() + root->left->team->getMedals() + AddMedals + getAddMedals(root->left);
		Node* newRoot = root->left;
		root->left = newRoot->right;
		newRoot->right = root;
		root->height = 1 + max(height(root->left), height(root->right));
		newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
		root->NodesInSubtree = 1 + getNodesInSubtree(root->left) + getNodesInSubtree(root->right);
		newRoot->NodesInSubtree = 1 + getNodesInSubtree(newRoot->left) + getNodesInSubtree(newRoot->right);
		root->maxRank = max(rootRank, max(getMaxRank(root->left), getMaxRank(root->right)));	//TODO: fix this maxrank
		newRoot->maxRank = max(newRootRank, max(getMaxRank(newRoot->left), getMaxRank(newRoot->right)));
		int temp = newRoot->addMedals;
		newRoot->addMedals += temp;
		root->addMedals = -temp;
		root->left->addMedals += temp;
		return newRoot;
	}

	TeamTree::Node* TeamTree::rollRightRight(TeamTree::Node * root, int AddMedals) {
		int rootRank = root->team->getStrength() + root->team->getMedals() + AddMedals;
		int newRootRank = root->right->team->getStrength() + root->right->team->getMedals() + AddMedals + root->right->addMedals;
		Node* newRoot = root->right;
		root->right = newRoot->left;
		newRoot->left = root;
		root->height = 1 + max(height(root->left), height(root->right));
		newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
		root->NodesInSubtree = 1 + getNodesInSubtree(root->left) + getNodesInSubtree(root->right);
		newRoot->NodesInSubtree = 1 + getNodesInSubtree(newRoot->left) + getNodesInSubtree(newRoot->right);
		root->maxRank = max(rootRank, max(getMaxRank(root->left), getMaxRank(root->right)));
		newRoot->maxRank = max(newRootRank, max(getMaxRank(newRoot->left), getMaxRank(newRoot->right)));
		int temp = newRoot->addMedals;
		newRoot->addMedals += temp;
		root->addMedals = -temp;
		root->right->addMedals += temp;
		return newRoot;
	}
	TeamTree::Node* TeamTree::rollLeftRight(TeamTree::Node * root, int AddMedals) {
		root->left = rollRightRight(root->left, getAddMedals(root->left) + AddMedals);
		return rollLeftLeft(root, AddMedals);
	}
	TeamTree::Node* TeamTree::rollRightLeft(TeamTree::Node * root, int AddMedals) {
		root->right = rollLeftLeft(root->right, getAddMedals(root->right)+ AddMedals);
		return rollRightRight(root, AddMedals);
	}