"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, J. Brajard
Licence: GPL

File: playRandomPlayer.py
	Contains the class playRandomPlayer
	-> defines a dummy player that play randomly every time (but do not loose)

Copyright 2016-2017 T. Hilaire, J. Brajard
"""

from copy import deepcopy
from operator import itemgetter
from math import sqrt
from CGSserver.Player import TrainingPlayer
from random import choice, randint
from .Constants import INSERT_COLUMN_BOTTOM, INSERT_COLUMN_TOP, INSERT_LINE_LEFT, INSERT_LINE_RIGHT, OPPOSITE


class PlayRandomPlayer(TrainingPlayer):
	"""
	This class implements a training player that plays... randomly
	Every player should be able to beat him
	"""
	def __init__(self, **options):
		"""
		Initialize the training player
		"""
		super().__init__('Play_Random')


	def playMove(self):
		"""
		Plays the move -> here a random move
		Returns the move (string %d %d %d %d %d)
		"""
		# get our player number
		us = 0 if (self.game.players[0] is self) else 1
		# random insertion
		insert = choice((INSERT_COLUMN_BOTTOM, INSERT_COLUMN_TOP, INSERT_LINE_LEFT, INSERT_LINE_RIGHT))
		if insert == INSERT_COLUMN_TOP or insert == INSERT_COLUMN_BOTTOM:
			number = randint(1, self.game.L//2) * 2 - 1
		else:
			number = randint(1, self.game.H//2) * 2 - 1
		rotate = randint(0, 3)
		# check if same as the last move
		if self.game.lastInsert == (OPPOSITE[insert], number):
			insert = OPPOSITE[insert]

		# copy the labyrinth and play the move
		lab = deepcopy(self.game._lab)
		playerPos = self.game._playerPos.copy()
		lab.extraTile.rotate(rotate)
		lab.insertExtraTile(insert=insert, number=number, playerPos=playerPos)

		# search for the next item
		nextItem = self.game._playerItem[self.game._whoPlays]
		itemPos = [(x, y) for x in range(self.game.L) for y in range(self.game.H) if lab[x, y].item == nextItem]
		xitem, yitem = itemPos[0] if itemPos else playerPos[self.game._whoPlays]

		#try to reach the next item
		lab.reachable(*playerPos[self.game._whoPlays])
		if lab[xitem, yitem].reachable:
			x, y = xitem, yitem
		else:
			# if not, move to the reachable tile that is the closest to the item to reach
			pos = [(x, y) for x in range(self.game.L) for y in range(self.game.H) if lab[x, y].reachable]  # list of reachable points
			dist = list(map(lambda p: sqrt((p[0] - xitem) ** 2 + (p[1] - yitem) ** 2), pos))  # list of distance
			x, y = pos[min(enumerate(dist), key=itemgetter(1))[0]]		# find the index of the minimum distance

		return "%d %d %d %d %d" % (insert, number, rotate, x, y)


