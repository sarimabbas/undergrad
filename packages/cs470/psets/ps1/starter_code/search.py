# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util
from game import Actions, Directions


class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


class Frontier:
    def __init__(self, problem, algo, heuristic=None):
        self.algo = algo
        if algo == dfs:
            self.frontier = util.Stack()
        elif algo == bfs:
            self.frontier = util.Queue()
        elif algo == ucs:
            # the cost is just the 1st component of each triple (Direction)
            # passed to the problem util
            cost = lambda path: problem.getCostOfActions([x[1] for x in path][1:])
            self.frontier = util.PriorityQueueWithFunction(cost)
        elif algo == astar:
            # f(x) = g(x) + h(x)
            # g(x) = lambda path: problem.getCostOfActions([x[1] for x in path][1:])
            # h(x) = heuristic(path[-1][0], problem)
            cost = lambda path: problem.getCostOfActions(
                [x[1] for x in path][1:]) + heuristic(path[-1][0], problem)
            self.frontier = util.PriorityQueueWithFunction(cost)

    def add(self, toAdd):
        self.frontier.push(toAdd)

    def choose(self):
        return self.frontier.pop()

    def isEmpty(self):
        return self.frontier.isEmpty()


def recoverSolution(path):
    # first get a list of all directions
    # [Stop, West, East, West, North...]
    # and then remove Stop from it via [1:]
    return [x[1] for x in path][1:]


def genericGraphSearch(problem, algo, heuristic=None):
    # init the frontier using the initial state
    frontier = Frontier(problem, algo, heuristic)
    frontier.add([(problem.getStartState(), "Stop", 0)])
    # init the explored set to be empty
    explored = []
    # loop do
    while True:
        # if nothing to choose, no directions
        if frontier.isEmpty():
            return []
        # choose a leaf and remove it from the frontier
        # this is actually an entire path uptil that point
        leaf_path = frontier.choose()
        leaf_node = leaf_path[-1][0]
        # if the node contains a goal state, then return the corresponding solution
        if problem.isGoalState(leaf_node):
            solution =  recoverSolution(leaf_path)
            return solution
        # add the node to the explored set
        if leaf_node not in explored:
            explored.append(leaf_node)
            # expand the chosen node
            successors = problem.getSuccessors(leaf_node)
            for successor in successors:
                # add the resulting nodes to the frontier
                # only if not in the explored set...
                if successor[0] in explored:
                    continue
                successor_path = leaf_path[:]
                successor_path.append(successor)
                # add to frontier
                frontier.add(successor_path)

def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    s = Directions.SOUTH
    w = Directions.WEST
    print(problem.isGoalState((1, 1)))
    return [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    return(genericGraphSearch(problem, dfs))


# * BFS finds an optimal solution when all step costs are equal, because it
# * always expands the shallowest unexpanded node.
def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    return(genericGraphSearch(problem, bfs))


def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    return(genericGraphSearch(problem, ucs))


def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0


def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    return(genericGraphSearch(problem, astar, heuristic))

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
