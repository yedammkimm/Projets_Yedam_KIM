"""
* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, J. Brajard
Licence: GPL

File: WebSocketBase.py
	Contains the base class to answer to websockets asking for information about the derived class
	-> defines the generic behavior wrt websockets
	-> Game, RegularPlayer, Tournament will derived from it

Copyright 2016-2017 T. Hilaire, J. Brajard
"""

import logging
import json
from CGSserver.Logger import configureBaseClassLogger

logger = logging.getLogger()


class BaseClass:
	"""
	This class is the base class of the classes Player, Game and Tournament

	It put in common:
	- the dictionary of all the instances (all the players, all the games, etc.)
	- the name
	- the logger
	- some methods to manage the webSockets
	(send informations about objects and list of objects to webpages, through websockets)

	"""
	socketio = None
	allInstances = {}  # unnecessary (will be overwritten by the inherited class, and unused)

	# TODO: we should use weak references here (for the allInstances dictionary) (see
	#  http://stackoverflow.com/questions/37232884/in-python-how-to-remove-an-object-from-a-list-if-it-is-only
	#  -referenced-in-that)

	def __init__(self, name):
		"""
		Base constructor
		- store the name, create the logger
		- add the object to the dictionary of all instances
		- and create list of websockets

		Should be called at the end of the subclass init

		Parameters:
		name: (string) name of the instance
		"""
		# store the name
		self._name = name

		# create and configure the logger
		self._logger = configureBaseClassLogger(self.__class__, name)

		# add itself to the dictionary of games/player/tournament
		if name in self.allInstances:
			raise ValueError("A %s with the same name already exist" % self.__class__.__name__)
		self.allInstances[name] = self

		# send the new list of instances to web listeners
		self.__class__.sendListofInstances()

	# ===========
	# Properties
	# ===========
	@property
	def name(self):
		"""Returns the name of the object"""
		return self._name

	@property
	def logger(self):
		"""Returns the logger of the object"""
		return self._logger

	# ========================
	# Manage list of instances
	# ========================

	@classmethod
	def getFromName(cls, name):
		"""
		Get an instance of this class from its name
		Parameters:
		- name: (string) name of the instance (used as key in the dictionary)
		Returns the object or None if it doesn't exist
		"""
		return cls.allInstances.get(name, None)

	@classmethod
	def removeInstance(cls, name):
		"""
		Remove one instance
		- remove from the dictionary of all the instances
		- close the logger handlers associated (close the log files)
		Parameters:
		- name: (string) name of the instance
		"""
		# remove from the list of instances
		if name in cls.allInstances:
			obj = cls.allInstances[name]
			# close the associated logger handlers (close file)
			for handler in obj.logger.handlers[:]:
				handler.close()
				obj.logger.removeHandler(handler)
			del cls.allInstances[name]
			type(obj).sendListofInstances()     # type(obj) may be different to cls (obj may be a Snake and cls the Game class)

	# ===================================
	# List of Instances (LoI) WebSockets
	# ===================================
	@classmethod
	def sendListofInstances(cls):
		"""
		Broadcast the list of instances
		Called everytime the list of instances is changed
		"""
		if cls.socketio:
			# prepare the list
			d = [obj.HTMLrepr() for obj in cls.allInstances.values()]
			js = json.dumps(d)
			# broadcast to the client in the room)
			logger.low_debug("send list of %s", cls.__name__)
			cls.socketio.emit('list'+cls.__name__, js, room=cls.__name__)

	def sendUpdateToWebSocket(self, firstTime=False):
		"""
		Send some informations about self through all the websockets (in the specific room)
		Called everytime the object (self) is changed
		"""
		if self.socketio:
			js = json.dumps(self.getDictInformations(firstTime))
			logger.low_debug("send 'update' to webseocket for room %s", self.__class__.__name__+'/'+self.name)
			# send to all the websockets or only to one
			self.socketio.emit('update'+self.__class__.__name__, js, room=self.__class__.__name__+'/'+self.name)

	def getDictInformations(self, firstTime=False):
		"""
		Send information (a dictionary) about the object

		TO BE OVERLOADED

		- firstTime is True when this is called for the 1st time by a websocket
		"""
		return {}
