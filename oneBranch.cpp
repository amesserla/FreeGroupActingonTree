#include "TreeAnimation.h"

void oneBranch::draw(sf::RenderTarget& window, sf::RenderStates states) const {
	for (line aLine : branchEdges) {
		window.draw(aLine);
	}
	window.draw(mainLine);
}


oneBranch::oneBranch(int iters, float dist, sf::RenderTarget& window) : mainLine{ line(dist - epsilon) } {
	if (dist - epsilon > 0) {
		mainLine.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	}
	if (iters == 0) {
		return;
	}
	else {
		oneBranch prev(iters - 1, dist / 2, window);
		prev.recurse(prev);

		*this = prev; //might be better way to do this.
	}



}

void oneBranch::scale(float factor) {
	for (line& aLine : branchEdges) {
		aLine.setScale(factor);
	}
	mainLine.setScale(factor);
}

void oneBranch::setPosition(sf::Vector2f newPosition) {
	for (line& aLine : branchEdges) {
		aLine.setPosition(newPosition);
	}
	mainLine.setPosition(newPosition);
}

void oneBranch::flip() {
	for (line& aLine : branchEdges) {
		aLine.rotateClockwise();
		aLine.rotateClockwise();
	}
	mainLine.rotateClockwise();
	mainLine.rotateClockwise();

}

sf::Vector2f oneBranch::getPosition() {
	return mainLine.getPosition();
}

// method to add another tier to existing branch. doubles size on each iteration.
// use prevBranch input for constructing tree. might be better way of doing this.

void oneBranch::recurse(oneBranch prevBranch) {

	// constants needed for computations

	const float dist = 2 * (prevBranch.mainLine.length + epsilon);
	const float midScreenHeight = prevBranch.mainLine.getPosition().y;
	const float midScreenWidth = prevBranch.mainLine.getPosition().x;

	// Clear existing branch edges

	branchEdges.clear();

	// Add new branch edges on left and right from mainLine of previous iteration

	line mainLeft = prevBranch.mainLine;
	line mainRight = prevBranch.mainLine;
	mainLeft.rotateCounterClockwise();
	mainLeft.shiftOriginDown(dist / 2);
	mainLeft.setPosition(sf::Vector2f(midScreenWidth, midScreenHeight));
	mainRight.rotateClockwise();
	mainRight.shiftOriginDown(dist / 2);
	mainRight.setPosition(sf::Vector2f(midScreenWidth, midScreenHeight));
	branchEdges.push_back(mainLeft);
	branchEdges.push_back(mainRight);

	// Set new mainLine

	mainLine = line(dist - epsilon);
	mainLine.setPosition(sf::Vector2f(midScreenWidth, midScreenHeight));
	mainLine.setFillColor(sf::Color::Black);

	// Shift previous iteration up to facilitate proper rotations of branch edges, probably not needed since manually updating branch edge origin and positions in for loop.

	prevBranch.setPosition(sf::Vector2f(midScreenWidth, midScreenHeight + dist));

	// Copy branch edges from previous iterations to new branch edges, updating position and origin. Then rotate to left and right, update origin and position, and add to new branch edges

	for (line aLine : prevBranch.branchEdges) {
		line leftLine = aLine;
		line rightLine = aLine;
		aLine.shiftOriginDown(dist / 2);
		aLine.setPosition(midScreenWidth, midScreenHeight);
		branchEdges.push_back(aLine);


		leftLine.rotateCounterClockwise();
		leftLine.shiftOriginDown(dist / 2);
		leftLine.setPosition(midScreenWidth, midScreenHeight);
		branchEdges.push_back(leftLine);


		rightLine.rotateClockwise();
		rightLine.shiftOriginDown(dist / 2);
		rightLine.setPosition(midScreenWidth, midScreenHeight);
		branchEdges.push_back(rightLine);
	}
}