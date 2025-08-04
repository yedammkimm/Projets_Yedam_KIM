"""
* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, J. Brajard
Licence: GPL

File: Game.py
	Contains the class Game
	-> defines the generic Game's behavior (this class will be inherits for each game, like the Labyrinth)

Copyright 2016-2017 T. Hilaire, J. Brajard
"""


import time as timemod
from datetime import datetime
from random import seed as set_seed, randint, choice
from threading import Barrier, BrokenBarrierError
from time import time
from ansi2html import Ansi2HTMLConverter
from CGSserver.Comments import CommentQueue
from CGSserver.Constants import NORMAL_MOVE, WINNING_MOVE, LOSING_MOVE, TIMEOUT_TURN, MAX_COMMENTS
from CGSserver.BaseClass import BaseClass


def crc24(octets):
	"""
	Compute a CRC 24 bits hash
	Credits Karl Knechtel
	http://stackoverflow.com/questions/4544154/crc24-from-c-to-python
	"""
	INIT = 0xB704CE
	POLY = 0x1864CFB
	crc = INIT
	for octet in octets:
		crc ^= (octet << 16)
		for i in range(8):
			crc <<= 1
			if crc & 0x1000000:
				crc ^= POLY
	return crc & 0xFFFFFF


def hex6(x):
	"""
	Returns (a string) the hexadecimal of x (but with 6 digits, without the trailing 0x)
	"""
	h = "000000" + hex(x)[2:]       # add zeros before the hexadecimal (without 0x)
	return h[-6:]   # get the 6 last characters


def unpack23(x):
	"""
	Returns a 3-element tuple from a 2-element tuple (if needed, the last element is repeated)
	"""
	if len(x) == 2:
		return x[0], x[1], x[1]
	else:
		return x


