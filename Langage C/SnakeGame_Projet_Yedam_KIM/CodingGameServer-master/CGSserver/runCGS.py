#!/usr/bin/env python3
# -*- coding: utf-8
"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *


Authors: T. Hilaire, J. Brajard
Licence: GPL

File: runCGS.py
	Main file/entry for the Coding Game Server


CGS requires Python3 and the following packages:
- colorama, colorlog: display color in text (and logs)
- docopt: parse the documentation and implement it
- jinja2, flask, flask-socketio, eventlet: webserver (+ websocket)
>> pip install colorama colorlog docopt jinja2 ansi2html flask flask-socketio gevent eventlet

Copyright 2016-2019 T. Hilaire, J. Brajard
"""

from docopt import docopt                   # used to parse the command line


usage = """
Coding Game Server
Run the servers (Game server and web server)

Usage:
  runCGS.py -h | --help
  runCGS.py <gameName> [options] [--debug|--dev|--prod]

Options:
  gameName                 Name of game [default: Labyrinth]
  -h --help                Show this screen.
  -p PORT --port=PORT      Game server port [default: 1234].
  -w PORT --web=PORT       Web server port [default: 8088].
  -H HOST --host=HOST      Servers host [default: localhost].
  -e EMAIL --email=EMAIL   Email address used in prod to send info when the server fails [default: pythoncgs@gmail.com]
  -s SMTP --smtp=SMTP      SMTP server:port used in prod to send the email [default: smtp.gmail.com:587]
  -l LOGS --log=LOGS       Folder where the logs are stored [default: logs/{{hostname}}/]
  --no-email               Do not send email in production [default: False]
  --no-webserver           Do not run the webserver [default: False]
  --debug                  Debug mode (log and display everything)
  --dev                    Development mode (log everything, display infos, warnings and errors)
  --prod                   Production mode (only log infos, warnings and errors and send emails) [default: True]
"""


def runCGS():
	"""run CGS server"""
	# parse the command line
	args = docopt(usage)
	if (not args['--debug']) and (not args['--dev']) and (not args['--prod']):
		args['--prod'] = True
	args['--port'] = int(args['--port'])
	args['--web'] = int(args['--web'])
	gameName = args['<gameName>']

	# get the mode (export it to Constants.mode, so that the webserver can have it as global constant)
	mode = 'prod' if args['--prod'] else 'dev' if args['--dev'] else 'debug'
	import CGSserver.Constants as Constants
	Constants.mode = mode

	# now, we can import the library, depending on the mode (prod vs dev/debug)
	# because in case of prod, we import gevent and monkey patch all
	if mode == 'prod':
		from gevent import monkey
		monkey.patch_all()


	import threading  # to run threads
	from importlib import import_module  # to dynamically import modules
	from socketserver import ThreadingTCPServer  # socket server (with multi-threads capabilities)
	from colorama import Fore

	from CGSserver.Game import Game
	from CGSserver.Logger import configureRootLogger
	from CGSserver.Player import PlayerSocketHandler  # TCP socket handler for players

	# import the <gameName> module and store it (in Game)
	try:
		mod = import_module('games.' + gameName + '.server.' + gameName)
		if gameName not in mod.__dict__:
			print(Fore.RED + "Error: The file `games/" + gameName + "/server/" + gameName +
			      ".py` must contain a class named `" + gameName + "`." + Fore.RESET)
			quit()
		Game.setTheGameClass(mod.__dict__[gameName])
	except ImportError as e:
		print(Fore.RED + "Error: Impossible to import the file `games/" + gameName +
		      "/server/" + gameName + ".py`." + Fore.RESET)
		print(e)
		quit()

	# configure the loggers
	logger = configureRootLogger(args)

	# Start !
	logger.message("")
	logger.message("#=====================================================#")
	logger.message("# Coding Game Server is going to start (mode=`%s`) #" % mode)
	logger.message("#=====================================================#")
	logger.message("")

	# Run the webserver
	if not args['--no-webserver']:
		from CGSserver.Webserver import runWebServer  # to run the webserver (Flask)
		# TODO: do not run it in a separate thread, but use socketio.start_background_task instead
		# see https://stackoverflow.com/questions/34581255/python-flask-socketio-send-message-from-thread-not-always-working
		threading.Thread(
			target=runWebServer,
			kwargs={'host': args['--host'], 'port': args['--web']}
		).start()


	# Start TCP Socket server (connection to players)
	PlayerServer = ThreadingTCPServer((args['--host'], args['--port']), PlayerSocketHandler)
	logger.message("Run the game server on port %d...", args['--port'])
	threading.Thread(target=PlayerServer.serve_forever())


if __name__ == "__main__":
	runCGS()


# !TODO: add a timeout for the dataReceive (this exists in the BaseRequestHandler class)
# TODO: send pretty emails (when send from webserver)
# !TODO: allows the C API do not quit when there is an error (and to get back the error
#  message) -> in some hidden variables "onErrorContinue" and "lastError" ?
# !TODO: unify the docstrings (`Parameters` vs `param:`, check with sphinx)
# FIXME: when a player play (or wait for a move) but it's not his
#  turn, the client exits and disconnects. This should be a problem in tournament (should only loose the game): in
#  the client API, should display the error and return LOSING_MOVE/WINNING_MOVE
