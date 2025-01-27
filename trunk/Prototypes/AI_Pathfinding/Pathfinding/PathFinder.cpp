/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
* @version 1.0
* Copyright (©) A-Team.
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.html
* 
* @section DESCRIPTION
*
* Finds the path
*/

#include "Macros.h"
#include "PathFinder.h"
#include "AgentHandler.h"
#include "EnemyHandler.h"
#include "Agent.h"
#include "Enemy.h"
#include "PotentialInformation.h"
#include <cassert>
#include <iomanip>
#include <string>
#include <map>
#include <sstream>

const int FRAME_TO_DISPLAY = 20;

PathFinder::PathFinder(AgentHandler *pAgentHandler, EnemyHandler *pEnemyHandler, std::list<std::wostream*>* pOutStreams)
{
	mpAgentHandler = pAgentHandler;
	mpEnemyHandler = pEnemyHandler;
	mpOutStreams = pOutStreams;

	assert(mpAgentHandler != NULL);
	assert(mpEnemyHandler != NULL);
	assert(mpOutStreams != NULL);

	mFrame = 0;

	for (int width = 0; width < MAP_WIDTH; width++)
	{
		for (int height = 0; height < MAP_HEIGHT; height++)
		{
			mEnemyPotentialMap[width][height] = 0.0f;
		}
	}

	// Initialize the line separator
	//mLineSeparator = static_cast<unsigned char>(195);
	for (int i = 0; i < MAP_WIDTH*6; i++)
	{
		mLineSeparator += L"-";
		//for (int j  = 0; j < 4; j++)
		//{
		//	mLineSeparator += static_cast<unsigned char>(196);
		//}

		//// Add a ┼
		//if (i < MAP_WIDTH - 1)
		//{
		//	mLineSeparator += static_cast<unsigned char>(197);
		//}
	}
	//mLineSeparator += static_cast<unsigned char>(180);
	mLineSeparator += L"-";
}

PathFinder::~PathFinder()
{
	mpAgentHandler = NULL;
	mpEnemyHandler = NULL;
	mpOutStreams = NULL;
}

void PathFinder::calculate()
{
	//FixedCoordinate tempCoord(18,10);
	//mAgentNextGoalPositions[tempCoord] = 1;
	//mAgentNextGoalPositions.erase(tempCoord);

	//for (int i = 0; i < 1; i++)
	//{
	//	tempCoord.x = 20;
	//	tempCoord.y = 9+i;
	//	mAgentNextGoalPositions[tempCoord] = i+2;
	//}

	//tempCoord.x = 18;
	//tempCoord.y = 10;
	//std::map<FixedCoordinate, KeyType, FixedCoordinateCompare>::iterator it;
	//it = mAgentNextGoalPositions.find(tempCoord);
	//if (it != mAgentNextGoalPositions.end())
	//{
	//	bool test = false;
	//}
	//bool bananas = false;

	std::map<KeyType, Agent*>* pAgents = mpAgentHandler->checkOutAllAgents();
	std::map<KeyType, Agent*>::iterator agentIt;

	static PotentialSquare agentPotentialMap[GOAL_FIELD_SIZE];

	// calculate a new goal for the agents without a goal
	for (agentIt = pAgents->begin(); agentIt != pAgents->end(); ++agentIt)
	{
		PotentialInformation& potentialInfo = agentIt->second->getPotentialInformation();



		if (agentIt->second->arrivedAtGoal())
		{
			// Remove the goal position from the map
			mAgentGoalPositions.erase(potentialInfo.getBestGoalPosition());

			potentialInfo.updateGoalAndPheromones();
	
			// If we don't have a goal calculate one, that means didn't have a valid nextGoal.
			if (potentialInfo.getBestGoalPosition() == INVALID_COORDINATE)
			{
				calculateSurrondingField(GoalType_Goal, agentIt->second, agentPotentialMap);

				sortGoals(agentPotentialMap);

				potentialInfo.setGoals(agentPotentialMap);
			}
			// Remove next goal position from the map
			else
			{
				mAgentNextGoalPositions.erase(potentialInfo.getBestGoalPosition());
			}
		}
		// If we don't have a goal this is our first time
		// TODO - This should never appear here later
		else if (potentialInfo.getBestGoalPosition() == INVALID_COORDINATE)
		{
			calculateSurrondingField(GoalType_Goal, agentIt->second, agentPotentialMap);

			sortGoals(agentPotentialMap);

			potentialInfo.setGoals(agentPotentialMap);
		}

		if (agentIt->second->getPosition().x >= 16.75f)
		{
			bool weAreHere = true;
		}
	}

	// Check for collisions in goal and solve them
	addCollisions(*pAgents, GoalType_Goal);
	solveConflicts(GoalType_Goal, *pAgents);

	// Calculate a new next goal for the agents without a next goal
	for (agentIt = pAgents->begin(); agentIt != pAgents->end(); ++agentIt)
	{
		PotentialInformation& potentialInfo = agentIt->second->getPotentialInformation();
		
		// If we don't have a next goal calculate one
		if (potentialInfo.getBestNextGoalPosition() == INVALID_COORDINATE)
		{
			calculateSurrondingField(GoalType_NextGoal, agentIt->second, agentPotentialMap);

			sortGoals(agentPotentialMap);

			potentialInfo.setNextGoals(agentPotentialMap);
		}
	}

	// Check for collisions in next goals and solve them
	addCollisions(*pAgents, GoalType_NextGoal);
	solveConflicts(GoalType_NextGoal, *pAgents);

	mpAgentHandler->checkInAllAgents();
	
	mFrame++;
	if (mFrame % FRAME_TO_DISPLAY == 0)
	{
		// Only print the current frame and map each 10th frame
		std::list<std::wostream*>::iterator it;
		for (it = mpOutStreams->begin(); it != mpOutStreams->end(); ++it)
		{
			**it << "Current Frame: " << mFrame << std::endl;
		}

		printMap();
	}
}

