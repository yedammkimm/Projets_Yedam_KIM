"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, J. Brajard
Licence: GPL

File: DontMove.py
	Contains the class playDontMove
	-> defines a dummy player that do not move (and change randomly the tiles)

Copyright 2016-2017 T. Hilaire, J. Brajard
"""

from CGSserver.Player import TrainingPlayer
from random import choice, randint
from .Constants import INSERT_COLUMN_BOTTOM, INSERT_COLUMN_TOP, INSERT_LINE_LEFT, INSERT_LINE_RIGHT, OPPOSITE


class PlayDontMove(TrainingPlayer):
	"""
	This class implements a training player that do not move
	Every player should be able to beat him
	"""
	def __init__(self, **options):
		"""
		Initialize the training player
		"""
		super().__init__('Dont_Move')


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
		# same position
		x, y = self.game.playerPos[us]
		return "%d %d %d %d %d" % (insert, number, rotate, x, y)


