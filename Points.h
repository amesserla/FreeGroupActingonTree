//////// 
// Points are dots (cicles) that represent group elements. They are moved aroudn by the group action.
// The word origin refers to the fact that originally the plan was to only track the 
// identity, which starts at the "origin"
// 
// All points are assumed to start at the identity (origin) and be moved using
// appropriate move methods. Manually changing location outside (i.e. using setPosition
// from SFML) will create a disconnect between groupElement and the position, causing problems.
// 
// In particular, this is used to avoid computing canellation movements (i.e. aA has already been computed from 'e')
///////




#ifndef POINTS
#define POINTS

#include <stack>
#include <unordered_map>
#include <string>
#include <iostream>

#include "SFML/Graphics.hpp"



class originPoint : public sf::CircleShape {
public:
	//////
	// Constructors
	//////

	originPoint(float baseLenght);

	originPoint(std::string word, float baseLength);	//allow generating a point that starts at the specified word

	//////
	// Movement methods
	//////
	
	//move point "right"
	void movea();

	//move point "up"
	void moveb();

	//move point "left"
	void moveA();

	//move point "down"
	void moveB();

	//move in direction of c
	void move(char c);	

	//used to avoid computations on already visited points
	//returns false if not already visited
	bool updateFromSavedPos();
	


	///////
	// Position calculations methods
	// Movement methods call this to find final place of point after movement
	//////

	sf::Vector2f halfPositionPlus(sf::Vector2f posChange);



	///////
	// Methods to look at the name of the group element of a point
	// 
	// if hasCancellation returns true, we can use a saved location, since we are undoing an already done action 
	// 
	// getGroupElement is used to change "" to "e" for dict lookup,
	// as well as being called in printGroupElement
	// 
	// printGroupElement might be used for an onClick method or something later
	///////
	bool hasCancellation(char c);

	std::string getGroupElement();
	
	void printGroupElement();

	//////
	// Method used to calculate the new position after given the specified action
	// Used to match the points to the tree movements
	// 
	//////

	sf::Vector2f nextPos(char c);

	//////
	// Used to update memo on group element positions
	/////

	void savePosition();





	//////
	// Members
	//////
private:

	const float baseLength;			// initialized to half of the trees width
	const int maxTiers;				// should be moved out of here. 
									//related to resolution of the tree

	std::string groupElement;

	
public:
	//////
	//Together, these are used in the animation process, 
	//thenPos is calculated at time of animation, so it does not need to 
	//be kept up to date, but nowPos should be updated each time the point moves
	// 
	// In the future, code can likely be rewritten to remove use of these and use the elemToPos dict instead.
	//////
	sf::Vector2f thenPos;			//used to predict where the point goes in animatedTreeAndPoints::Move()
	sf::Vector2f nowPos;			//used to save where the point started from in animatedTreeAndPoints::Move()


};






#endif //POINTS