void PathFinder::solveConflicts(GoalTypes goalType, std::map<KeyType, Agent*>& agents)
{
	Agent* pFirstAgent = NULL;
	Agent* pCollidingAgent = NULL;
	std::map<FixedCoordinate, KeyType, FixedCoordinateCompare>::iterator keyIt;
	std::map<KeyType, Agent*>::iterator agentIt;

	switch (goalType) {
		case GoalType_Goal:
			while (!mCollidingAgents.empty())
			{
				pCollidingAgent = mCollidingAgents.getFirst();
				//pFirstAgent = agents[mAgentGoalPositions[pCollidingAgent->getPotentialInformation().getBestGoalPosition()]];
				keyIt = mAgentGoalPositions.find(pCollidingAgent->getPotentialInformation().getBestGoalPosition());
				assert(keyIt != mAgentGoalPositions.end());
				agentIt = agents.find(keyIt->second);
				assert(agentIt != agents.end());
				pFirstAgent = agentIt->second;
				solveGoalConflict(pFirstAgent, pCollidingAgent);
				mCollidingAgents.removeFirst();
			}
			break;

			
		case GoalType_NextGoal:
			while (!mCollidingAgents.empty())
			{
				pCollidingAgent = mCollidingAgents.getFirst();
				//pFirstAgent = agents[mAgentNextGoalPositions[pCollidingAgent->getPotentialInformation().getBestNextGoalPosition()]];
				keyIt = mAgentNextGoalPositions.find(pCollidingAgent->getPotentialInformation().getBestNextGoalPosition());
				assert(keyIt != mAgentNextGoalPositions.end());
				agentIt = agents.find(keyIt->second);
				assert(agentIt != agents.end());
				pFirstAgent = agentIt->second;
				solveNextGoalConflict(pFirstAgent, pCollidingAgent);
				mCollidingAgents.removeFirst();
			}
			break;
	}
}

