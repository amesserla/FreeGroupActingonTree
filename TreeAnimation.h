#pragma once

#include <iostream>
#include <stack>
#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "Points.h"


const float epsilon = 12;
const int tiers = 20;
const float transitionTime = 1;

const unsigned int screenSize = 1000;
const std::string title = "Tree";

const std::vector<sf::Color> colorWheel = {	sf::Color::Green, 
											sf::Color::Blue, 
											sf::Color::Red, 
											sf::Color::Magenta, 
											sf::Color::Yellow, 
											sf::Color::Cyan};




///////
// The line structure is used because when using rectangleShape it was hard to track which direction needed scaling.
// These lines have a constant width of 1px even after scaling.
///////



struct line : public sf::ConvexShape {
	friend class oneBranch;
	friend class animatedTreeAndPoints;
	//Scale line by the length only, maintaining thickness of 1 pixel, and position relative to the origin.

	void setScale(float newScale) {
		//save old data 
		sf::Vector2f oldOrigin = getOrigin();
		sf::Vector2f oldPosition = getPosition();
		float newLength{ newScale * length };
		setOrigin(sf::Vector2f((oldOrigin.x-.5)*newScale+.5,(oldOrigin.y-(length+.5))*newScale+(newLength+.5)));
		
		// update each corner of the rectangle
		setPoint(0, sf::Vector2f(0, 0));
		setPoint(1, sf::Vector2f(0, newLength));
		setPoint(2, sf::Vector2f(1, newLength));
		setPoint(3, sf::Vector2f(1, 1));
		
		// needed for sfml to update the geometry
		update();

		setPosition(oldPosition);

		length = newLength;


	}



	// construct a line of the given length, origin is by default at bottom center
	// bottom center was chosen so it could be positioned along the main line pointing up easily

	line(float length, bool isHorizantal = false) : sf::ConvexShape::ConvexShape(4), length{ length }, isHorizantal{ isHorizantal }, rotationCount{0} {
		//make 0 long, 0 wide line if it is too short
		if (length < 0) {
			setPoint(0, sf::Vector2f(0, 0));
			setPoint(1, sf::Vector2f(0, 0));
			setPoint(2, sf::Vector2f(0,0));
			setPoint(3, sf::Vector2f(0, 0));
			setOrigin(.5, .5);
			setFillColor(sf::Color::Black);
		}


		setPoint(0, sf::Vector2f(0,0));
		setPoint(1,sf::Vector2f(0,length));
		setPoint(2,sf::Vector2f(1,length));
		setPoint(3,sf::Vector2f(1,0));
		setOrigin(.5, length + .5);
		setFillColor(sf::Color::Black);
		if (isHorizantal) { 
			setRotation(90);
			rotationCount = 1;
		}
		update();

	}

	//////
	//rotation methods to keep track of orientation of the line 
	//////

	void rotateClockwise() {
		isHorizantal = !isHorizantal;
		rotate(90);
		rotationCount = rotationCount + 1;
		rotationCount = rotationCount % 4;
	}

	void rotateCounterClockwise() {
		isHorizantal = !isHorizantal;
		rotate(-90);
		rotationCount = rotationCount - 1;
		rotationCount = (rotationCount + 4) % 4;
	}

	//////
	// The origin functions as the base point of the line.
	// Drops origin in the positive y direction (down) in global coordinates. The local coordinate change is determined by the 
	// rotationCount member variable. 
	// Used to move the origin of the line to match up with the origin of the new main line for recursively defining
	// branches.
	//////

	void shiftOriginDown(float dist) {
		switch (rotationCount) {
		case 1:
			setOrigin(getOrigin().x + dist, getOrigin().y);
			break;

		case 2:
			setOrigin(getOrigin().x, getOrigin().y - dist);
			break;

		case 3:
			setOrigin(getOrigin().x - dist, getOrigin().y);
			break;

		case 0:
			setOrigin(getOrigin().x, getOrigin().y + dist);
			break;

		default:
			setFillColor(sf::Color::Magenta);
		}

	}


private:
	float length;
	bool isHorizantal;
	int rotationCount;


};



class oneBranch : sf::Drawable {
	friend class tree;
	friend class animatedTreeAndPoints;

public:	
	
