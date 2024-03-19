#ifndef TEAM_H_
#define TEAM_H_
class Team {
private:
	int teamId;
	int medals;
	int medianStrength;
	//player stack
	//player median heap

public:
	Team(int teamId);
	int getTeamId() const;
	int getStrength() const;
	bool operator<(const Team& other) const; //in the sense of strength, and inverse 
	bool operator>(const Team& other) const;
	bool operator==(const Team& other) const;
	void addPlayer(int playerStrength);
	bool removeLastPlayer();
	void addMedals(int medals);
};
#endif // TEAM_H_