void PathFinder::solveGoalConflict(Agent *pFirstAgent, Agent *pCollidingAgent)
{
	// Set the remove agent as the one furthest away from the goal
	PotentialInformation& firstAgentPotential = pFirstAgent->getPotentialInformation();
	PotentialInformation& collidingAgentPotential = pCollidingAgent->getPotentialInformation();

	PotentialInformation* pNewGoalPotential = NULL;
	PotentialInformation* pNotNewGoalPotential = NULL;
	Agent* pNewGoalAgent = NULL;
	Agent* pNotNewGoalAgent = NULL;

	// If the colliding agent's second goal is better than the first agent's, use that one.
	// use that ones
	if (collidingAgentPotential.getGoals()[1].value > firstAgentPotential.getGoals()[1].value)
	{
		pNewGoalPotential = &collidingAgentPotential;
		pNotNewGoalPotential = &firstAgentPotential;
		pNewGoalAgent = pCollidingAgent;
		pNotNewGoalAgent = pFirstAgent;
	}
	// else if the first agent's second goal is better than colliding agent's, use that one.
	else if (firstAgentPotential.getGoals()[1].value > collidingAgentPotential.getGoals()[1].value)
	{
		pNewGoalPotential = &firstAgentPotential;
		pNotNewGoalPotential = &collidingAgentPotential;
		pNewGoalAgent = pFirstAgent;
		pNotNewGoalAgent = pCollidingAgent;
	}
	// Second goal position is equal, prioritize the one that's closest
	else
	{
		if ((pFirstAgent->getPosition() - firstAgentPotential.getBestGoalPosition()).longerThan(
			(pCollidingAgent->getPosition() - collidingAgentPotential.getBestGoalPosition())))
		{
			pNewGoalPotential = &firstAgentPotential;
			pNotNewGoalPotential = &collidingAgentPotential;
			pNewGoalAgent = pFirstAgent;
			pNotNewGoalAgent = pCollidingAgent;
		}
		else
		{
			pNewGoalPotential = &collidingAgentPotential;
			pNotNewGoalPotential = &firstAgentPotential;
			pNewGoalAgent = pCollidingAgent;
			pNotNewGoalAgent = pFirstAgent;
		}
	}

	// Check so that we can remove the goal, i.e. has more or equal to 2 goals.
	// If we have we swap to the other agent.
	if (pNewGoalPotential->getGoals()[1].value == INVALID_POTENTIAL_VALUE)
	{
		std::swap(pNewGoalPotential, pNotNewGoalPotential);
		std::swap(pNewGoalAgent, pNotNewGoalAgent);
	}

	// Remove the best goal and move all goals one step
	PotentialSquare* goals = pNewGoalPotential->getGoals();
	
	int i = 0;
	while (i < GOAL_FIELD_SIZE-1 && goals[i].value != INVALID_POTENTIAL_VALUE)
	{
		goals[i].position = goals[i+1].position;
		goals[i].value = goals[i+1].value;
		i++;
	}

	// Invalidate the last goal
	goals[i].position = INVALID_COORDINATE;
	goals[i].value = INVALID_POTENTIAL_VALUE;
	std::map<FixedCoordinate, KeyType, FixedCoordinateCompare>::iterator keyIt;

	// Check if we have a new collision with the new goal.
	// Add to the conclicting agent to the colliding agent queue
	if (checkCollision(GoalType_Goal, pNewGoalAgent))
	{
		mCollidingAgents.add(pNewGoalAgent);
	}
	// Add to the agent goal position
	else
	{
		mAgentGoalPositions[goals[0].position] = pNewGoalAgent->getKey();
	}
	mAgentGoalPositions[pNotNewGoalPotential->getBestGoalPosition()] = pNotNewGoalAgent->getKey();
}