	void draw(sf::RenderTarget& window, sf::RenderStates = sf::RenderStates::Default) const;

	
	
	oneBranch(int iters, float dist, sf::RenderTarget& window);


	void scale(float factor);


	void setPosition(sf::Vector2f newPosition);



	void flip();


	sf::Vector2f getPosition();


	// method to add another tier to existing branch. doubles size on each iteration.
	// use prevBranch input for constructing tree. might be better way of doing this.
	void recurse(oneBranch prevBranch);



// Public Members
private:
	line mainLine;					// Origin of mainLine is the origin of all branch edges, so branch can be moved all at once
	std::vector<line> branchEdges;

};

class tree : sf::Drawable {
	void draw(sf::RenderTarget& window, sf::RenderStates = sf::RenderStates::Default) const {
		window.draw(mainLine);
		for (oneBranch branch : tierUpBranches) {
			window.draw(branch);
		}
		for (oneBranch branch : tierDownBranches) {
			window.draw(branch);
		}
	}


	tree(sf::RenderTarget& window) : mainLine(window.getSize().x - 2 * epsilon, true) {
		float xLength = window.getSize().x;
		float yLength = window.getSize().y;

		float minDist = yLength / 2;
		maxTiers = 0;
		while (minDist - epsilon > 0 && maxTiers < tiers) {
			minDist = minDist / 2;
			maxTiers += 1;
		}

		std::cout << "Max tiers computed to be " << maxTiers << '\n';

		minDist = 2 * minDist;

		mainLine.setPosition(sf::Vector2f(epsilon, yLength / 2));
		mainLine.setFillColor(sf::Color::Black);

		oneBranch aBranch(0, minDist, window);
		aBranch.setPosition(sf::Vector2f(minDist, yLength / 2));
		tierUpBranches.push_back(aBranch);
		aBranch.flip();
		tierDownBranches.push_back(aBranch);
		aBranch.flip();

		for (int i = 1; i < maxTiers; i++) {
			minDist = minDist * 2;
			aBranch.recurse(aBranch);
			aBranch.setPosition(sf::Vector2f(minDist, yLength / 2));
			tierUpBranches.push_back(aBranch);
			aBranch.flip();
			tierDownBranches.push_back(aBranch);
			aBranch.flip();


		}

		minDist = xLength / 4;

		for (int i = 2; i <= maxTiers; i++) {
			oneBranch aBranch = tierUpBranches[maxTiers - i];
			aBranch.setPosition(sf::Vector2f(xLength - minDist, yLength / 2));
			tierUpBranches.push_back(aBranch);
			minDist = minDist / 2;
			aBranch.flip();
			tierDownBranches.push_back(aBranch);
		}



	}

	//Animates the action of 'a' on the tree.

	void rightMove(sf::RenderWindow& window) {
		sf::Clock timer;
		while (timer.getElapsedTime().asSeconds() < transitionTime) {
			window.clear(sf::Color::White);
			window.draw(mainLine);
			float factor = (timer.getElapsedTime().asSeconds()) / (transitionTime);
			for (int i = 0; i < maxTiers - 1; i++) {
				oneBranch copyBranch = tierUpBranches[i];
				copyBranch.recurse(copyBranch);
				copyBranch.scale(.5);
				sf::Vector2f newPos = tierUpBranches[i].getPosition() * (1-factor) + tierUpBranches[i + 1].getPosition() * factor;
				copyBranch.scale(factor + 1);
				copyBranch.setPosition(newPos);
				window.draw(copyBranch);
				copyBranch.flip();
				window.draw(copyBranch);

			}
			for (int i = maxTiers - 1; i < 2 * maxTiers - 2; i++) {
				oneBranch copyBranch = tierUpBranches[i];
				sf::Vector2f newPos = tierUpBranches[i].getPosition() * (1 - factor) + tierUpBranches[i + 1].getPosition() * factor;
				copyBranch.scale(1 - .5 * (factor));
				copyBranch.setPosition(newPos);
				window.draw(copyBranch);
				copyBranch.flip();
				window.draw(copyBranch);
			}
			window.display();
		}
	}

friend class animatedTreeAndPoints;

// Private Members
private:
	int maxTiers;
	line mainLine;
	std::vector<oneBranch> tierUpBranches;
	std::vector<oneBranch> tierDownBranches;

};

