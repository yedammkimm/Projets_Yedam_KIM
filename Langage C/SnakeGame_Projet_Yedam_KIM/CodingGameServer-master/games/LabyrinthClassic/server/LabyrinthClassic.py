"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire
Licence: GPL

File: LabyrinthClassic.py
	Contains the class LabyrinthClassic
	-> defines the Labyrinth game (its rules, moves, etc.)

Copyright 2021 T. Hilaire


TODO:
Reste à faire:
+ tester le déplacement (Lab.reachable marche, donc devrait être ok; tester le déplacement en bord de labyrinthe)
- tester la fin de partie (pris tous les items)
- coder le joueur random (insert une tuile random, se déplace sur l'item si il est joignable sinon se déplace au hasard là où il peut)
- coder un autre joueur (teste toutes les possibilités, regarde celles qui peuvent faire gagner et prend celle qui ne permet pas à l'adversaire d'avoir son item)

Coté C:
implémenter la partie de jeu (déplacement colone)
implémenter le A* (reachable)
implémenter la même IA que le dernier joueur Python


"""

from random import shuffle, random, randint
from re import compile
from ansi2html import Ansi2HTMLConverter
from colorama import Fore, Back
from logging import getLogger
from copy import deepcopy, copy

from CGSserver.Constants import NORMAL_MOVE, WINNING_MOVE, LOSING_MOVE
from CGSserver.Game import Game
from .Constants import INSERT_LINE_LEFT, INSERT_LINE_RIGHT, INSERT_COLUMN_TOP, INSERT_COLUMN_BOTTOM
from .Constants import MAX_ITEM, BACKPLAYER, ITEMCHAR, OPPOSITE
from .Basic import BasicPlayer
from .PlayRandomPlayer import PlayRandomPlayer
from .DontMove import PlayDontMove
from .Laby import Tile, Laby


logger = getLogger("Labyclassic")  # general logger ('root')

regdd = compile(r"(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)")  # regex to parse a "%d %d %d %d %d" string


class LabyrinthClassic(Game):
	"""
	Labyrinth game
	Use the classic rules (Ravensburger's game)
	Inherits from Game
	- _players: tuple of the two players
	- _logger: logger to use to log infos, debug, ...
	- _name: name of the game
	- _whoPlays: number of the player who should play now (0 or 1)
	- _waitingPlayer: Event used to wait for the players
	- _lastMove, _last_return_code: string and returning code corresponding to the last move

	Add some properties
	- _lab: array (list of lists of tiles) representing the labyrinth
	- _L,_H: length and height of the labyrinth
	- _playerPos: list of the coordinates of the two players (player #0 and player #1)
	- _playerItem: list of the number of the next item to found (player#0 and player #1)
	"""

	# dictionary of the possible training Players (non-regular players)
	type_dict = {"RANDOM": PlayRandomPlayer, "BASIC": BasicPlayer, "DONTMOVE": PlayDontMove}

	def __init__(self, player1, player2, **options):
		"""
		Create a labyrinth
		:param player1: 1st Player
		:param player2: 2nd Player
		:param options: dictionary of options (the options 'seed' and 'timeout' are managed by the Game class)
			"mini" in option set the size to minimum
		"""
		# set the seed
		self._seed = self._setseed(options)

		# random Labyrinth
		self._L = 2 * randint(3, 6) + 1
		self._H = 2 * randint(3, 6) + 1
		if 'mini' in options:
			self._L = self._H = 7
		self._lab = Laby(self._L, self._H)

		# history
		self._initLab = deepcopy(self._lab)
		self._history = []

		# list of coordinates
		self._playerPos = [(0, 0), (self._L-1, self._H-1)]

		# item
		self._playerItem = [1, MAX_ITEM]

		# last move
		self._lastInsert = 0, 0

		# get the margin in the options
		self._margin = options.get("margin", "").lower() in ['true', '1', 'yes']
		# get debug display in the options
		self._displayDebug = options.get("display", "").lower() == 'debug'

		# call the superclass constructor (only at the end, because the superclass constructor launches
		# the players, and they will immediately require some Labyrinth's properties)
		super().__init__(player1, player2, **options)


	@property
	def playerPos(self):
		"""Returns the positions of the players"""
		return self._playerPos

	@property
	def lab(self):
		"""Returns the lab"""
		return self._lab

	def HTMLrepr(self):
		"""Returns an HTML representation of a Labyrinth"""
		return "<A href='/game/%s'>%s</A>" % (self.name, self.name)


	def getDictInformations(self, firstTime=False):
		"""
		Returns a dictionary for HTML display
		:return:
		"""

		d = {}
		# add the lab for the 1st call
		if firstTime:
			d.update(self._lab.toJSON())
			d['history'] = self._history[:-1]
			d['names'] = [p.name for p in self.players]

		# add the last move and the inserted tile
		if self._history:
			d['lastInsert'] = self._history[-1]

		return d

	def __str__(self):
		"""
		Convert a Game into string (to be sent to clients, and display)
		"""
		xmargin = "  " if self._margin else ""
		lines = ["   " + (" ▽ " + xmargin + " ▼ " + xmargin) * (self.L//2) + " ▽"]
		for y in range(self.H):
			top, mid, bot = ["   "], [" ▶ " if y % 2 else " ▷ "], ["   "]
			for x in range(self.L):
				c = BACKPLAYER[(x, y) == self._playerPos[0], (x, y) == self._playerPos[1]] \
					+ ITEMCHAR[self.lab[x, y].item == self._playerItem[0], self.lab[x, y].item == self._playerItem[1]] \
				    + ("⚑" if self.lab[x, y].item > 0 else "·") \
					+ Fore.RESET + Back.RESET
				t, m, b = self._lab.toStr(x, y, c)
				top.append(t + xmargin)
				mid.append(m + xmargin)
				bot.append(b + xmargin)
			lines.append("".join(top))
			lines.append("".join(mid))
			lines.append("".join(bot))

			if self._margin:
				lines.append("")

		# add player names
		# index of lines where player display is add
		iline = [(self.H//4)*4 + 1, (self.H//4)*4 + 5]
		colors = [Fore.LIGHTRED_EX, Fore.LIGHTBLUE_EX]
		for i, pl in enumerate(self._players):
			br = "[]" if self._whoPlays == i else "  "
			lines[iline[i]] += "\t\t" + br[0] + colors[i] + "Player " + str(i + 1) + ": " + Fore.RESET + pl.name + br[1]
			lines[iline[i] + 1] += "\t\t " + "Next item:" + str(self._playerItem[i])
		extra_item = ITEMCHAR[self._lab.extraTile.item == self._playerItem[0], self._lab.extraTile.item == self._playerItem[1]] \
				    + ("⚑" if self._lab.extraTile.item > 0 else "·") \
					+ Fore.RESET + Back.RESET
		extra_top, extra_mid, extra_bot = self._lab.toStr(-1, 0, extra_item)
		lines[(self.H//4)*4 + 9] += "\t\t             " + extra_top
		lines[(self.H//4)*4 + 10] += "\t\t Extra tile: " + extra_mid
		lines[(self.H//4)*4 + 11] += "\t\t             " + extra_bot

		# debug mode
		if self._displayDebug:
			lines.append("\n\n\n")
			for y in range(self.H):
				line = " ".join(["%d%d%d%d%d" % (self.lab[x, y].north, self.lab[x, y].east, self.lab[x, y].south, self.lab[x, y].west, self.lab[x, y].item) for x in range(self.L)])
				lines.append(line)

		return "\n".join(lines)

	@property
	def L(self):
		"""Returns the Length of the labyrinth"""
		return self._L

	@property
	def H(self):
		"""Returns the Height of the labyrinth"""
		return self._H

	@property
	def lastInsert(self):
		"""return the last move (ie insert, number)"""
		return self._lastInsert

	def updateGame(self, move):
		"""
		update the game by playing a move
		- move: a string "%d %d %d %d %d"
		Return a tuple (move_code, msg), where
		- move_code: (integer) 0 if the game continues after this move, >0 if it's a winning move, -1 otherwise (illegal move)
		- msg: a message to send to the player, with the new extra tile (N, E, S, W, item) and the new item chased by the player
		"""
		# parse the move
		result = regdd.match(move)
		# check if the data receive is valid
		if result is None:
			return LOSING_MOVE, "The move is not in correct form ('%d %d %d %d %d') !"
		# get the type and the value
		insert = int(result.group(1))
		number = int(result.group(2))
		rotation = int(result.group(3))
		x = int(result.group(4))
		y = int(result.group(5))
		# check the possible values
		if not (INSERT_LINE_LEFT <= insert <= INSERT_COLUMN_BOTTOM):
			return LOSING_MOVE, "The insertion is not valid !"
		if (INSERT_LINE_LEFT <= insert <= INSERT_LINE_RIGHT) and not (0 <= number < self.H):
			return LOSING_MOVE, "The line number is not valid !"
		if (INSERT_COLUMN_TOP <= insert <= INSERT_COLUMN_BOTTOM) and not (0 <= number < self.L):
			return LOSING_MOVE, "The column number is not valid !"
		if (number % 2) == 0:
			return LOSING_MOVE, "The column/line number must be odd !"
		if not (0 <= rotation <= 3):
			return LOSING_MOVE, "The rotation is not valid !"
		if not (0 <= x < self.L):
			return LOSING_MOVE, "The position x is not valid !"
		if not (0 <= y < self.H):
			return LOSING_MOVE, "The position y is not valid !"
		if self._lastInsert == (OPPOSITE[insert], number):
			return LOSING_MOVE, "The extra tile cannot be pushed back at the same place as previous move !"
		# rotate the line/column and insert the rotated extra tile
		self._lab.extraTile.rotate(rotation)
		inserted = copy(self._lab.extraTile)
		self._lab.insertExtraTile(insert, number, self._playerPos)

		# move the player
		self._lab.reachable(*self._playerPos[self._whoPlays])
		if not self._lab[x, y].reachable:
			return LOSING_MOVE, "The position %d,%d is not reachable !" % (x, y)
		else:
			self._playerPos[self._whoPlays] = x, y

		# check if the item is found
		if self._lab[x, y].item == self._playerItem[self._whoPlays]:
			# found !
			self._playerItem[self._whoPlays] += -1 if self._whoPlays else +1
			self.sendComment(self.playerWhoPlays, "I've found a new item!")
			# is it the last one ?
			if self._playerItem[self._whoPlays] == (0 if self._whoPlays else MAX_ITEM+1):
				return WINNING_MOVE, "The last item has been reached!"
			else:
				self.sendComment(self.playerWhoPlays, "My next item is #%d" % self._playerItem[self._whoPlays])

		# then return the new extra tile and numbering of the next item of the player
		self._lastInsert = insert, number
		self._history.append({
			'insert': insert,
			'number': number,
			'inserted': inserted.toType(),
			'rotation' : rotation,
			'playerPos': self._playerPos[:],
			'itemPos': self._playerItem[:],
		})
		return NORMAL_MOVE, "%d %d %d %d %d %d" % (self._lab.extraTile.north, self._lab.extraTile.east, self._lab.extraTile.south, self._lab.extraTile.west, self._lab.extraTile.item, self._playerItem[self._whoPlays])


	def getData(self, player):
		"""
		Return the datas of the labyrinth (when ask with the GET_GAME_DATA message)
		"""
		ZeroOne = {False: "0", True: "1"}
		msg = []
		for y in range(self.H):
			for x in range(self.L):
				t = self._lab[x, y]
				msg.extend([ZeroOne[t.north], ZeroOne[t.east], ZeroOne[t.south], ZeroOne[t.west], str(t.item)])
		msg.extend([ZeroOne[self._lab.extraTile.north], ZeroOne[self._lab.extraTile.east],
					ZeroOne[self._lab.extraTile.south], ZeroOne[self._lab.extraTile.west],
					str(self._lab.extraTile.item)])
		return " ".join(msg)


	def getDataSize(self):
		"""
		Returns the size of the next incoming data
		Here, the size of the labyrinth
		"""
		return "%d %d" % (self.L, self.H)

