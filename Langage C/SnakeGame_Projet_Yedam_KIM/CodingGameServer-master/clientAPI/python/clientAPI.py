"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, T. Gautier
Licence: GPL

File: clientAPI.py
	A python version of the clientAPI

Copyright 2019 T. Hilaire, T. Gautier
"""

import socket
import sys
from logging import getLogger, WARNING, StreamHandler, Formatter

# some constants
NORMAL_MOVE = 0
WINNING_MOVE = 1
LOOSING_MOVE = -1


class ClientAPI:
	"""Simple API for the client
	Served as a based class to be extended"""
	def __init__(self, serverName, port, name, debugLevel=WARNING):
		"""Initialize the game
		debug is the debug level (see the levels of the module logging)

		"""
		self._serverName = serverName
		self._port = port
		self.playerName = name
		# configure logger
		self._logger = getLogger('CGS')
		self._logger.setLevel(debugLevel)
		# Add an other handler to redirect some logs to the console
		# (with colors, depending on the level DEBUG/INFO/WARNING/ERROR/CRITICAL)
		steam_handler = StreamHandler()
		steam_handler.setLevel(debugLevel)
		LOGFORMAT = "\033[35m\u26A0\033[0m [%(name)s]\033[0m | %(message)s"
		steam_handler.setFormatter(Formatter(LOGFORMAT))
		self._logger.addHandler(steam_handler)


	def sendString(self, msg):
		"""Send string through socket and acknowledge"""
		self.sock.send(msg.encode())
		self._logger.debug("Send '%s' to the server" % msg)

		res = self.read_inbuf()

		if res != "OK":
			self._logger.error("Error: The server does not acknowledge, but answered:\n%s" % res)
			sys.exit(1)
		self._logger.debug("Receive acknowledgment from the server")

	def read_inbuf(self):
		"""Read data through socket"""
		length = int(self.sock.recv(4).decode())

		self._logger.debug("prepare to receive a message of length :%lu" % length)

		buffer = b""
		read_length = 0
		while read_length < length:
			res = self.sock.recv(length - read_length)
			read_length += len(res)
			buffer += res
				
		return buffer.decode()

	def __enter__(self):
		self._logger.info("Initiate connection with %s (port: %d)" % (self._serverName, self._port))

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((self._serverName, self._port))

		self.sendString("CLIENT_NAME %s" % self.playerName)
	
	def __exit__(self):
		self.sock.close()

	def waitForGame(self, fct, training=""):
		"""wait for a game"""
		if training != "":
			self.sendString("WAIT_GAME %s" % training)
		else:
			self.sendString("WAIT_GAME ")
		
		gameName = "NOT_READY"
		while gameName == "NOT_READY":
			gameName = self.read_inbuf(fct)
		
		self._logger.warning("Receive Game name=%s" % gameName)

		# read game size
		buffer = self.read_inbuf()

		self._logger.info("Receive Game sizes=%s" % buffer)

		return gameName, buffer


	def getGameData(self, fct):

		self.sendString("GET_GAME_DATA")

		# read game data
		data = self.read_inbuf()

		self._logger.info("Receive game's data:%s" % data)

		# read if we begin (0) or if the opponent begins (1)
		buffer = self.read_inbuf()

		self._logger.info("Receive these player who begins=%s" % buffer)
		
		return data, int(buffer)


	def getCGSMove(self, fct):
		self.sendString("GET_MOVE")

		# read move
		move = self.read_inbuf()
		self._logger.info("Receive that move:%s" % move)

		# read the return code
		buffer = self.read_inbuf()
		self._logger.debug("Receive that return code:%s" % buffer)

		# extract result
		result = int(buffer)
		self._logger.debug("results=%d" % result)
		return move, result


	def sendCGSMove(self, fct, move):
		self.sendString("PLAY_MOVE %s" % move)

		# read return code
		buffer = self.read_inbuf()
		self._logger.info("Receive that return code:%s" % buffer)

		result = int(buffer)
		self._logger.debug("results=%d" % result)

		# read msg
		buffer = self.read_inbuf()
		self._logger.info("Receive that message: %s" % buffer)

		return result

	def printGame(self, fct):
		self._logger.debug("Try to get string to display Game")

		self.sendString("DISP_GAME")

		buffer = self.read_inbuf()
		print(buffer)
	
	def sendCGSComment(self, fct, comment):
		self._logger.debug("Try to send a comment")

		if len(comment) > 100:
			self._logger.error("The Comment is more than 100 characters.")
			sys.exit(1)
		self.sendString("SEND_COMMENT %s" % comment)

