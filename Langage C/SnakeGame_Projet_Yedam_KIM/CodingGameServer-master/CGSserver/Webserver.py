"""
* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire, J. Brajard
Licence: GPL

File: webserver.py
	Contains the webserver routines (based on Flask)
	-> all the routes are defined here
	-> the template files used are in templates

Copyright 2016-2019 T. Hilaire, J. Brajard
"""

from flask import Flask, render_template, abort, send_from_directory, request, redirect
from jinja2 import ChoiceLoader, FileSystemLoader

from flask_socketio import SocketIO, join_room
import threading
from glob import glob
from os.path import isfile, join, basename, splitext
from CGSserver.Game import Game
from CGSserver.Player import RegularPlayer
from CGSserver.Logger import Config
from CGSserver.Tournament import Tournament
from CGSserver.BaseClass import BaseClass
from CGSserver.Constants import mode


# flask object and socketIO ('threading' in debug/dev mode, but 'gevent' in prod)
flask = Flask("webserver")
if mode == 'prod':
	socketio = SocketIO(flask, async_mode='gevent')
else:
	socketio = SocketIO(flask, async_mode='threading')

# set the template paths so that in priority,
# it first looks in <gameName>/server/templates/ and then in CGS/server/templates
templatePaths = ['games/' + Game.getTheGameName() + '/server/templates/', 'CGSserver/templates/']


def runWebServer(host, port):
	"""
	Run the webserver
	"""
	# add a custom jinja loader
	my_loader = ChoiceLoader([flask.jinja_loader, FileSystemLoader(templatePaths), ])
	flask.jinja_loader = my_loader

	# set some global variables
	flask.jinja_env.globals['base_url'] = '/'
	flask.jinja_env.globals['GameName'] = Game.getTheGameName()
	flask.jinja_env.globals['host'] = Config.host
	flask.jinja_env.globals['webPort'] = Config.webPort
	flask.jinja_env.globals['SubTitle'] = 'A CGS-based game'

	# Start the web server, in background
	flask.logger.message('Run the web server on port %d...', port)
	flask.config['SECRET_KEY'] = 'QSDFGHJKLM|'

	BaseClass.socketio = socketio
	# TODO: do not run it in a separate thread, but use socketio.start_background_task instead
	# see https://stackoverflow.com/questions/34581255/python-flask-socketio-send-message-from-thread-not-always-working
	socketio.run(flask, host=host, port=port, debug=True, use_reloader=False, allow_unsafe_werkzeug=True)


# # ================
# #   main page
# # ================
@flask.route('/')
@flask.route('/index.html')
def index():
	"""
	Main page (based on index.html template)
	"""
	return render_template('index.html')


# ================
#   static files
# ================
def static_file(fileName):
	"""
	Returns a static_file from the static paths
	The function first searches in the first path of the template path list (staticPaths).
	If the file exists, the function returns that file, otherwise it searches
	for the file in the next path...
	Redirects to error 404 if the file is not found.
	"""
	for path in templatePaths:
		if isfile(join(path, fileName)):
			return send_from_directory(path, fileName)
	abort(404)


# some static files
@flask.route('/favicon.ico')
def favicon():
	"""Returns the favicon"""
	return static_file('favicon.ico')


@flask.route('/style.css')
def css():
	"""Returns the CSS style"""
	return static_file('style.css')


@flask.route('/game/gamestyle.css')
def game_css():
	"""Returns the CSS game display style"""
	return static_file('game/gamestyle.css')


@flask.route('/banner.png')
def banner():
	"""Returns the pages top banner PNG file"""
	return static_file('banner.png')


# =======
#  Games
# =======
@flask.route('/games.html')
def games():
	"""Web page that display all the (available) players"""
	return render_template('game/games.html')


@flask.route('/new_game.html')
def new_game():
	"""
	Page to create a new game
	"""
	Players = ''.join(['<option value="%s">%s</option>\n' % (p.name, p.name) for p in RegularPlayer.allInstances.values()])
	return render_template('game/new_game.html', list_players=Players, nb_players=len(RegularPlayer.allInstances))


