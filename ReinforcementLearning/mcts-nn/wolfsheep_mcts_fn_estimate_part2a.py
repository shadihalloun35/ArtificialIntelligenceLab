from math import *
import random
import numpy as np

class GameState:
    """ A state of the game, i.e. the game board. These are the only functions which are
        absolutely necessary to implement UCT in any 2-player complete information deterministic 
        zero-sum game, although they can be enhanced and made quicker, for example by using a 
        GetRandomMove() function to generate a random move during rollout.
        By convention the players are numbered 1 and 2.
    """
    def __init__(self):
            self.playerJustMoved = 2 # At the root pretend the player just moved is player 2 - player 1 has the first move
        
    def Clone(self):
        """ Create a deep clone of this game state.
        """
        st = GameState()
        st.playerJustMoved = self.playerJustMoved
        return st

    def DoMove(self, move):
        """ Update a state by carrying out the given move.
            Must update playerJustMoved.
        """
        self.playerJustMoved = 3 - self.playerJustMoved
        
    def GetMoves(self):
        """ Get all possible moves from this state.
        """
    
    def GetResult(self, playerjm):
        """ Get the game result from the viewpoint of playerjm. 
        """

    def __repr__(self):
        """ Don't need this - but good style.
        """
        pass



class Piece:
    def __init__(self):
        self.pos_x = -1
        self.pos_y = -1


class Wolf(Piece):
    def __init__(self):
        Piece.__init__(self)
        self.value = 'W'


class Sheep(Piece):
    def __init__(self, number):
        Piece.__init__(self)
        self.value = str(number)

class WolfSheepMove(object):
    def __init__(self,idx,_x, _y, x, y, value):
        self.idx = idx
        self._x = _x
        self._y = _y
        self.x = x
        self.y = y
        self.value = value

    def __repr__(self):
        return "index:"+str(self.idx)+" _x:" + str(self._x) + " _y:" + str(self._y)+" x:" + str(self.x) + " y:" + str(self.y) + " v:" + str(self.value)
