"""
* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, J. Brajard
Licence: GPL

File: PlayerSocket.py
	Contains the Socket Handler for the player
	-> implements the protocol client <-> server
	-> answers to each request of the client

Copyright 2016-2017 T. Hilaire, J. Brajard
"""

import logging
import shlex
from re import sub
from socketserver import BaseRequestHandler

from CGSserver.Constants import LOSING_MOVE
from CGSserver.Constants import SIZE_FMT
from CGSserver.Game import Game
from CGSserver.Player.RegularPlayer import RegularPlayer
from CGSserver.Tournament import Tournament

logger = logging.getLogger()  # general logger ('root')



class ProtocolError(Exception):
	"""Empty Exception class to manage protocol errors"""
	pass


class DisconnectionError(Exception):
	"""Empty Exception class to manage connection errors (like disconnected)"""
	pass



class PlayerSocketHandler(BaseRequestHandler):
	"""
	The request handler class for our server.
	It is instantiated once per connection to the server, ie one per player
	"""

	def __init__(self, request, client_address, server):
		"""
		Call the constructor of the based class, but add an attribute
		"""
		super().__init__(request, client_address, server)
		self._player = None


	def handle(self):
		"""
		Function that handle the connection with the client (player)
		All the connection protocol is managed here (see doc/Protocol.md)
		After identifying the player, there is an endless loop, following the protocol (waitForGame, sendDataGame, etc.)
		"""
		try:
			# get the name from the client and create the player
			self._player = None
			name = self.getPlayerName()
			try:
				self._player = RegularPlayer(name, self.client_address[0], self)
			except ValueError as err:
				raise ProtocolError(str(err))

			while True:
				# then, wait for a (new) game
				self.waitForGame()

				# and finally send the data for the game
				self.sendGameData()

				# repeat until we're still in the game
				while self.game is not None:
					data = self.receiveData()

					if data.startswith("GET_MOVE"):
						# get move of the opponent
						if self._player is not self.game.playerWhoPlays:
							self.sendData("OK")
							# get the last move
							move, msg, return_code = self.game.getLastMove()
							# send the move and the return code
							self.sendData(move)
							self.sendData(msg)
							self.sendData(str(return_code))
						else:
							# we cannot ask for a move, since it's our turn to play
							self.sendData("It's our turn to play, so we cannot ask for a move!")

					elif data.startswith("PLAY_MOVE "):
						# check if it's not too late (timeout)
						if self.game is None:   # the game is already finished due to TIMEOUT
							# Timeout !
							self.sendData("OK")
							self.sendData("Timeout !")
							self.sendData(str(LOSING_MOVE))
						# play move
						elif self._player is self.game.playerWhoPlays:
							self.sendData("OK")
							# play that move to see if it's a winning/losing/normal move
							return_code, msg = self.game.playMove(data[10:])
							# now, send the result of the move and the associated message
							self.sendData(msg)
							self.sendData(str(return_code))
						else:
							self.sendData("It's not our turn to play, so we cannot play a move!")

					elif data.startswith("DISP_GAME"):
						# returns a (long) string describing the labyrinth
						self.sendData("OK")
						# we do not use sendData here, because we do not want to log the full message...
						msg = self.game.display(self._player).encode()
						head = SIZE_FMT % len(msg)
						try:
							self.request.sendall(str(head).encode('utf-8'))
							self.request.sendall(msg)
						except BrokenPipeError:
							raise DisconnectionError()
						self.logger.low_debug("Send string to display to player %s (%s)", self._player.name, self.client_address[0])

					elif data.startswith("SEND_COMMENT "):
						# receive comment
						self.sendData("OK")
						self.game.sendComment(self._player, data[13:])

					else:
						raise ProtocolError("Bad protocol, command should not start with '" + data + "'")

		except ProtocolError as err:
			# log the protocol error
			if self._player is None:
				logger.info("Error with client (%s): '%s'", self.client_address[0], err)
			else:
				self.logger.info("Error with %s (%s): '%s'", self._player.name, self.client_address[0], err)
			# ends the game
			if self.game is not None:
				self.game.partialEndOfGame(self._player)
			# answers the client about the error
			try:
				self.sendData(str(err))
			except ConnectionError:
				pass
			except DisconnectionError:
				pass

		except DisconnectionError:
			# ends the game
			if self.game is not None:
				self.game.partialEndOfGame(self._player)


		except Exception as err:
			# log all the other errors
			self.logger.error(err, exc_info=True)



	def finish(self):
		"""
		Call when the connection is closed
		"""
		try:
			if self._player is not None:
				self.logger.info("Connection closed with player %s (%s)", self._player.name, self.client_address[0])
				self._player.unregisterTournament()
				RegularPlayer.removeInstance(self._player.name)
				self._player = None
			else:
				logger.info("Connection closed with client %s", self.client_address[0])

		except Exception as err:
			# log all the other errors
			self.logger.error(err, exc_info=True)



	def receiveData(self, size=1024):
		"""
		Receive data (from self.request.recv)
		and log it
		"""
		# get raw data
		try:
			rawdata = self.request.recv(size).strip()
		except ConnectionResetError:
			raise DisconnectionError()
		# convert it to utf-8 (but it could be utf-8 or latin1, let's try and see)
		for enc in ('utf-8', 'latin1'):
			try:
				data = str(rawdata, enc)
				break
			except UnicodeDecodeError:
				pass
		else:
			# if we don't get which encoding it is, with just ignore non utf-8 chars
			data = str(rawdata, 'utf-8', 'ignore')

		# check if the client has closed the connection
		# (don't know why, but when the connection is closed by the client when the server wait with recv, we cannot
		# use the self.server._closed attribute...)
		if data == '':
			raise DisconnectionError()
		# log it
		if self._player:
			self.logger.low_debug("Receive: '%s' from %s (%s) ", data, self._player.name, self.client_address[0])
		else:
			logger.low_debug("Receive: '%s' from %s ", data, self.client_address[0])
		return data


	def sendData(self, data):
		"""
		Send data (with self.request.sendall) and log it
		:param data: (str) data to send
		"""

		try:
			head = SIZE_FMT % len(data.encode("utf-8"))
			self.request.sendall(str(head).encode('utf-8'))
			if data:
				self.request.sendall(data.encode('utf-8'))
			else:
				# that's a trick when we want to send an empty message...
				self.request.sendall(''.encode('utf-8'))
			if self._player:
				self.logger.low_debug("Send '%s' to %s (%s) ", data, self._player.name, self.client_address[0])
			else:
				logger.low_debug("Send '%s' to %s", data, self.client_address[0])
		except BrokenPipeError:
			raise DisconnectionError()
		except OSError:
			raise DisconnectionError()


	@property
	def game(self):
		"""
		Returns the game of the player (self.game is a shortcut for self.game)
		"""
		if self._player:
			return self._player.game
		else:
			return None


	@property
	def logger(self):
		"""
		Return the looger
		(the general logger, or the player's logger if a player already exists)
		"""
		if self._player:
			return self._player.logger
		else:
			return logger

	def getPlayerName(self):
		"""
		Waits for a message "CLIENT_NAME" and treat it
		Returns the player name
		or raises an exception (ProtocolError) if the request is not valid
		"""

		# get data
		data = self.receiveData()
		if not data.startswith("CLIENT_NAME "):
			raise ProtocolError("Bad protocol, should start with CLIENT_NAME ")

		data = data[12:]

		# check if the player doesn't exist yet
		if data in RegularPlayer.allInstances:
			self.sendData("A client with the same name ('" + data + "') is already connected!")
			raise ProtocolError("A client with the same name is already connected: %s (%s)" % (data, self.client_address[0]))


		# check if the name is valid (20 characters max, and only in [a-zA-Z0-9_]
		name = sub(r'\W+', '', data)
		if name != data or len(name) > 20:
			self.sendData("The name is invalid (max 20 characters in [a-zA-Z0-9_])")
			raise ProtocolError("The name '%s' (from %s) is invalid (max 20 characters in [a-zA-Z0-9_])" %
			                    (data, self.client_address[0]))


		# just send back OK
		self.sendData("OK")
		return name



	def waitForGame(self):
		"""
		Waits for a message "WAIT_GAME %s" and then wait for a game (with an Event)
		%s is a string like (options is like "key1=value1 key2=value2 ...")
		- "{options}": regular game (with options)
		- "TOURNAMENT NAME {options}": tournament
		- or "NAME {options}": play agains training player
        Returns nothing
		"""
		# !TODO: normalize the options: should be "[options]", "TRAINING <name> [options]" or "TOURNAMENT <name> [options]"
		# get the WAIT_GAME message
		data = self.receiveData()
		if not data.startswith("WAIT_GAME"):
			self.sendData("Bad protocol, should send 'WAIT_GAME %s' command")
			raise ProtocolError("Bad protocol, should send 'WAIT_GAME %s' command")

		# parse the game type (in the form "TOURNAMENT NAME key1=value1..." or "NAME key1=value1 key2=value2")
		trainingPlayerName = ""
		tournamentName = ""
		options = {}
		try:
			terms = shlex.split(data[10:])
			if terms:
				if "=" in terms[0]:
					trainingPlayerName = ""
					tournamentName = ""
					options = dict([token.split('=') for token in terms])
				elif terms[0] == 'TOURNAMENT':
					trainingPlayerName = ""
					try:
						tournamentName = terms[1]
					except IndexError:
						raise ValueError
					options = dict([token.split('=') for token in terms[2:]])
				elif terms[0] == 'TRAINING':
					try:
						trainingPlayerName = terms[1]
					except IndexError:
						raise ValueError
					tournamentName = ""
					options = dict([token.split('=') for token in terms[2:]])
				else:
					raise ValueError
		except ValueError:
			strerr = "The string sent with 'WAIT_GAME' is not valid (should be '{options}'," \
			         " 'TRAINING <NAME> {options}' or 'TOURNAMENT <NAME> {options}', but is '%s' instead)" % data[10:]
			self.sendData(strerr)
			raise ProtocolError(strerr)

		if trainingPlayerName:
			# Create a particular Game
			try:
				# create game (no need to store it in a variable)
				g = Game.getTheGameClass().gameFactory(trainingPlayerName, self._player, options)
			except ValueError as err:
				self.sendData("The training player sent by '%s' command is not valid (%s)" % (data, err))
				raise ProtocolError("The training player sent by '%s' command is not valid (%s)" % (data, err))
			# log it
			self.logger.debug("The game %s starts with training player `%s` and options=%s" %
			                  (g.name, trainingPlayerName, options))
		elif tournamentName:
			try:
				# register the player in the tournament
				Tournament.registerPlayer(self._player, tournamentName)
			except ValueError as err:
				self.sendData("The tournament '%s' cannot be joined: %s" % (tournamentName, err))
				raise ProtocolError("The tournament '%s' cannot be joined: %s" % (tournamentName, err))

		# just send back OK
		self.sendData("OK")

		# wait for the Game
		# and send every second a "NOT_READY" if the game do not start
		# in order to know if the client has disconnected
		gameHasStarted = self._player.waitForGame(5)
		while not gameHasStarted:
			self.sendData("NOT_READY")
			gameHasStarted = self._player.waitForGame(3)

		# now send the game name
		self.sendData(self.game.name)

		# now send the game sizes
		self.sendData(self.game.getDataSize())





	def sendGameData(self):
		"""
		Waits for a message "GET_GAME_DATA", and then send back the game datas
		Returns nothing
		"""

		# receive data from the socket
		data = self.receiveData()
		if not data.startswith("GET_GAME_DATA"):
			self.sendData("Bad protocol, should send 'GET_GAME_DATA' command")
			raise ProtocolError("Bad protocol, should send 'GET_GAME_DATA' command")

		# Get the labyrinth
		self.sendData("OK")
		self.sendData(self.game.getData(self._player))
		self.sendData('0' if self.game.playerWhoPlays is self._player else '1')  # send '0' if we begin, '1' otherwise

