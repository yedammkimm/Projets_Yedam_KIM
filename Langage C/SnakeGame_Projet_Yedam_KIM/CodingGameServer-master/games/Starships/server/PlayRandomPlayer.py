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

from CGSserver.Player import TrainingPlayer
from random import choice, randint
from .Constants import MOVE_DOWN, MOVE_UP, DO_NOTHING
from .Constants import Ddy

boolConv = {'true': True, 'false': False}


class PlayRandomPlayer(TrainingPlayer):
	"""
	This class implements a training player that plays... randomly
	Every player should be able to beat him
	"""
	def __init__(self, **options):
		"""
		Initialize the training player
		The option "rotate=true" (default) or "rotate=false" is possible
		This option indicates if the player can also rotate the lines/columns or not
		"""
		super().__init__('Play_Random')

		# check "rotate" option
		if "rotate" not in options:
			self.rotate = True
		elif options["rotate"].lower() in boolConv:
			self.rotate = boolConv[options["rotate"].lower()]
		else:
			raise ValueError("The option rotate=%s is incorrect." % options["rotate"])


	def playMove(self):
		"""
		Plays the move -> here a random move
		Returns the move (string %d %d)
		"""
		# get our player number
		us = 0 if (self.game.players[0] is self) else 1

		# build the list of the possible moves
		moves = []

		# move
		for move_type in (MOVE_UP, MOVE_DOWN):
			x, y = self.game.playerPos[us]
			y = (y + Ddy[move_type]) % self.game.H

			if self.game.lab[x][y]:
				moves.append("%d 0" % move_type)

		# choose one (up to 4 moves and 4 rotations)
		if moves:
			return choice(moves)
		else:
			# sometimes, we cannot move...
			self.game.sendComment(self, "I am blocked... I cannot move...")
			return "%d 0" % DO_NOTHING

