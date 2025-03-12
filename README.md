# The Action of a Free Group on a Tree

If you animate the [action](https://en.wikipedia.org/wiki/Group_action) of the integers on a line, it looks like the line sliding back and forth. Similarly, if you animate the action of a free abelian group with two generators on a plane, it looks like the worst 2D scroller game you could imagine (I also made this animation, somewhere, but it is less fun). Using the same idea of animating the group action by generators on a [Cayley graph](https://en.wikipedia.org/wiki/Cayley_graph) relative to those generators for a group, I wanted to animate the standard action of a free group on two generators acting on a regular tree of degree 4.

I used the [SFML](https://www.sfml-dev.org/) package (version 2.6.1) to draw the tree and animate the action of the two free generators and their inverses.

The code as is draws a tree in a 1000x1000px window, with a blue dot that follows the origin. Arrow keys left, right, up, and down move the tree. Inverses of each generator are in the opposite direction (The action is on the left, so if you want to see the result of acting on the tree by "ab", then you will need to move by "b" first, and then by "a". You can choose if "b" is up, down, right, or left, but in the code "b" is up and "a" is right).

Tools that already exist include a line class that is just a rectangle that is always one pixel wide, can be rotated, scaled, and has funtions related to building the branch class. The branch class, is a single branch of the tree that can be built recursively to draw a quarter of the tree, and various spacing and movement calculations. There exist variables that can be modified to adjust spacing, and maximum iterations of the tree to be drawn for performance reasons (exponential growth is fast).

I started working on other tools that might be useful, such as things to validate that with strings are on the alphabet "[abAB]", finding the reduction and inverse of words in that alphabet, and a class of points (like the point that follows the origin). Eventually, I want to use these tools to draw [line patterns](https://arxiv.org/pdf/1006.2123).
