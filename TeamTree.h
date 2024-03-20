#ifndef TEAMTREE_H_
#define TEAMTREE_H_

#include "Team.h"
class TeamTree{
public:
	TeamTree();
	~TeamTree();
	void insert(Team* team);
	void remove(Team* team);
	Team* find(int teamId);
	bool search(int teamId);	
	bool isEmpty() const;
	int size() const;
	int getMaxRank() const;
	int getTeamStrength(int teamId) const;
private:
	struct Node{
		Team* team;
		Node* left;
		Node* right;
		int NodesInSubtree;
		int maxRank;
		int addMedals;
		int height;
		Node(Team* team): team(team), left(nullptr), right(nullptr),
		 NodesInSubtree(1), maxRank(team->getStrength()+team->getMedals()), addMedals(0), height(0){};
	};
	Node* root;
	int n;

	//recursive utility functions	
	Node* insertUtil(Node* root, Team* team, int removeMedals);
	Node* removeUtil(Node* root, Team* team);
	Node* findUtil(Node* root, int teamId);
	void deleteTree(Node* root);
	int getTeamStrengthUtil(Node* root, int teamId, int addMedals) const;

	//auxiliary functions
	int height(Node* root) const;
	int balanceFactor(Node* root) const;
	Node* balanceTree(Node* root, int bf, int addMedals);

	//Roll functions
	Node* rollLeftLeft(Node* Node, int addMedals);
	Node* rollLeftRight(Node* Node, int addMedals);
	Node* rollRightLeft(Node* Node, int addMedals);
	Node* rollRightRight(Node* Node, int addMedals);

};
#endif // TEAMTREE_H_