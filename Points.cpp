#include "Points.h"

const int tiers = 20;

static std::unordered_map<std::string, sf::Vector2f> elemToPos{ {"",sf::Vector2f(0,0)} };

originPoint::originPoint(float baseLength) :	
	sf::CircleShape(5),
	baseLength{ baseLength },
	maxTiers{ tiers },
	groupElement{}	
{

	//set origin of circle to be center
	setOrigin(sf::Vector2f(5, 5));

	//set posiiton of center to center of screen
	setPosition(sf::Vector2f(baseLength, baseLength));

	//set fill color
	setFillColor(sf::Color::Blue);

	//keep track of position on screen with members
	nowPos = getPosition();
	thenPos = getPosition();
	
	// add position of identity into saved positions;
	savePosition();
}

originPoint::originPoint(std::string word, float baseLength) :
	sf::CircleShape(5),
	baseLength{ baseLength },
	maxTiers{ tiers },
	groupElement{}

{
	setOrigin(sf::Vector2f(5, 5));

	

	setFillColor(sf::Color::Blue);

	int i = 1;

	while (elemToPos.count(word.substr(word.size() - i-1, i))) {
		i++;
	}
	i--;
	if (i == 0) {
		setPosition(sf::Vector2f(baseLength, baseLength));
	}
	else {
		setPosition(elemToPos[word.substr(word.size() - 1 - i, i)]);
	}

	

	for (i = word.size()-1-i; i >= 0; i--) {
		move(word[i]);
	}

	
}





///////
// Generally what happens for movex() methods is the word is updated,
// then if backtracking occurs, or the word is found in the dict, we 
// load the position from the dict.
// 
// Otherwise, we call halfPositionPlus to compute the offset from the
// origin, plus the correct translation, which is always a rotation of
// (baselength/2,0). Then save the new position with the new word to
// the dict.
//////

void originPoint::movea(){
	if (hasCancellation('a')) {
		groupElement.pop_back();
		updateFromSavedPos();
		nowPos = getPosition();
		return;
	}
	groupElement.push_back('a');

	if (updateFromSavedPos()) {
		nowPos = getPosition();
		return;
	}

	setPosition(halfPositionPlus(sf::Vector2f(baseLength / 2, 0)));
	savePosition();
	nowPos = getPosition();
}
void originPoint::moveb(){
	if (hasCancellation('b')) {
		groupElement.pop_back();
		updateFromSavedPos();
		nowPos = getPosition();
		return;
	}
	groupElement.push_back('b');
	if (updateFromSavedPos()) {
		nowPos = getPosition();
		return;
	}
	setPosition(halfPositionPlus(sf::Vector2f(0, -baseLength / 2)));
	savePosition();
	nowPos = getPosition();
		
}
void originPoint::moveA(){
	if (hasCancellation('A')) {
		groupElement.pop_back();
		updateFromSavedPos();
		nowPos = getPosition();
		return;
	}
	groupElement.push_back('A');
	if (updateFromSavedPos()) {
		nowPos = getPosition();
		return;
	}
	setPosition(halfPositionPlus(sf::Vector2f(-baseLength / 2, 0)));
	savePosition();
	nowPos = getPosition();
}
void originPoint::moveB() {
	if (hasCancellation('B')) {
		groupElement.pop_back();
		updateFromSavedPos();
		nowPos = getPosition();
		return;
	}
	groupElement.push_back('B');
	if (updateFromSavedPos()) {
		nowPos = getPosition();
		return;
	}
	setPosition(halfPositionPlus(sf::Vector2f(0,baseLength / 2)));
	savePosition();
	nowPos = getPosition();
}

void originPoint::move(char c) {
	switch (c) {
	case 'a':
		movea();
		break;
	case 'b':
		moveb();
		break;
	case 'A':
		moveA();
		break;
	case 'B':
		moveB();
		break;
	default:
		break;
	}
}

bool originPoint::updateFromSavedPos() {
	std::string elem = groupElement;
	if (elem.size() == 0) {
		elem = "e";
	}
	if (elemToPos.count(elem)) {
		setPosition(elemToPos[elem]);
		nowPos = getPosition();
		return true;
	}
	return false;
}









sf::Vector2f originPoint::halfPositionPlus(sf::Vector2f posChange) {
	return (((getPosition() - sf::Vector2f(baseLength, baseLength)) * .5f) 
			+ posChange) + sf::Vector2f(baseLength, baseLength);
}



bool originPoint::hasCancellation(char c) {
	if (groupElement.empty()) {
		return false;
	}
	switch (c) {
	case 'a':
		return groupElement.back() == 'A';
	case 'b':
		return groupElement.back() == 'B';
	case 'A':
		return groupElement.back() == 'a';
	case 'B':
		return groupElement.back() == 'b';
	default:
		return false;
	}
}


std::string originPoint::getGroupElement() {
	if (groupElement.empty()) {
		return "e";
	}
	
	return groupElement;
}



void originPoint::printGroupElement() {
	std::cout << getGroupElement() << '\n';
}





sf::Vector2f originPoint::nextPos(char c) {
	if (hasCancellation(c)) {
		char temp = groupElement.back();
		groupElement.pop_back();
		sf::Vector2f res = elemToPos[getGroupElement()];
		groupElement.push_back(temp);
		return res;
	}
	switch (c) {
	case 'a':
		return halfPositionPlus(sf::Vector2f(baseLength/2,0));
	case 'b':
		return halfPositionPlus(sf::Vector2f(0,-baseLength/2));
	case 'A':
		return halfPositionPlus(sf::Vector2f(-baseLength/2,0));
	case 'B':
		return halfPositionPlus(sf::Vector2f(0,baseLength/2));
	default:
		return sf::Vector2f(0, 0);
	}

}







void originPoint::savePosition() {
	elemToPos[getGroupElement()] = getPosition();
}