void PathFinder::solveNextGoalConflict(Agent *pFirstAgent, Agent *pCollidingAgent)
{
	// Set the remove agent as the one furthest away from the goal
	PotentialInformation& firstAgentPotential = pFirstAgent->getPotentialInformation();
	PotentialInformation& collidingAgentPotential = pCollidingAgent->getPotentialInformation();

	PotentialInformation* pNewGoalPotential = NULL;
	PotentialInformation* pNotNewGoalPotential = NULL;
	Agent* pNewGoalAgent = NULL;
	Agent* pNotNewGoalAgent = NULL;

	// If the colliding agent's second goal is better than the first agent's, use that one.
	// use that ones
	if (collidingAgentPotential.getNextGoals()[1].value > firstAgentPotential.getNextGoals()[1].value)
	{
		pNewGoalPotential = &collidingAgentPotential;
		pNotNewGoalPotential = &firstAgentPotential;
		pNewGoalAgent = pCollidingAgent;
		pNotNewGoalAgent = pFirstAgent;
	}
	// else if the first agent's second goal is better than colliding agent's, use that one.
	else if (firstAgentPotential.getNextGoals()[1].value > collidingAgentPotential.getNextGoals()[1].value)
	{
		pNewGoalPotential = &firstAgentPotential;
		pNotNewGoalPotential = &collidingAgentPotential;
		pNewGoalAgent = pFirstAgent;
		pNotNewGoalAgent = pCollidingAgent;
	}
	// Second goal position is equal, prioritize the one that's closest
	else
	{
		if ((firstAgentPotential.getBestGoalPosition() - firstAgentPotential.getBestNextGoalPosition()).longerThan(
			(collidingAgentPotential.getBestGoalPosition() - collidingAgentPotential.getBestNextGoalPosition())))
		{
			pNewGoalPotential = &firstAgentPotential;
			pNotNewGoalPotential = &collidingAgentPotential;
			pNewGoalAgent = pFirstAgent;
			pNotNewGoalAgent = pCollidingAgent;
		}
		else
		{
			pNewGoalPotential = &collidingAgentPotential;
			pNotNewGoalPotential = &firstAgentPotential;
			pNewGoalAgent = pCollidingAgent;
			pNotNewGoalAgent = pFirstAgent;
		}
	}
	
	// Check so that we can remove the goal, i.e. has more or equal to 2 goals.
	// If we have we swap to the other agent.
	if (pNewGoalPotential->getNextGoals()[1].value == INVALID_POTENTIAL_VALUE)
	{
		std::swap(pNewGoalPotential, pNotNewGoalPotential);
		std::swap(pNewGoalAgent, pNotNewGoalAgent);
	}

	// Remove the best goal and move all goals one step
	PotentialSquare* goals = pNewGoalPotential->getNextGoals();
	
	int i = 0;
	while (i < GOAL_FIELD_SIZE-1 && goals[i].value != INVALID_POTENTIAL_VALUE)
	{
		goals[i].position = goals[i+1].position;
		goals[i].value = goals[i+1].value;
		i++;
	}

	// Invalidate the last goal
	goals[i].position = INVALID_COORDINATE;
	goals[i].value = INVALID_POTENTIAL_VALUE;

	// Check if we have a new collision with the new goal.
	// Add to the conclicting agent to the colliding agent queue
	if (checkCollision(GoalType_NextGoal, pNewGoalAgent))
	{
		mCollidingAgents.add(pNewGoalAgent);
	}
	// Add to the agent goal positions
	else
	{
		mAgentNextGoalPositions[goals[0].position] = pNewGoalAgent->getKey();
	}
	mAgentNextGoalPositions[pNotNewGoalPotential->getBestNextGoalPosition()] = pNotNewGoalAgent->getKey();
}

void PathFinder::sortGoals(PotentialSquare *goals)
{
	// Bubblesort
	for (int i = 0; i < GOAL_FIELD_SIZE-1; i++)
	{
		for (int j = i+1; j < GOAL_FIELD_SIZE; j++)
		{
			if (goals[j].value > goals[i].value)
			{
				std::swap(goals[j], goals[i]);
			}
		}
	}
}

void PathFinder::addCollisions(const std::map<KeyType, Agent*>& agents, GoalTypes goalType)
{
	std::map<KeyType, Agent*>::const_iterator agentIt;

	// Find all colliding agents only save second, third, etc. Not first
	for (agentIt = agents.begin(); agentIt != agents.end(); ++agentIt)
	{
		// If there already is a goal in that position we store the agent as a conflict
		if (checkCollision(goalType, agentIt->second))
		{
			mCollidingAgents.add(agentIt->second);
		}
		// Insert the agent into the map
		else
		{
			switch (goalType)
			{
				case GoalType_Goal:
					mAgentGoalPositions[agentIt->second->getPotentialInformation().getBestGoalPosition()] = agentIt->first;
					break;
				case GoalType_NextGoal:
					mAgentNextGoalPositions[agentIt->second->getPotentialInformation().getBestNextGoalPosition()] = agentIt->first;
					break;
				default:
					DEBUG_MESSAGE("Not valid goal-type, exiting...");
					assert(false);
					break;
			}
		}
	}
}