class Game(BaseClass):
	"""
	Game class

	allInstances: (class property) dictionary of all the games

	An instance of class Game contains:
	- _players: tuple of the two players (player0 and player1)
	- _logger: logger to use to log infos, debug, ...
	- _name: name of the game
	- _whoPlays: number of the player who should play now (0 or 1)
	- _waitingPlayer: Event used to wait for the player
	- _lastMove: string corresponding to the last move
	- _lastMessage: message associated with the last move (to be sent to the opponent)
	- _tournament: (Tournament) the tournament the game is involved in (or None no tournament)

	"""

	allInstances = {}          # dictionary of all the instances
	_theGameClass = None

	type_dict = {}          # dictionary of the possible training Players (TO BE OVERLOADED BY INHERITED CLASSES)

	def __init__(self, player1, player2, tournament=None, **options):
		"""
		Create a Game
		Parameters:
		- player1, player2: two Player (the order will be changed according who begins)
		- options: dictionary of options
			- 'seed': seed of the labyrinth (same seed => same labyrinth); used as seed for the random generator
			- 'timeout': timeout of the game (if not given, the default timeout is used)
			- 'start': who starts the game (0, 1 or -1); random when not precised or '-1'
			- 'once': when presents, the player can only play once
			- 'delay': delay between two moves
		"""

		# check if we can create the game (are the players available)
		if player1 is None or player2 is None:
			raise ValueError("Players doesn't exist")
		if player1 is player2:
			raise ValueError("Cannot play against himself")
		if player1.game is not None or player2.game is not None:
			raise ValueError("Players already play in a game")

		# check if the player can play again
		for p in player1, player2:
			if p.isRegular:
				if p.hasAlreadyPlayed and 'once' in options:
					p.disconnect()
					raise ValueError("The player can only play once... STOP")
				p.hasAlreadyPlayed = True


		# get a seed if the seed is not given; seed the random numbers generator
		self._seed = self._setseed(options)

		# players
		# we randomly decide the order of the players
		if 'start' not in options:
			pl = choice((0, 1))
		else:
			try:
				pl = int(options['start'])
				if pl == -1:
					pl = choice((0, 1))
			except ValueError:
				raise ValueError("The 'start' option must be '0', '1' or '-1'")
		self._players = (player1, player2) if pl == 0 else (player2, player1)


		# (unique) name composed by
		# - the first 6 characters are the seed (in hexadecimal),
		# - the 6 next characters are hash (CRC24) of the time and names (hexadecimal)
		ok = False
		name = ""
		while not ok:   # we need a loop just in case we are unlucky and two existing games have the same hash
			fullName = str(int(time())) + player1.name + player2.name
			name = hex6(self._seed) + hex6(crc24(bytes(fullName, 'utf8')))
			ok = name not in self.allInstances
			if not ok:
				timemod.sleep(1)

		# store the tournament
		self._tournament = tournament

		# determine who starts (player #0 ALWAYS starts)
		self._whoPlays = 0

		# last move
		self._lastMove = ""
		self._lastMessage = ""
		self._lastBotMessage = ""
		self._lastReturn_code = 0

		# set a delay after each move (to let the time to see the party)
		if 'delay' not in options:
			self._delay = 0
		else:
			try:
				self._delay = float(options['delay'])
			except ValueError:
				self._delay = 0
				# raise ValueError("The 'delay' value is invalid ('delay=%s')" % options['delay'])

		# time out for the move
		if 'timeout' not in options:
			self._timeout = TIMEOUT_TURN
		else:
			try:
				self._timeout = int(options['timeout'])
			except ValueError:
				raise ValueError("The 'timeout' value is invalid ('timeout=%s')" % options['timeout'])
		# timestamp of the last move
		self._lastMoveTime = datetime.now()     # used for the timeout when one player is a non-regular player

		# Barrier used for the synchronization of the two players (during playMove and getMove)
		self._sync = Barrier(2, timeout=self._timeout)

		# list of comments
		self._comments = CommentQueue(MAX_COMMENTS)

		# and (almost) last, call the super init for base initialization
		super().__init__(name)

		# advertise the players that they enter in a game
		player1.game = self
		player2.game = self

		# log the game
		self.logger.info("=================================")
		if self._tournament:
			self.logger.message("[Tournament %s] Game %s just starts with '%s' and '%s' (seed=%d).",
			                    self._tournament.name, name, player1.name, player2.name, self._seed)
		else:
			self.logger.message(
				"Game %s just starts with '%s' and '%s' (seed=%d).", name, player1.name, player2.name, self._seed
			)
		self.logger.debug("The delay is set to %ds" % self._delay)
		self.logger.debug("The timeout is set to %ds" % self._timeout)


	@property
	def lastBotMessage(self):
		"""Return the answer of the last move of the bot
		used for a bot to get feedback/answer for his move"""
		return self._lastBotMessage


	def _setseed(self, options):
		"""get a seed if the seed is not given
		 set the seed for the random numbers generator"""
		# check if the seed has been already set (by a child, before calling the super init)
		if hasattr(self, '_seed'):
			return self._seed
		if 'seed' not in options or not options['seed']:
			set_seed(None)  # (from doc):  If seed is omitted or None, current system time is used
			seed = randint(0, 16777215)  # between 0 and 2^24-1
		else:
			try:
				seed = int(options['seed'], 0)
				if not 0 <= seed <= 16777215:
					raise ValueError(
						"The 'seed' value must be between 0 and 16777215 ('seed=%s'." % options['seed'])
			except ValueError:
				raise ValueError("The 'seed' value is invalid ('seed=%s')" % options['seed'])
		set_seed(seed)
		return seed


	def partialEndOfGame(self, whoLooses):
		"""
		manage a partial end of the game (player has deconnected or send wrong command)
		Parameters:
			- whoLooses: (RegularPlayer) player that looses
		The game is not fully ended, since we need to wait the other player to call GET_MOVE or PLAY_MOVE
		"""
		nWhoLooses = 0 if self._players[0] is whoLooses else 1
		# end of the game if the opponent is not regular or if it has already disconnected
		if not self._players[1 - nWhoLooses].isRegular:
			self.endOfGame(1 - nWhoLooses, "Opponent has disconnected")
		elif self._players[1 - nWhoLooses].game is None:
			# the opponent has disconnected first, so we win
			self.endOfGame(nWhoLooses, "Opponent has disconnected")
		else:
			whoLooses.game = None


	def manageNextTurn(self, return_code, msg):
		"""
		Called after having update the game
		Check if it's the end of the game (and call endOfGame), or update the next player
		Parameters:
		- return_code: (int) code returned by updateGame (NORMAL_MOVE, WINNING_MOVE or LOOSING_MOVE)
		- msg: (string) message when the move is not OK
		"""
		# check if the player wins
		if return_code == NORMAL_MOVE:
			# Wait for the delay time
			timemod.sleep(self._delay)
			# change who plays
			self._whoPlays = self.getNextPlayer()
		elif return_code == WINNING_MOVE:
			# Game won by the opponent, end of the game
			self.endOfGame(self._whoPlays, msg)
		else:  # return_code == LOOSING_MOVE
			# Game won by the regular player, end of the game
			self.endOfGame(1 - self._whoPlays, msg)


	def endOfGame(self, whoWins, msg):
		"""
		Manage the end of the game:
		The game is removed from the allGames dictionary, the players do not play to that game anymore
		Is called when the game is over (after a wining/losing move)
		Parameters:
			- whoWins: (int) number of the player who wins the game
			- msg: (sting) message explaining why it's the end of the game
		"""
		# log it
		self.logger.message("The game '%s' is now finished, %s won against %s (%s)",
		                    self.name, self._players[whoWins].name, self._players[1-whoWins].name, msg)
		self._players[whoWins].logger.info("We won the game (%s) !" % msg)
		self._players[1 - whoWins].logger.info("We loose the game (%s) !" % msg)

		# the players do not play anymore
		if self._players[0].game is not None:
			self._players[0].game = None
		if self._players[1].game is not None:
			self._players[1].game = None

		# tell the tournament the result of the game
		if self._tournament:
			self._tournament.endOfGame(self._players[whoWins], self._players[1 - whoWins])

		# tell the websockets the game has ended
		if self.socketio:
			conv = Ansi2HTMLConverter()
			msg = "<span class='result'>%s won !!</span><br/>%s" % (self._players[whoWins].name, conv.convert(msg))
			# send to all the websockets or only to one
			self.socketio.emit('endOfGame', msg, room=self.__class__.__name__ + '/' + self.name)

		# remove from the list of Games
		Game.removeInstance(self.name)


	@property
	def playerWhoPlays(self):
		"""
		Returns the player who Plays
		"""
		return self._players[self._whoPlays]


	@property
	def players(self):
		"""
		Returns the players
		"""
		return self._players


	def getLastMove(self):
		"""
		Wait for the move of the player playerWhoPlays (and sync with it)
		If it doesn't answer in TIMEOUT_TURN seconds, then he losts the game
		Returns:
			- last move: (string) string describing the opponent last move (exactly the string it sends)
			- msg: (string) extra data (may be empty) or message explaining why the move is WINING/LOSING
			- last return_code: (int) code (NORMAL_MOVE, WINNING_MOVE or LOSING_MOVE) describing the last move
		"""

		# check if the opponent doesn't have disconnected
		if self._players[self._whoPlays].game is None:
			self.endOfGame(1-self._whoPlays, "Opponent has disconnected")
			return "", "Opponent has disconnected", LOSING_MOVE

		# wait for the move of the opponent if the opponent is a regular player
		if self._players[self._whoPlays].isRegular:
			try:
				# 1st synchronization with the opponent (with playMove)
				self._sync.wait()
				# now the opponent has played, we wait now for self._whoPlays to be updated
				self._sync.wait()
				return self._lastMove, self._lastMessage, self._lastReturn_code
			except BrokenBarrierError:
				# Timeout !!
				# the opponent has lost the game
				self.endOfGame(1 - self._whoPlays, "Timeout")
				return self._lastMove, "Timeout", LOSING_MOVE

		else:
			# the opponent is a training player
			# so we call its playMove method
			move = self._players[self._whoPlays].playMove()
			self.logger.info("'%s' plays %s" % (self._players[self._whoPlays].name, move))
			self._players[1 - self._whoPlays].logger.info("%s plays %s" % (self._players[self._whoPlays].name, move))

			# and update the game
			return_code, msg, msgOpponent = unpack23(self.updateGame(move))
			self._lastBotMessage = msg
			# update who plays next and check for the end of the game
			self.manageNextTurn(return_code, msg)

			self.sendUpdateToWebSocket()

			return move, msgOpponent, return_code



	def playMove(self, move):
		"""
		Play a move we just received (from PlayerSocket)
		Do all the synchronization stuff (between the two players)
		The move is really played in the method updateGame (that tells if the move is legal or not)

		Parameters:
		- move: a string corresponding to the move
		Returns a tuple (move_code, msg), where
		- move_code: (integer) 0 if the game continues after this move, >0 if it's a winning move, -1 otherwise (illegal move)
		- msg: a message to send back to the player, with extra data or a message explaining why the game is ending
		"""
		# check if the opponent doesn't have disconnected
		if self._players[self._whoPlays].game is None:
			self.endOfGame(self._whoPlays, "Opponent has disconnected")
			return WINNING_MOVE, "Opponent has disconnected"

		# log that move
		self.logger.info("'%s' plays %s" % (self.players[self._whoPlays].name, move))
		if self._players[self._whoPlays].isRegular:
			self._players[self._whoPlays].logger.info("I play %s" % move)
		if self._players[1 - self._whoPlays].isRegular:
			self._players[1 - self._whoPlays].logger.info("%s plays %s" % (self.players[self._whoPlays].name, move))


		# if the opponent is a regular player
		if self._players[1 - self._whoPlays].isRegular:
			# play that move, update the game and keep the last move
			return_code, msg, msgOpponent = unpack23(self.updateGame(move))
			self._lastMove = move
			self._lastMessage = msgOpponent
			self._lastReturn_code = return_code

			try:
				# 1st synchronization with the opponent
				self._sync.wait()
			except BrokenBarrierError:
				# Timeout !
				self.endOfGame(self._whoPlays, "Timeout")
				return WINNING_MOVE, "Timeout of the opponent!"

			# update who plays next and check for the end of the game
			self.manageNextTurn(return_code, msg)
			self.sendUpdateToWebSocket()

			# 2nd synchronization with the opponent
			self._sync.wait()


		else:   # when the opponent is a training player
			# check for timeout
			if (datetime.now()-self._lastMoveTime).total_seconds() > self._timeout:
				# Timeout !!
				# the player has lost the game
				self.endOfGame(1 - self._whoPlays, "Timeout")
				return LOSING_MOVE, "Timeout !"

			# play that move, update the game and keep the last move
			return_code, msg, msgOpponent = unpack23(self.updateGame(move))
			self._lastMove = move
			self._lastMessage = msgOpponent
			self._lastReturn_code = return_code

			#  we store the time (to compute the timeout)
			self._lastMoveTime = datetime.now()

			# update who plays next and check for the end of the game
			self.manageNextTurn(return_code, msg)

			self.sendUpdateToWebSocket()
		return return_code, msg


	def sendComment(self, player, comment):
		"""
			Called when a player send a comment
		Parameters:
		- player: player who sends the comment
		- comment: (string) comment
		"""
		# log it
		self.logger.info("[%s] : '%s'", player.name, comment)
		for n in (0, 1):
			self.players[n].logger.info("[%s] : %s" % (player.name, comment))

		# append comment
		nPlayer = 0 if player is self._players[0] else 1
		self._comments.append(comment, nPlayer)




	def display(self, player):
		"""
		Parameters:
			- player: player who ask for display

		Returns a string version of the game, composed of:
		- the game information (from __str__ of the inherited class)
		- the comments
		"""
		nPlayer = 0 if player is self._players[0] else 1
		names = [p.name for p in self._players]

		test_str = str(self) + "\n" + self._comments.getString(nPlayer, names) + "\n"*4
		self.logger.debug(test_str)		
		return test_str



	@classmethod
	def gameFactory(cls, name, player1, options):
		"""
		Create a game with a particular player
		each child class fills its own type_dict (dictionary of the possible non-regular Players)

		1) it creates the training player (according to the type name)
		2) it creates the game (calling the constructor)

		Parameters:
		- name: (string) type of the training player ("DO_NOTHING": play against do_nothing player, etc...)
		- player1: player who plays the game
		- options: (dict) some options given by the player

		"""
		if name in cls.type_dict:
			p = cls.type_dict[name](**options)    # may raise ValueError exception if the options are invalid
			return cls(player1, p, **options)     # may raise ValueError exception if the options are invalid
		else:
			raise ValueError("The training player name '%s' is not valid." % name)




	@classmethod
	def setTheGameClass(cls, theGameClass):
		"""
			Setter for the Game we are playing (in order to let that attribute be available for everyone)
			Set when the game is known and imported
		"""
		cls._theGameClass = theGameClass


	@classmethod
	def getTheGameClass(cls):
		"""
		Getter for the Game we are playing (in order to let that class be available for everyone)
		Returns the class of the game we are playing (used to create those games)
		"""
		return cls._theGameClass


	@classmethod
	def getTheGameName(cls):
		"""
		Getter for the name of the Game we are playing
		"""
		return cls._theGameClass.__name__


	def getNextPlayer(self):
		"""
		Change the player who plays

		IT CAN BE OVERLOADED BY THE CHILD CLASS if the behaviour is different than a tour by tour game
		(if a player can replay)

		Returns the next player (but do not update self._whoPlays)
		"""
		return 1 - self._whoPlays


	def HTMLrepr(self):
		"""
		Returns HTML representation of the game

		IT CAN BE OVERLOADED BY THE CHILD CLASS
		"""
		return "<B><A href='/game/%s'>%s</A></B> (%s vs %s)" % \
		       (self.name, self.name, self.players[0].name, self.players[1].name)




	def updateGame(self, move):
		"""
		update the Game by playing the move
		TO BE OVERLOADED BY THE CHILD CLASS

		Play a move and update the game
		- move: a string
		Return a tuple (move_code, msg) OR (move_code, msg, msgOppenent), where
		- move_code: (integer) 0 if the game continues after this move, >0 if it's a winning move, -1 otherwise (illegal move)
		- msg: a message to send to the players, explaining why the game is ending, it may contain data
		- msgOpponnent: (OPTIONAL) a message sent to the opponent, IF the opponent should not receive the same data
		"""
		# play that move
		return 0, ''


	def getData(self, player):
		"""
		Return the datas of the game (when ask with the GET_GAME_DATA message)

		TO BE OVERLOADED BY THE CHILD CLASS

		"""
		return ""


	def getDataSize(self):
		"""
		Returns the size of the next incoming data (for example sizes of arrays)

		TO BE OVERLOADED BY THE CHILD CLASS

		"""
		return ""



	def getCutename(self):
		"""
		Returns the cutename of the game (to display in html views)

		TO BE OVERLOADED BY THE CHILD CLASS

		"""
		if hasattr(self, '_cutename'):
			return self._cutename
		else:
			return None