@flask.route('/create_new_game.html', methods=['POST'])
def create_new_game():
	"""
	Receive the form to create a new game
	-> create the game (ie runPhase it)
	"""
	# get Players
	player1 = RegularPlayer.getFromName(request.form.get('player1'))
	player2 = RegularPlayer.getFromName(request.form.get('player2'))

	seed = request.form.get('seed')
	delay = request.form.get('delay')
	timeout = request.form.get('timeout')
	# !TODO: add some options (timeout, seed, etc.) in the html, and send them to the constructor
	try:
		# the constructor will check if player1 and player2 are available to play
		# no need to store the game object created here
		Game.getTheGameClass()(player1, player2, seed=seed, delay=delay, timeout=timeout)

	except ValueError as e:
		return render_template('error.html', error='Error. Impossible to create a game with ' +
		                                           str(request.form.get('player1')) + ' and ' + 
		                                           str(request.form.get('player2')) + ': "' + str(e) + '"')
	else:
		return redirect('/')


@flask.route('/game/<gameName>')
def game(gameName):
	"""Returns the webpage of a game
	<gameName> is the name of the game
	If the name is not valid, the answer with the noObject page
	"""
	g = Game.getFromName(gameName)

	if g:
		try:
			displayName = g.getCutename()
		except NotImplementedError:
			displayName = gameName
		return render_template(
			'game/Game.html', host=Config.host, webPort=Config.webPort, gameName=gameName, displayName=displayName, 
			player1=g.players[0].HTMLrepr(), player2=g.players[1].HTMLrepr()
		)
	else:
		return render_template('error.html', error="The Game %s doesn't exist." % (gameName,))


@flask.route('/data/<path:something>')
def gameData(something):
	"""Returns something needed by a grame
	"""
	return static_file(something)


# ============
#  Tournament
# ============
@flask.route('/tournaments.html')
def tournaments():
	"""Web page that display all the (available) tournaments"""
	return render_template('tournament/tournaments.html')


@flask.route('/new_tournament.html')
def new_tournament():
	"""
	Page to create a new tournament
	Build from HTMLFormDict class method of TournamentMode (build from all the tournament modes)
	"""
	return render_template("tournament/new_tournament.html", **Tournament.HTMLFormDict(Game.getTheGameName()))


@flask.route('/create_new_tournament.html', methods=['POST'])
def create_new_tournament():
	"""
	Receive the form to create a new tournament
	"""
	# create the tournament
	try:
		Tournament.factory(**dict(request.form))
	except ValueError as e:
		# !TODO: redirect to an error page
		# TODO: log this
		return render_template('error.html', error='Error: Impossible to create a tournament with ' + str(dict(request.form))
		                                           + ':"' + str(e) + '"')
	else:
		return redirect('/')


@flask.route('/tournament/<tournamentName>')
def tournament(tournamentName):
	"""
	Web page for a tournament
	redirect to `noTournament.html` if tournament doesn't exist
	Parameters:
	- tournamentName: name of the tournament
	"""
	t = Tournament.getFromName(tournamentName)
	if t:
		return render_template('tournament/tournament.html', t=t, host=Config.host, webPort=Config.webPort)
	else:
		return render_template('error.html', error="The Tournament %s doesn't exist." % (tournamentName,))



@flask.route('/run_tournament/<tournamentName>', methods=['POST'])
def runTournament(tournamentName):
	"""
	Receive the runPhase tournament form
	redirect to `noTournament.html` if the tournament doesn't exit
	other, return nothing, since it is run from ajax (doesn't wait for any response)
	Parameters:
	- tournamentName: name of the tournament
	"""
	t = Tournament.getFromName(tournamentName)
	if t:
		threading.Thread(target=t.runPhase, kwargs=dict(request.form)).start()
		return ''
	else:
		return render_template('error.html', error="The Tournament %s doesn't exist." % (tournamentName,))


# =========
#  Player
# =========

@flask.route('/player/<playerName>')
def player(playerName):
	"""
	Web page for a player
	Redirects to `noPlayer.html` if the player doesn't exist
	"""
	pl = RegularPlayer.getFromName(playerName)
	if pl:
		return render_template('player/Player.html', host=Config.host, webPort=Config.webPort, playerName=playerName)
	else:
		return render_template('error.html', error="The Player %s doesn't exist." % (playerName,))


@flask.route('/players.html')
def players():
	"""Web page that display all the (available) players"""
	return render_template('player/players.html')


@flask.route('/player/disconnect/<playerName>')
def disconnectPlayer(playerName):
	"""
	Disconnect a player
	Only for debug...
	:param playerName:
	:return:
	"""
	# !FIXME: activate this only in debug or dev mode
	# TODO: if necessary, add a disconnectAllPlayer
	pl = RegularPlayer.getFromName(playerName)
	if pl:
		pl.disconnect()
		return redirect('/')
	else:
		return render_template('error.html', error="The Player %s doesn't exist." % (playerName,))


