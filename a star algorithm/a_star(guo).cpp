// MTE 140 - Assignment 4
// Name: Matthew Robert Gulbronson, Student ID: 20656205
// Name: Peiliang Guo, Student ID: 20665804
// Name: Kapilan Satkunanathan, Student ID: 20694418

#include <iostream>
#include <math.h>
#include <algorithm> 
#include <string>
#include <sstream>

#include "a_star.hpp"

using namespace std;

// Calculates and returns the fcost
int AStar::getFCost(Node* currentNode) {
	// TODO Step1. Return fCost as a sum of gCost and hCost
	return currentNode->gCost + currentNode->hCost;
}

// Computes the Euclidean distance between two nodes and scales it by 10 to avoid floats
int AStar::getHDistance(Node* A, Node* B) {
	// TODO Step2. Return the Euclidian distance scaled by 10
    
    // Calculate the delta x and delta y
    int dx = B->posX - A->posX;
    int dy = B->posY - A->posY;
    
    // Calculate the Euclidian distance * 10
    return sqrt(dx*dx + dy*dy) * 10;
}

// Iterates through the node's parents and creates a list of the shortest path to take
list<Node*> AStar::retracePath (Node* startNode, Node* endNode) {
	list<Node*> path;	
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		path.push_front(currentNode);
		currentNode = currentNode->parent;
	}
	return path;
}

// For the current node, cnode, discovers all walkable neighbours, and adds them to the neighbours list
list<Node*> AStar::getNeighbours(Node* cnode, Node* grid[Y_LENGTH][X_LENGTH]) {
	list<Node*> neighbours;	
	
	// TODO Step3. Add walkable neighbours to the neighbours list
	// Step3.1 Iterate from (cnode.y - 1) to (cnode.y + 1)
	// Step3.2 Iterate from (cnode.x - 1) to (cnode.x + 1)	
	// Step3.3 Add neighbours that are not outside the world bounds and that are walkable
    
    // Determine the valid ranges of x and y within the boundaries of the grid
    int xRange[2] = {
        max(cnode->posX - 1, 0),
        min(cnode->posX + 1, X_LENGTH - 1)
    };
    
    int yRange[2] = {
        max(cnode->posY - 1, 0),
        min(cnode->posY + 1, Y_LENGTH - 1)
    };
    
    // Iterate through all points in the x and y ranges
    for (int y = yRange[0]; y <= yRange[1]; y++) {
        for (int x = xRange[0]; x <= xRange[1]; x++) {
            // Ignore the cnode itself
            if (x != cnode->posX || y != cnode->posY) {
                // Check if the neighbour is walkable
                Node* neighbour = grid[y][x];
                if (neighbour->walkable) {
                    // Push neighbour to the list
                    neighbours.push_back(neighbour);
                }
            }
        }
    }

	return neighbours;
}

// Outputs the discovered path as a string
string AStar::getPathString(list<Node*> path) {
	stringstream outputStream;
	for (list<Node*>::iterator it = path.begin(); it != path.end(); ++it) {
		outputStream << "(" << (*it)->posX << "," << (*it)->posY << ")" << endl;
	}
	return outputStream.str();
}

// Outputs path distance
int AStar::getPathDistance(list<Node*> path) {
	return path.size() > 0 ? path.back() -> fCost : 0;
}

// Finds shortest path between startNode and endNode using A* algorithm
list<Node*> AStar::findPath(Node* world[Y_LENGTH][X_LENGTH], Node* startNode, Node* endNode) {
	list<Node*> openSet; // list of nodes not yet evaluated
	list<Node*> closedSet; // list of nodes already evaluated
	Node* currentNode;

	startNode->gCost = 0;
	startNode->hCost = getHDistance(startNode, endNode);
	startNode->fCost = startNode->hCost;
	
	openSet.push_back(startNode); // insert the starting node at the beginning of the open set
	while(!openSet.empty()) {
		// TODO Step4. Find a node in the openSet that has the smallest fCost
		// If there is a conflict, select the node with the smaller hCost
		// Use <list> iterator to iterate through the list; see sample iterator code below
        
        // Store the node with minimal cost for comparison
        list<Node*>::iterator minIt;
        int minFCost = INT_MAX, minHCost = INT_MAX;
        
        for(list<Node*>::iterator it = openSet.begin(); it != openSet.end(); it++) {
            Node* node = *it;
            
            // If the fCost is smaller than the current minimal
            // or equivalent fCost but smaller hCost than current minimal,
            // record the node as the new minimal
            if (node->fCost < minFCost ||
                (node->fCost == minFCost && node->hCost < minHCost)) {
                minIt = it;
                minFCost = node->fCost;
                minHCost = node->hCost;
            }
        }
        
        // TODO Step5. Remove the found node from the open list and insert it into closed list
        currentNode = *minIt;
        openSet.erase(minIt);
        closedSet.push_back(currentNode);
		
		// TODO Step6. Get a list of walkable neighbours for the current node
        list<Node*> neighbours = getNeighbours(currentNode, world);
		
		// TODO Step7. Iterate through the neighbours list and add matching neighbours to the open list		
		for(list<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
            
            Node* neighbour = *it;
            
			// Step7.1. Check if the current neighbour is already in the closed list; if it is, skip it
            bool foundInClosed = false;
            for(list<Node*>::iterator closedIt = closedSet.begin(); closedIt != closedSet.end(); closedIt++) {
                // If the neighbour is found in the closedSet, signal foundInClosed and break the loop
                if (*closedIt == neighbour) {
                    foundInClosed = true;
                    break;
                }
            }
            
            // If the neighbour is found in the closedSet, ignore it and continue to the next one.
            if (foundInClosed) {
                continue;
            }

			// Step7.2. Compute gCost from the start node for the curent neighbour
			// If that cost is less than previously computed gCost, update the neighbour's parent to the current node, and 
			// update gCost, hCost, and fCost values for the neighbour to match the current node
			// Use getHDistance to get the cost from the current node to the current neighour
            
            // If a node has zero hCost, it means the node has not being visited
            bool isUnvisited = neighbour->fCost == 0;
            if (isUnvisited) {
                // Calculate and store the hCost for the node
                neighbour->hCost = getHDistance(neighbour, endNode);
                
                // Add the neighbour to the open set
                openSet.push_back(neighbour);
            }
            
            // Calculate the new gCost based on the current node
            int gCost = getHDistance(neighbour, currentNode) + currentNode->gCost;
            
            // Update the gCost if the node has never been visited or the new
            // gCost is lower than the previously stored gCost
            if (isUnvisited || neighbour->gCost > gCost) {
                neighbour->gCost = gCost;
                neighbour->fCost = getFCost(neighbour);
                
                // Update the parent so the path can be retracted
                neighbour->parent = currentNode;
            }
 		}
 		
 		// TODO Step8. Check if the current node is the end node; if it is, return the retraced path from start to end
        if (currentNode == endNode) {
            return retracePath(startNode, endNode);
        }
	}
	
	list<Node*> empty;
	return empty;
}