class animatedTreeAndPoints : public sf::RenderWindow {
public:
	animatedTreeAndPoints() : 
			sf::RenderWindow(sf::VideoMode(screenSize, screenSize),title), 
			theTree{*this}, 
			thePoints{{originPoint(getSize().x / 2)}} 
	{
		up = sf::View(sf::FloatRect(0,0,screenSize,screenSize));
		up.rotate(90);
		down = sf::View(sf::FloatRect(0,0,screenSize,screenSize));
		down.rotate(-90);
		left = sf::View(sf::FloatRect(0,0,screenSize,screenSize));
		left.rotate(180);
	}

	void addPoint(originPoint point) 
	{
		thePoints.push_back(point);
	}

	//////
	// Drawing the tree animations makes use of the sfml views.
	// 
	// Each frame is cleared, and rotated, each branch of the tree is drawn (with rotations to make the bottom branches),
	// and then the view is rotated back, before the points are drawn. This allows the points to use nowPos and thenPos in the
	// normal coordinates, but the logic for the tree is only drawn moving to the right.
	//////


	void Move(char c) 
	{
		//setup thenPos for linear shifting of the point.
		for (auto& point : thePoints) 
		{
			point.thenPos = point.nextPos(c);
		}
		sf::Clock timer;				//timer to make the animation look right
		sf::View useView;				//view for frame rotation
		switch (c) {
		case 'a':
			break;
		case 'b':
			useView = up;
			break;
		case 'A':
			useView = left;
			break;
		case 'B':
			useView = down;
			break;
		}

		//some constants to make the left most line appear out of nowhere
		const float endLeftLength = theTree.tierUpBranches[0].mainLine.length;
		const float endLeftPosition = theTree.tierUpBranches[0].mainLine.getPosition().x;
		const float midLineHeight = getSize().y / 2;


		while (timer.getElapsedTime().asSeconds() < transitionTime) {
			setView(useView);
			clear(sf::Color::White);
			draw(theTree.mainLine);
			
			float factor = (timer.getElapsedTime().asSeconds()) / (transitionTime);
			
			//draw new leftmost line if it should be long enough
			if (factor * endLeftPosition > epsilon) {
				line leftLine(endLeftLength * factor);
				leftLine.setPosition(sf::Vector2f(factor * endLeftPosition, midLineHeight));
				draw(leftLine);
				leftLine.rotate(180);
				draw(leftLine);
				leftLine.rotate(180);
			}

			//draw leftmost branches scaling up
			for (int i = 0; i < theTree.maxTiers - 1; i++) {
				oneBranch copyBranch = theTree.tierUpBranches[i+1];
				copyBranch.scale(.5);
				sf::Vector2f newPos = theTree.tierUpBranches[i].getPosition() * (1 - factor) + theTree.tierUpBranches[i + 1].getPosition() * factor;
				copyBranch.scale(factor + 1);
				copyBranch.setPosition(newPos);
				draw(copyBranch);
				copyBranch.flip();
				draw(copyBranch);

			}
			
			//draw right half of branches shrinking down
			for (int i = theTree.maxTiers - 1; i < 2 * theTree.maxTiers - 2; i++) {
				oneBranch copyBranch = theTree.tierUpBranches[i];
				sf::Vector2f newPos = theTree.tierUpBranches[i].getPosition() * (1 - factor) + theTree.tierUpBranches[i + 1].getPosition() * factor;
				copyBranch.scale(1 - .5 * (factor));
				copyBranch.setPosition(newPos);
				draw(copyBranch);
				copyBranch.flip();
				draw(copyBranch);
			}

			//turn view back to use default coordinates on points
			setView(getDefaultView());

			//draw the points
			for (auto& point : thePoints) {
				point.setPosition(point.nowPos*(1-factor)+point.thenPos*factor);
				draw(point);
			}

			//display everything
			display();
		}

		//the branches didn't actually move, but the points need to
		for (auto& point : thePoints) {
			point.setPosition(point.nowPos);
			point.move(c);
		}
	}



	void fullDraw() {
		draw(theTree);
		for (auto& point : thePoints) {
			draw(point);
		}
	}


private:
	sf::View up;
	sf::View down;
	sf::View left;
	std::vector<originPoint> thePoints;

public:
	tree theTree;

};


