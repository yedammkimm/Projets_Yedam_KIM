"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire
Licence: GPL

File: DoNothingPlayer.py
	Contains the class DoNothingPlayer
	-> defines a stupid player that does... nothing (it plays DO_NOTHING every time)

Copyright 2020 T. Hilaire
"""

from random import choice
from CGSserver.Player import TrainingPlayer
from .Constants import MULTICOLOR


class DoNothingPlayer(TrainingPlayer):
	"""
	This class defines a stupid training player that does... nothing
	(it get a card from the deck  or the face up card every time)
	It is used for the first part of the project (players need an opponent that does nothing to check their	code)
	"""

	def __init__(self, **_):
		"""Initialize the Training Player
		no options, nothing to do with options
		"""
		super().__init__('Do_nothing')


	def playMove(self):
		"""
		Plays the move -> here take a card
		(50% chance to take from the deck, 50% to take a random face up card)
		Returns the move
		"""
		# face up cards that are not a Locomotive
		faceUp = ["3 %d" % c for c in self.game.faceUpCards() if c != MULTICOLOR]
		return choice(["2", choice(faceUp)])