bool PathFinder::checkCollision(GoalTypes goalType, Agent* pAgent)
{
	std::map<FixedCoordinate, KeyType, FixedCoordinateCompare>::iterator it;
	switch (goalType)
	{
	case GoalType_Goal:
		it = mAgentGoalPositions.find(pAgent->getPotentialInformation().getBestGoalPosition());
		// If we don't find it, and it's not ourselves we found, that add it as a collision
		if (it != mAgentGoalPositions.end() && it->second != pAgent->getKey())
		{
			return true;
		}
		else
		{
			return false;
		}
	case GoalType_NextGoal:
	{
		FixedCoordinate bestCoord = pAgent->getPotentialInformation().getBestNextGoalPosition();
		it = mAgentNextGoalPositions.find(bestCoord);
		// If we don't find it, and it's not ourselves we found, that add it as a collision
		if (it != mAgentNextGoalPositions.end() && it->second != pAgent->getKey())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	default:
		DEBUG_MESSAGE("Not valid goal-type, exiting...");
		assert(false);
		break;
	}
	return false;
}

void PathFinder::addEnemyToEnemyPotentialMap(Enemy* pEnemy)
{
	for (int width = 0; width < MAP_WIDTH; width++)
	{
		for (int height = 0; height < MAP_HEIGHT; height++)
		{
			float distance = Coordinate(width, height) * pEnemy->getPosition();
			mEnemyPotentialMap[width][height] += pEnemy->calculatePotentialValue(distance);
		}
	}
}

void PathFinder::calculateSurrondingField(GoalTypes goalType, Agent *pAgent, PotentialSquare* field)
{
	FixedCoordinate agentCoord;
	if (goalType == GoalType_Goal)
	{
		agentCoord.x = static_cast<int>(pAgent->getPosition().x);
		agentCoord.y = static_cast<int>(pAgent->getPosition().y);
	}
	else if (goalType == GoalType_NextGoal)
	{
		agentCoord = pAgent->getPotentialInformation().getBestGoalPosition();
	}
	int fieldX = 0;
	int fieldY = 0;
	for (int y = 0; y < GOAL_FIELD_HEIGHT; y++)
	{
		for (int x = 0; x < GOAL_FIELD_WIDTH; x++)
		{
			fieldX = agentCoord.x - 1 + x;
			fieldY = agentCoord.y - 1 + y;

			// Be sure to not go out of bounds on the enemy potential map
			if (fieldX >= 0 &&
				fieldX < MAP_WIDTH &&
				fieldY >= 0 && 
				fieldY < MAP_HEIGHT)
			{
				field[x + y*GOAL_FIELD_HEIGHT].value = mEnemyPotentialMap[fieldX][fieldY];
				field[x + y*GOAL_FIELD_HEIGHT].position = FixedCoordinate(fieldX, fieldY);
			}
			else
			{
				field[x + y*GOAL_FIELD_HEIGHT].value = INVALID_POTENTIAL_VALUE;
				field[x + y*GOAL_FIELD_HEIGHT].position = INVALID_COORDINATE;
			}
			

		}
	}

	// TODO - update the information with the agent's pheromone trail

}

void PathFinder::printMap()
{
	KeyType agentMap[MAP_WIDTH][MAP_HEIGHT];
	KeyType enemyMap[MAP_WIDTH][MAP_HEIGHT];

	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			agentMap[i][j] = INVALID_KEY;
		}
	}

	for (int i = 0; i < MAP_WIDTH; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			enemyMap[i][j] = INVALID_KEY;
		}
	}

	// Iterate through all the agents and put the key in the local map
	std::map<KeyType, Agent*>* agents = mpAgentHandler->checkOutAllAgents();
	std::map<KeyType, Agent*>::iterator agentIt;
	for (agentIt = agents->begin(); agentIt != agents->end(); ++agentIt)
	{
		Coordinate position = agentIt->second->getPosition();
		agentMap[static_cast<int>(position.x+0.5f)][static_cast<int>(position.y+0.5f)] = agentIt->first;
	}
	mpAgentHandler->checkInAllAgents();

	// Iterate through all the enemies
	std::map<KeyType, Enemy*>* enemies = mpEnemyHandler->checkOutAllEnemies();
	std::map<KeyType, Enemy*>::iterator enemyIt;
	for (enemyIt = enemies->begin(); enemyIt != enemies->end(); ++enemyIt)
	{
		FixedCoordinate position = enemyIt->second->getPosition();
		enemyMap[position.x][position.y] = enemyIt->first;
	}
	mpEnemyHandler->checkInAllEnemies();

	// Print the map
	std::list<std::wostream*>::iterator it;
	for (it = mpOutStreams->begin(); it != mpOutStreams->end(); ++it)
	{
		printLine(**it, true);
		std::wstring text;
		std::wstringstream ss;
		std::wostream& out = **it;

		for (int height = 0; height < MAP_HEIGHT; height++)
		{
			// Print the id
			for (int width = 0; width < MAP_WIDTH; width++)
			{
				ss.clear();
				text = L"";
				// Check the agent and enemy map
				if (agentMap[width][height] != INVALID_KEY)
				{
					ss << agentMap[width][height];
				}
				else if (enemyMap[width][height] != INVALID_KEY)
				{
					ss << enemyMap[width][height];
				}
				ss >> text;

				out << L"|" << std::setw(5) << text;
			}
			out << L"|" << std::endl;

			// Print additional information
			for (int width = 0; width < MAP_WIDTH; width++)
			{
				// Check the agent and enemy map
				if (agentMap[width][height] != INVALID_KEY)
				{
					// get the goal position
					Agent* pAgent = mpAgentHandler->checkOutAgent(agentMap[width][height]);
					FixedCoordinate goal = pAgent->getPotentialInformation().getBestGoalPosition();
					FixedCoordinate nextGoal = pAgent->getPotentialInformation().getBestNextGoalPosition();
					mpAgentHandler->checkInAgent(agentMap[width][height]);

					// current position
					FixedCoordinate curPos;
					curPos.x = width;
					curPos.y = height;

					text = getArrow(curPos, goal);
					text += L" ";
					text += getArrow(goal, nextGoal);
				}
				else if (enemyMap[width][height] != INVALID_KEY)
				{
					text = L"XXXXX";
				}
				else
				{
					text = L"";
				}

				out << L"|" << std::setw(5) << text;
			}
			out << L"|" << std::endl;
			out.setf(std::ios::fixed, std::ios::floatfield);
			out.precision(2);
			
			// Print the potential info
			for (int width = 0; width < MAP_WIDTH; width++)
			{
				out << L"|" << std::setw(5) << mEnemyPotentialMap[width][height] << text;
			}

			
			out << L"|" << std::endl;
			
			if (height < MAP_HEIGHT - 1)
			{
				printLine(**it);
			}
		}

		printLine(**it, false, true);

		out << "\n\n";
	}
}

void PathFinder::printLine(std::wostream& out, bool firstLine, bool lastLine)
{
	out << mLineSeparator << std::endl;
}

std::wstring PathFinder::getArrow(const FixedCoordinate& from, const FixedCoordinate& to)
{
	// up
	if (to.y < from.y)
	{
		// left
		if (to.x < from.x)
		{
			return L"<\\";
		}
		// right
		else if (to.x > from.x)
		{
			return L"/>";
		}
		// only up
		else
		{
			return L"/\\";
		}
	}
	// down
	else if (to.y > from.y)
	{
		// left
		if (to.x < from.x)
		{
			return L"</";
		}
		// right
		else if (to.x > from.x)
		{
			return L"\\>";
		}
		// only down
		else
		{
			return L"\\/";
		}
	}
	else
	{
		// left
		if (to.x < from.x)
		{
			return L"<-";
		}
		// right
		else if (to.x > from.x)
		{
			return L"->";
		}
		// same place
		else
		{
			return L"[]";
		}
	}
}