class WSState:
    w = 1
    s = 2
    pos = 2
    __wolf_directions = [(-1,-1),(-1,1),(1,-1),(1,1)]
    __sheep_directions = [(-1,1),(1,1)]
    theta = [1.0,0.2,0.2,0.2,0.2]#[0.46826272 ,0.56510358, 0.94505812, 0.48647976, 0.01876345]
    def __init__(self,sz = 8):
        self.size = sz
        self.board = np.zeros((sz,sz))
        self.playerJustMoved = 2 # At the root pretend the player just moved is p2 - p1 has the first move
        for counter in range(4):
            self.board[(counter * 2) + 1][0] = WSState.s
        self.board[self.pos * 2] [7] = WSState.w
        #init move here
    

    def Clone(self):
        """ Create a deep clone of this game state.
        """
        st = WSState()
        st.playerJustMoved = self.playerJustMoved
        st.board = np.zeros((self.size,self.size))
        for i in  range(self.size):
            for j in range (self.size):
                st.board[i][j] = self.board[i][j] 
        st.size = self.size
        return st
    def Estimated_V(self,playerjm):
        state = []
        
        wolf,sheeps, squares = self.get_characters()
        max_y_sheep = max([s.pos_y for s in sheeps])
        wolf_to_goal = abs(max_y_sheep - wolf.pos_y)
        if playerjm==WSState.w:
            state.append(8-wolf_to_goal)
            for s in sheeps:
                state.append(min(abs(s.pos_x - wolf.pos_x),abs(s.pos_y - wolf.pos_y)))
        else:
            state.append(wolf_to_goal)
            for s in sheeps:
                state.append(8-min(abs(s.pos_x - wolf.pos_x),abs(s.pos_y - wolf.pos_y)))

        return np.dot(np.array(self.theta) , np.array(state))
    def get_characters(self):
        sheep_indices = np.where(self.board == WSState.s)
        sheeps = []
        for i, coords in enumerate(list(zip(sheep_indices[0], sheep_indices[1]))):
            sheep = Sheep(i)
            sheep.pos_x = coords[0]
            sheep.pos_y = coords[1]
            sheeps.append(sheep)
        wolf_indices = np.where(self.board == WSState.w)
        wolf = Wolf()
        wolf.pos_x=wolf_indices[0][0]
        wolf.pos_y=wolf_indices[1][0]
        squares = [['w' if (x + y) % 2 == 0 else 'b' for y in range(self.size)] for x in range(self.size)]
        for idx, sheep in enumerate(sheeps):
            squares[sheep.pos_x][sheep.pos_y] = 'S'
        squares[wolf.pos_x][wolf.pos_y] = 'W'
        return wolf, sheeps, squares

    


    def GetMoves(self):
        #print('get_legal_actions')
        wolf,sheeps, squares = self.get_characters()
        if all([wolf.pos_y <= sheeps[i].pos_y for i in range(len(sheeps))]):
            return []
        legal_actions = []
        wolf,sheeps, squares = self.get_characters()
        if self.playerJustMoved == WSState.s:
            legal_actions = [WolfSheepMove(-1,wolf.pos_x,wolf.pos_y,wolf.pos_x + d[0], wolf.pos_y + d[1], WSState.w)   for d in self.__wolf_directions ]
            legal_actions = [a for a in legal_actions if a.x>=0 and a.x<self.size and a.y>=0 and a.y<self.size and self._is_wolf_move_valid(a.x,a.y)]
            #print('****legal actions wolf', legal_actions)
        elif self.playerJustMoved == WSState.w:
            for idx, sheep in enumerate(sheeps):
                if sheep.pos_x < 0:
                    print('No!!!!!!!')
                actions = [WolfSheepMove(idx, sheep.pos_x, sheep.pos_y, sheep.pos_x + d[0], sheep.pos_y + d[1], WSState.s)  for d in self.__sheep_directions if self._is_sheep_move_valid(sheep.pos_x + d[0],sheep.pos_y + d[1],idx)]

                actions = [a for a in actions if a.x>=0 and a.x<self.size and a.y>=0 and a.y<self.size]
                #remove duplicate
                unique_actions = []
                for a in actions:
                    if len(unique_actions)==0:
                        unique_actions.append(a)
                    else:
                        flag = False
                        for u in unique_actions:
                            if  (a.x == u.x and a.y == u.y):
                                flag = True
                        if not flag:
                            unique_actions.append(a)
                for a in unique_actions:
                    if len(legal_actions) == 0:
                        legal_actions.append(a)
                    else:
                        flag = False
                        for l in legal_actions:
                            if (a.x == l.x and a.y == l.y):
                                flag = True

                        if not flag:
                            legal_actions.append(a)
        else:
            return []
        #print('****legal_actions', legal_actions)
        
        return legal_actions
    def DoMove(self, move):
        """ Update a state by carrying out the given move.
            Must update playerToMove.
        """
        (idx,_x,_y,x,y)=(move.idx, move._x,move._y,move.x,move.y)
        
        self.playerJustMoved = 3 - self.playerJustMoved
        self.board[_x][_y] = 0
        self.board[x][y] = self.playerJustMoved

    def GetResult(self, playerjm):
        

        if playerjm == WSState.w :
            if self._is_wolf_win():
                return 1
            else:
                return 0
        elif playerjm == WSState.s:
            if self._is_sheep_win():
                return 0
            elif self._is_sheep_win():
                return 1
            else:
                return 0

        
        if self.GetMoves() == []: return 0.5 # draw
        assert False # Should not be possible to get here

    def _is_wolf_move_valid(self, x, y):
        wolf,sheeps, squares = self.get_characters()
        if 0 <= x <= 7 and 0 <= y <= 7:
            #print(x ,  self.wolf.pos_x)

            if abs(x - wolf.pos_x) == 1 \
                    and abs(y - wolf.pos_y) == 1 \
                    and squares[x][y] == 'b':
                return True
            else:
                #print('a')
                return False
        else:
            #print('b')
            return False

    # Sheep can move forward only (increasing y-coordinate)
    def _is_sheep_move_valid(self, x, y, j_no):
        wolf,sheeps, squares = self.get_characters()
        if 0 <= x <= 7 and 0 <= y <= 7:
            if y - sheeps[j_no].pos_y == 1 and abs(
                    x - sheeps[j_no].pos_x) == 1 and \
                    squares[x][y] == 'b':
                return True
            else:
                return False
        else:
            #print('stupid wrong')
            return False
    def _is_wolf_win(self):
        wolf,sheeps, squares = self.get_characters()
        if all([wolf.pos_y <= sheeps[i].pos_y for i in range(len(sheeps))]):
            return True
        moves = self.GetMoves()
        if len(moves) ==  0:
            return True
        return False

    def _is_sheep_win(self):
        moves = self.GetMoves()
        if len(moves) ==  0:
            return True
        return False
    def show_boad_game(self):
        s = ''
        for i in range(self.size):
            print("")
            print("{0:3}".format(i).center(8)+"|", end='')
            for j in range(self.size):
                if self.board[j][i] == 0:
                    print('_'.center(8), end='')
                if self.board[j][i] == WSState.w:
                    print('W'.center(8), end='')
                if self.board[j][i] == WSState.s:
                    print('S'.center(8), end='')
        print("")
        print("______________________________")


