#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"


#include "TreeAnimation.h"
#include "GroupStuff.h"
#include "TreesCommandLine.h"

int main() {
	animatedTreeAndPoints aTree;
	
	while (aTree.isOpen())
	{
		sf::Event event;
		while (aTree.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				aTree.close();
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.scancode) {
				case (86):
					aTree.Move('a');
					break;

				case (87):
					aTree.Move('A');
					break;

				case (88):
					aTree.Move('B');
					break;
				case (89):
					aTree.Move('b');
					break;
				default:
					break;
				}
			}

		}

		aTree.clear(sf::Color::White);
		aTree.fullDraw();
		aTree.display();
	}
	return 0;
}

