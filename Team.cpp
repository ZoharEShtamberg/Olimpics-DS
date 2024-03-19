#include "Team.h"
Team::Team(int teamId): teamId(teamId), medianStrength(0), medals(0){
}
int Team::getTeamId() const{
	return teamId;
}
int Team::getStrength() const{
	return medianStrength;
}
bool Team::operator<(const Team& other) const{
	if(getStrength() == other.getStrength()){
		return getTeamId() > other.getTeamId();
	}
	return getStrength() < other.getStrength();
}

bool Team::operator>(const Team& other) const{
	return other < *this;
}
bool Team::operator==(const Team& other) const{
	return Team::getTeamId() == other.getTeamId();
}
void Team::addPlayer(int playerStrength){
	//TODO: implement
	//add player to stack
	//add player to median heap
	//update median
}
bool Team::removeLastPlayer(){
	//TODO: implement
	//remove player from stack
	//remove player from median heap
	//update median
	//return true if team is empty after removal
}