# ==========
# Websockets
# ==========
# the clients just register to have update
wsCls = {
	Game.getTheGameName(): Game.getTheGameClass(), 'RegularPlayer': RegularPlayer, 'Tournament': Tournament, 
	'League': Tournament, 'PoolKnockout': Tournament
}


@socketio.on('registerList')
def websocket_class(data):
	"""When a client want to register and have the list of Game, Player, Tournament,..."""
	if not isinstance(data, list):
		data = [data]
	# iter over the class the page want to receive the list of instances
	for p in data:
		if p in wsCls:
			# add it in the appropriate room
			join_room(room=p)
			wsCls[p].sendListofInstances()
		else:
			flask.logger.debug("Receive (ang ignore) incompatible data on channel 'registerList': %s", data)


@socketio.on('registerObject')
def websocket_Object(data):
	"""Register an object (so it will receive feedback through a websocket"""
	clsName, name = data
	if clsName in wsCls:
		cls = wsCls[clsName]
		obj = cls.getFromName(name)
		if obj:
			# add it in the appropriate room
			join_room(room=clsName + '/' + name)
			obj.sendUpdateToWebSocket(firstTime=True)
		else:
			flask.logger.info(
				'Receive (and ignore) incompatible data on channel registerObject: Invalid name (%s) for class %s',
				name, clsName
			)
	else:
		flask.logger.info(
			'Receive (and ignore) incompatible data on channel registerObject: Invalid class %s is not in %s', 
			clsName, wsCls.keys()
		)


# ======
#  logs
# =======
@flask.route('/log.html')
def log_():
	"""Returns the log.html"""
	return render_template("log/logs.html")


@flask.route('/logs/games.html')
def log_games():
	"""Returns the log/games.html
	The list of game logs availables"""
	# build the list of files in log/games
	files = glob(join(Config.logPath, "Games/*.log"))
	# render the page
	logg = [
		'<li><a href="/logs/game/%s">%s</a></li>' % (splitext(basename(f))[0], splitext(basename(f))[0]) 
		for f in files
	]
	return render_template("log/games.html", games_log="\n".join(logg))


@flask.route('/logs/players.html')
def log_players():
	"""Returns the log/players.html
	The list of player logs availables"""
	# build the list of files in log/games
	files = glob(join(Config.logPath, "Players/*.log"))
	# render the page
	logg = [
		'<li><a href="/logs/player/%s">%s</a></li>' % (splitext(basename(f))[0], splitext(basename(f))[0])
		for f in files
	]
	return render_template("log/players.html", players_log="\n".join(logg))


@flask.route('/logs/activity')
def log_activity():
	"""Returns the activity.log file"""
	return send_from_directory(Config.logPath, 'activity.log')


@flask.route('/logs/errors')
def log_error():
	"""Returns the errors.log file"""
	return send_from_directory(Config.logPath, 'errors.log')


@flask.route('/logs/player/<playerName>')
def logP(playerName):
	"""
	Returns a player log file
	:param playerName: (string) name of the player
	"""
	return send_from_directory(join(Config.logPath, 'Players'), playerName+'.log')


@flask.route('/logs/game/<gameName>')
def logG(gameName):
	"""
	Returns a game log file
	:param gameName: (string) name of the game
	"""
	return send_from_directory(join(Config.logPath, 'Games'), gameName + '.log')


@flask.route('/logs/tournament/<tournamentName>')
def logT(tournamentName):
	"""
	Returns a tournament log file
	:param tournamentName: (string) name of the game
	"""
	return send_from_directory(join(Config.logPath, 'Tournaments'), tournamentName + '.log')


# ================
#   info page
# ================
@flask.route('/about.html')
def about():
	"""
	About page
	"""
	return render_template("about.html")


# =======
#  errors
# ========
@flask.errorhandler(404)
def error404(err):
	"""Returns error 404 page"""
	# TODO: log this
	return render_template('error404.html', message=err)


@flask.errorhandler(500)
def errror500(err):
	"""
	Return for error 500
	"""
	# TODO: return a full page ?
	flask.logger.error(err, exc_info=True)
	return "We have an unexpected error. It has been reported and logged," \
	       " and we will work on it so that it never occurs again !"
