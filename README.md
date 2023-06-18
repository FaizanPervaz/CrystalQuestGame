# CrystalQuestGame

This repository contains a C++ implementation of a text-based game called "The Quest for the Crystal Kingdom." The game is set in a dangerous and enchanted forest filled with enemies, obstacles, and rewards. The player's objective is to retrieve a legendary crystal by navigating through the forest, overcoming challenges, and collecting items.

The game incorporates various algorithms and data structures, including AVL trees, Floyd's algorithm, Prim's algorithm, Kruskal's algorithm, and Dijkstra's algorithm. These algorithms are utilized to calculate the shortest paths between areas, find the minimum spanning tree of the forest, and determine the shortest path from the player's current location to the crystal.

The game map is represented using characters, with each character representing a specific entity. Safe paths allow the player to move freely, while obstacles require finding alternate routes due to their high weight. Rewards such as jewels, weapons, and potions can be collected to increase the player's score, while encounters with enemies may result in the loss of certain items and decrementing of the score. Death points lead to game over and require restarting from the beginning.

This repository provides the following functionalities:

Calculation of the shortest path between any two areas in the forest using Floyd's algorithm.
Calculation of the shortest path from a custom location (coordinates entered by the user) using Dijkstra's algorithm.
Finding the minimum spanning tree of the forest using Prim's algorithm and Kruskal's algorithm.
Tracking the score, including increments and decrements based on collected rewards and encounters with enemies.
A user-friendly menu with options to access the above functionalities.
The implementation adheres to good programming practices, includes detailed documentation, and is written entirely in C++. The algorithms are implemented from scratch, without relying on any pre-built libraries or APIs.

Feel free to explore, contribute, and enjoy the exciting quest for the Crystal Kingdom in this interactive game!

Note: Make sure to replace "ReprositryName" in the request with a suitable repository name of your choice.
