"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, T. Gautier
Licence: GPL

File: Snake.py
	Contains the class Snake
	-> defines the Snake game (its rules, moves, etc.)

Copyright 2019 T. Hilaire, T. Gautier
"""
from typing import Tuple, Union, List
from random import randint, seed
from CGSserver.Constants import NORMAL_MOVE, LOSING_MOVE
from .Constants import NORTH, SOUTH, EAST, WEST, Ddx, Ddy, DRAWING_BOX, HORIZONTAL_BOX, VERTICAL_BOX, BOX, TRIANGLES
from .Constants import DEFAULT_DIFFICULTY
from CGSserver.Game import Game
from colorama import Fore, Style
from re import compile
from itertools import product
from ansi2html import Ansi2HTMLConverter

# import here your training players
from .RandomPlayer import RandomPlayer
from .SuperPlayer import SuperPlayer

import logging
logger = logging.getLogger("Snake")  # general logger ('root')

regd = compile(r"(\d+)")  # regex to parse a "%d" string


class Arena:
	"""
	the arena is an array of integers

	In intern, for each box x,y of the arean
	- bits 0 to 3 are for the wall (arena[x][y] & (1<<DIR) -> tells if there is a wall in direction DIR)
	- bits 7 and 8 are for the players 0 and 1
	the class Box just encapsulates this
	"""
	def __init__(self, L, H, difficulty):
		"""create a game
		Returns an arena (array of integers) and a set of walls"""
		self._L = L
		self._H = H
		# create a L*H array of 0)
		self._array = [[0 for _ in range(H)] for _ in range(L)]
		self._walls = []
		# fill with random walls according to the difficulty
		if not 0 <= difficulty <= 3:
			difficulty = DEFAULT_DIFFICULTY
		nbWalls = [0, L*H//5, L*H//3, L*H][difficulty]
		for i in range(nbWalls):
			x = randint(1, L-2)
			y = randint(1, H-2)
			direction = randint(0, 3)
			# check if there is no wall around the start positions
			if (2-1 <= x <= 2+1 and H//2-1 <= y <= H//2+1) or ( L-3-1 <= x <= L-3+1 and H//2-1 <= y <= H//2+1):
				# we do not consider that wall
				pass
			else:
				self._setWall(x, y, direction)                              # no need to check if the wall already exists
				self._setWall(x + Ddx[direction], y + Ddy[direction], (direction+2) % 4)    # wall in the adjacent box
				self._walls.append((x, y, x + Ddx[direction], y + Ddy[direction]))

		# put walls around the arena to bound it
		for x in range(L):
			self._setWall(x, 0, NORTH)
			self._setWall(x, -1, SOUTH)
		for y in range(H):
			self._setWall(0, y, WEST)
			self._setWall(-1, y, EAST)


	@property
	def walls(self):
		"""Returns the list of walls (x,y, x+dx, y+dy)"""
		return self._walls

	def _setWall(self, x, y, direction):
		"""Set a wall direction on x, y"""
		if -1 <= x < self._L and -1 <= y < self._H:
			self._array[x][y] |= 1 << direction

	def _removeWall(self, x, y, direction):
		"""Remove the wall at x, y for direction"""
		self._array[x][y] &= ~(1 << direction)

	def _removeWalls(self, x, y):
		"""Remove the walls at x,y"""
		self._array[x][y] &= 128+64

	def getWall(self, x, y, direction):
		"""Check if there is a wall at x,y
		Check at x,y, and if we are on the borders,
		check the adjacent boxes"""
		if self.isPosValid(x, y):       # Most of the time
			return bool(self._array[x][y] & (1 << direction))
		# other wise, it's a corner case (only to draw the bounds of the arena)
		elif direction == NORTH and self.isPosValid(x, y-1):
			return bool(self._array[x][y-1] & (1 << SOUTH))
		elif direction == EAST and self.isPosValid(x+1, y):
			return bool(self._array[x+1][y] & (1 << WEST))
		elif direction == SOUTH and self.isPosValid(x, y+1):
			return bool(self._array[x][y+1] & (1 << NORTH))
		elif direction == WEST and self.isPosValid(x-1, y):
			return bool(self._array[x-1][y] & (1 << EAST))
		else:
			return False

	def setNoone(self, x, y):
		"""Remove the player at x,y"""
		self._array[x][y] &= 63

	def setPlayer(self, x, y, nPlayer):
		"""Set the player nPlayer at x,y"""
		self._array[x][y] |= 1 << (6+nPlayer)

	def getPlayer(self, x, y):
		"""Get the player at x,y
		Returns None if there is no one, or the number of the player (0 or 1)"""
		return {0: None, 1: 0, 2: 1}[self._array[x][y] >> 6]

	def isPosValid(self, x, y):
		"""Returns True if the position x,y is valid"""
		return 0 <= x < self._L and 0 <= y < self._H

	def strBox(self, x, y):
		"""Returns three characters to print the Box x,y
		1 2
		3 x
		The characters are taken into the UTF8 Unicode Box Drawing list
		"""
		# prepare 2 and 3
		c2 = HORIZONTAL_BOX if self.getWall(x, y, NORTH) else ' '
		c3 = VERTICAL_BOX if self.getWall(x, y, WEST) else ' '
		# prepare 1 from the 4 neighbours
		nesw = (
			self.getWall(x, y - 1, WEST),   # North
			self.getWall(x, y, NORTH),          # East
			self.getWall(x, y, WEST),           # South
			self.getWall(x - 1, y, NORTH))    # West
		c1 = DRAWING_BOX[nesw]

		return c1, c2, c3

	def getBorderHTML(self, x, y):
		"""Retuns the borders to apply to an HTML cell (for the walls)"""
		return ('2px' if self._array[x][y]&1 else '0px',	'2px' if self._array[x][y] & 2 else '0px',
			   '2px' if self._array[x][y] & 4 else '0px', '2px' if self._array[x][y] & 8 else '0px')


class Snake(Game):
	"""
	class Snake

	Inherits from Game
	- _players: tuple of the two players
	- _logger: logger to use to log infos, debug, ...
	- _name: name of the game
	- _whoPlays: number of the player who should play now (0 or 1)
	- _waitingPlayer: Event used to wait for the players
	- _lastMove, _last_return_code: string and returning code corresponding to the last move

	And some properties
	- _arena: (Arena) array representing the game
	- _L,_H: length and height of the area
	- playerPos: list of the coordinates (itself a list) of the two players
	"""

	# dictionary of the possible training Players (name-> class)
	type_dict = {"RANDOM_PLAYER": RandomPlayer, "SUPER_PLAYER": SuperPlayer}

	def __init__(self, player1, player2, **options):
		"""
		Create a game
		:param player1: 1st Player
		:param player2: 2nd Player
		:param options: dictionary of options (the options 'seed' and 'timeout' are managed by the Game class)
		"""

		# get a seed if the seed is not given; seed the random numbers generator
		if 'seed' in options:
			seed(int(options['seed']))

		# random arena
		totalSize = randint(40, 60)  # sX + sY is randomly in [30,60]
		self.L = randint(20, 40)
		self.H = max(totalSize - self.L, 5)     # at least 5
		self.L, self.H = max(self.L, self.H), min(self.L, self.H)   # L is greater than H
		self.arena = Arena(self.L, self.H, int(options.get("difficulty", DEFAULT_DIFFICULTY)))

		# players positions (list of positions+direction, first is the head)
		# : List[List[Tuple[int, int, Union[None, int]]]]
		self.playerPos = [[(2, self.H // 2, None)], [(self.L - 3, self.H // 2, None)]]

		self.arena.setPlayer(2, self.H // 2, 0)
		self.arena.setPlayer(self.L - 3, self.H // 2, 1)

		# counter (one per player), used to know when the snake grows
		self.counter = [0, 0]

		# call the superclass constructor (only at the end, because the superclass constructor launches
		# the players and they will immediately requires some Labyrinth's properties)
		super().__init__(player1, player2, **options)


	def HTMLrepr(self):
		"""Returns an HTML representation of your game"""
		# this, or something you want...
		return "<A href='/game/%s'>%s</A>" % (self.name, self.name)

	def getDictInformations(self, firstTime=False):
		"""
	    Returns a dictionary for HTML display
		"""
		d = {}
		# add the arena (the <table> with walls), for the 1st time
		if firstTime:
			lines = []
			for y in range(self.H):
				L = []
				for x in range(self.L):
					L.append('<td class="arena" id="t_%d_%d" style="border-width: %s %s %s %s"></td>' % ((x,y)+self.arena.getBorderHTML(x, y)))
				lines.append("<tr>" + "".join(L) + "</tr>")

			d['arena'] = "<table style='border-collapse:collapse;'>\n"+"\n".join(lines)+"\n</table>\n"
			d['names'] = [p.name for p in self.players]
		# add the snakes
		# d['pl'][player] is a list of (x,y,d) or (x,y,d1,d2) where d, d1 and d2 are the direction used for the sprite
		# (see the JS arrays head, tail and body generated by genJS.py; d or (d1,d2) are the indexes in this array)
		d['pl'] = []
		for p in range(2):
			ll = []
			for i in range(len(self.playerPos[p])):
				# head
				if i == 0:
					if len(self.playerPos[p]) > 1:
						ll.append((self.playerPos[p][i][0], self.playerPos[p][i][1], self.playerPos[p][i + 1][2]))
					else:
						ll.append((self.playerPos[p][i][0], self.playerPos[p][i][1], 0))
				# tail
				elif i == len(self.playerPos[p])-1:
					ll.append((self.playerPos[p][i][0], self.playerPos[p][i][1], (self.playerPos[p][i][2]+2)%4))
				# body
				else:
					mini, maxi = sorted([self.playerPos[p][i][2], (self.playerPos[p][i + 1][2]+2)%4])
					ll.append((self.playerPos[p][i][0], self.playerPos[p][i][1], mini, maxi))
			d['pl'].append(ll)
		# counters
		d['counters'] = self.counter
		d['comments'] = self._comments.getString(2, [p.name for p in self._players], html=True)
		return d

	def __str__(self):
		"""
		Convert a Game into string (to be send to clients, and display)
		"""
		# iter over each element of the array
		lines = []
		for y in range(self.H):
			# for each line of the array, build two lines (one for the wall on the north, one for the element)
			line1 = []
			line2 = []
			for x in range(self.L):
				# get the characters  c1 c2
				#                     c3 strPl
				c1, c2, c3 = self.arena.strBox(x, y)
				# build StrPl (character of the x,y box)
				pl = self.arena.getPlayer(x, y)
				if pl is not None:
					# find where it is in the list
					indexSnake = 0
					for indexSnake, (px, py, _) in enumerate(self.playerPos[pl]):
						if (px, py) == (x, y):
							break
					# define strPl (is it the head or not)
					if indexSnake == 0 and len(self.playerPos[pl]) > 1:
						b = TRIANGLES[self.playerPos[pl][1][2]]
					else:
						b = BOX
					strPl = (Fore.GREEN if pl else Fore.RED) + b + Fore.RESET
					# modify c2 or c3 if there is a connection with another block of Snake
					neighbours = [self.playerPos[pl][indexSnake+i] for i in (-1,+1) if 0 <= indexSnake+i < len(self.playerPos[pl])]
					for nx, ny, _ in neighbours:
						if nx+1 == x:
							c3 = (Fore.GREEN if pl else Fore.RED) + Style.DIM + BOX + Fore.RESET + Style.NORMAL
						elif ny+1 == y:
							c2 = (Fore.GREEN if pl else Fore.RED) + Style.DIM + BOX + Fore.RESET + Style.NORMAL
				else:
					strPl = '.'
				# append the 1st line (with NORTH wall) and 2nd line (with WEST wall)
				line1.append(c1+c2)
				line2.append(c3 + strPl)
			# add end of the line (EAST walls)
			c1, c2, c3 = self.arena.strBox(self.L, y)
			line1.append(c1)
			line2.append(c3)
			# add names
			if y == self.H//2 - 3:
				line1.append("\t  Game: " + self.name)
			if y == self.H//2 - 1:
				line1.append("\t" + ('> ' if self._whoPlays == 0 else '  ') + Fore.RED + "Player 0: " + Fore.RESET +
				             self.players[0].name + " (%d)" % self.counter[0])
			if y == self.H//2 + 1:
				line1.append("\t" + ('> ' if self._whoPlays == 1 else '  ') + Fore.GREEN + "Player 1: " + Fore.RESET +
				             self.players[1].name + " (%d)" % self.counter[1])
			# add the lines
			lines.append("".join(line1))
			lines.append("".join(line2))

		# end the last walls in the SOUTH
		line1 = []
		for x in range(self.L+1):
			c1, c2, c3 = self.arena.strBox(x, self.H)
			line1.append(c1+c2)
		lines.append("".join(line1))

		# TODO: add BOX on the wall if the snakes is there

		return "\n".join(lines) + "\n\n"



	def updateGame(self, move):
		"""
		update the game by playing a move
		- move: a string
		Return a tuple (move_code, msg), where
		- move_code: (integer) 0 if the game continues after this move, >0 if it's a winning move, -1 otherwise (illegal move)
		- msg: a message to send to the player, explaining why the game is ending
		"""
		result = regd.match(move)
		# check if the data receive is valid
		if result is None:
			return LOSING_MOVE, "The move is not in correct form ('%d') !"

		# get the type and the value
		direction = int(result.group(1))
		# check the possible values
		if not (NORTH <= direction <= WEST):
			return LOSING_MOVE, "The direction is not valid (should be between 0 and 3)!"

		# move the player
		pl = self._whoPlays
		# head position and new position
		hx, hy, d = self.playerPos[pl][0]
		nx = hx + Ddx[direction]
		ny = hy + Ddy[direction]
		# check if there is a wall and if the new position is free
		if self.arena.getWall(hx, hy, direction):
			return LOSING_MOVE, "The move makes the snake goes into a wall"
		if self.arena.getPlayer(nx, ny) is not None:
			return LOSING_MOVE, "The move makes the snakes collide..."
		# the snake move
		self.arena.setPlayer(nx, ny, pl)
		self.playerPos[pl][0] = (hx, hy, direction)
		self.playerPos[pl].insert(0, (nx, ny, None))
		# and it may grow or not
		if self.counter[pl] != 0:
			qx, qy, _ = self.playerPos[pl].pop()
			self.arena.setNoone(qx, qy)
		# increase the counter
		self.counter[pl] = (self.counter[pl] + 1) % 10

		logger.debug("self._playerPos=%s", self.playerPos)

		return NORMAL_MOVE, ""


	def getDataSize(self):
		"""
		Returns the size of the datas send by getData
		(for example sizes of arrays, so that the arrays could be allocated before calling getData)
		"""
		return "%d %d %d" % (self.L, self.H, len(self.arena.walls))



	def getData(self, player):
		"""
		Return the datas of the game (when ask with the GET_GAME_DATA message)
		ie the list of positions of walls
		"""
		return " ".join("%d %d %d %d" % wall for wall in self.arena.walls)
	

	def getCutename(self):
		"""
		Returns the cutename of the game (to display in html views)
		"""
		if hasattr(self, '_cutename'):
			return self._cutename
		else:
			return None