class Node:
    """ A node in the game tree. Note wins is always from the viewpoint of playerJustMoved.
        Crashes if state not specified.
    """
    def __init__(self, move = None, parent = None, state = None):
        self.move = move # the move that got us to this node - "None" for the root node
        self.parentNode = parent # "None" for the root node
        self.childNodes = []
        self.wins = 0
        self.visits = 0
        self.untriedMoves = state.GetMoves() # future child nodes
        self.playerJustMoved = state.playerJustMoved # the only part of the state that the Node needs later
        
    def SelectChild(self):
        """ Use the UCB1 formula to select a child node. Often a constant UCTK is applied so we have
            lambda c: c.wins/c.visits + UCTK * sqrt(2*log(self.visits)/c.visits to vary the amount of
            exploration versus exploitation.
        """
        s = sorted(self.childNodes, key = lambda c: c.wins/c.visits + sqrt(2*log(self.visits)/c.visits))[-1]
        return s
    
    def AddChild(self, m, s):
        """ Remove m from untriedMoves and add a new child node for this move.
            Return the added child node
        """
        n = Node(move = m, parent = self, state = s)
        list(self.untriedMoves).remove(m)
        self.childNodes.append(n)
        return n
    
    def Update(self, result):
        """ Update this node - one additional visit and result additional wins. result must be from the viewpoint of playerJustmoved.
        """
        self.visits += 1
        self.wins += result

    def __repr__(self):
        return "[M:" + str(self.move) + " W/V:" + str(self.wins) + "/" + str(self.visits) + " U:" + str(self.untriedMoves) + "]"

    def TreeToString(self, indent):
        s = self.IndentString(indent) + str(self)
        for c in self.childNodes:
             s += c.TreeToString(indent+1)
        return s

    def IndentString(self,indent):
        s = "\n"
        for i in range (1,indent+1):
            s += "| "
        return s

    def ChildrenToString(self):
        s = ""
        for c in self.childNodes:
             s += str(c) + "\n"
        return s


def Get_Best_Move(rootstate, itermax, verbose = False):
    """ Conduct a UCT search for itermax iterations starting from rootstate.
        Return the best move from the rootstate.
        Assumes 2 alternating players (player 1 starts), with game results in the range [0.0, 1.0]."""

    rootnode = Node(state = rootstate)

    for i in range(itermax):
        node = rootnode
        state = rootstate.Clone()

        # Select
        while node.untriedMoves == [] and node.childNodes != []: # node is fully expanded and non-terminal
            node = node.SelectChild()
            state.DoMove(node.move)

        # Expand
        if node.untriedMoves != []: # if we can expand (i.e. state/node is non-terminal)
            m = random.choice(node.untriedMoves) 
            state.DoMove(m)
            node = node.AddChild(m,state) # add child and descend tree
        '''
        # Rollout - this can often be made orders of magnitude quicker using a state.GetRandomMove() function
        while state.GetMoves() != []: # while state is non-terminal
            state.DoMove(random.choice(state.GetMoves()))
        '''
        #est_v = state.estimated_v()

        # Backpropagate
        flag = True
        while node != None: # backpropagate from the expanded node and work back to the root node
            #node.Update(state.GetResult(node.playerJustMoved)) # state is terminal. Update node with result from POV of node.playerJustMoved
            node.Update(state.Estimated_V(node.playerJustMoved))
            #print(state.Estimated_V(node.playerJustMoved))
            node = node.parentNode

    # Output some information about the tree - can be omitted
    #if (verbose): print (rootnode.TreeToString(0))
    #else: print (rootnode.ChildrenToString())

    return sorted(rootnode.childNodes, key = lambda c: c.visits)[-1].move # return the move that was most visited
                
def MCTSPlayGame():
    """ Play a sample game between two UCT players where each player gets a different number 
        of UCT iterations (= simulations = tree nodes).
    """
    # state = OthelloState(4) # uncomment to play Othello on a square board of the given size
    #state = OXOState() # uncomment to play OXO
    #state = NimState(15) # uncomment to play Nim with the given number of starting chips
    steps = 0
    state = WSState()
    while (state.GetMoves() != []):
        #print (str(state))
        state.show_boad_game()
        if state.playerJustMoved == 1:
            best_move = Get_Best_Move(rootstate = state, itermax = 10, verbose = False) # play with values for itermax and verbose = True
        else:
            best_move = Get_Best_Move(rootstate = state, itermax = 25000, verbose = False )
        print ("Best Move: " + str(best_move) + "\n")
        steps = steps + 1
        state.DoMove(best_move)
    print ('state.playerJustMoved ',state.playerJustMoved,state.GetResult(state.playerJustMoved))
    if state.GetResult(state.playerJustMoved) == 1.0:
        print ("Player " + str(state.playerJustMoved) + " wins!")
    elif state.GetResult(state.playerJustMoved) == 0.0:
        print ("Player " + str(3 - state.playerJustMoved) + " wins!")
    else: print ("Nobody wins!")
    print('Complete after',steps,'steps')

if __name__ == "__main__":
    """ Play a single game to the end using MCTS for both players. 
    """
    MCTSPlayGame()