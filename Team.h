#ifndef TEAM_H_
#define TEAM_H_
class Team {
private:
	int teamId;
	int medianStrength;
	//player stack
	//player median heap
	
public:
	Team(int teamId);
	int getTeamId() const;
	int getStrength() const;
		
};
#endif // TEAM_H_