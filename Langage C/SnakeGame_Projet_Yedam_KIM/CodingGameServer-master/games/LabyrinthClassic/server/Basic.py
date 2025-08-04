"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire
Licence: GPL

File: Basic.py
	Contains the class Basic
	-> defines a player that look at every possible move and keep the best one (1st level depth)

Copyright 2016-2017 T. Hilaire, J. Brajard
"""

from CGSserver.Player import TrainingPlayer
from itertools import product
from random import shuffle
from copy import deepcopy
from operator import itemgetter
from math import sqrt
from CGSserver.Player import TrainingPlayer
from random import choice, randint
from .Constants import INSERT_COLUMN_BOTTOM, INSERT_COLUMN_TOP, INSERT_LINE_LEFT, INSERT_LINE_RIGHT, OPPOSITE




class BasicPlayer(TrainingPlayer):
	"""
	class BasicPlayer that create a Basic player trainer
	"""

	def __init__(self, **_):
		super().__init__('Basic')


	def playMove(self):
		"""
		Plays the move -> here a random move
		Returns the move (string %d %d %d %d %d)
		"""
		# get our player number
		us = 0 if (self.game.players[0] is self) else 1

		# generate the list of possible (numbers, insert)
		ni = list(product(range(1, self.game.L-1, 2), [INSERT_COLUMN_BOTTOM, INSERT_COLUMN_TOP])) + \
			  list(product(range(1, self.game.H-1, 2), [INSERT_LINE_LEFT, INSERT_LINE_RIGHT]))
		# remove the last move
		lastInsert, lastNumber = self.game.lastInsert
		try:
			ni.remove((lastNumber, OPPOSITE[lastInsert]))
		except ValueError:
			pass
		# add all the possible rotations, and shuffle
		nir = list(product(ni, range(0, 4)))
		shuffle(nir)

		# iter over all the possinilities
		for (number, insert), rotate in nir:
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
				self.game.sendComment(self, "Yet another item !")
				return "%d %d %d %d %d" % (insert, number, rotate, xitem, yitem)


		# if not, move to the reachable tile that is the closest to the item to reach
		pos = [(x, y) for x in range(self.game.L) for y in range(self.game.H) if lab[x, y].reachable]  # list of reachable points
		dist = list(map(lambda p: sqrt((p[0] - xitem) ** 2 + (p[1] - yitem) ** 2), pos))  # list of distance
		x, y = pos[min(enumerate(dist), key=itemgetter(1))[0]]		# find the index of the minimum distance

		return "%d %d %d %d %d" % (insert, number, rotate, x, y)


