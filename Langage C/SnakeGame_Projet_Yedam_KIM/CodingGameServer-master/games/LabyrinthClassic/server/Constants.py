"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire
Licence: GPL

File: Constants.py
	Contains the constants of the game Labyrinth
	-> defines the constants used for the client communication

Copyright 2021 T. Hilaire
"""

from colorama import Back, Fore

# constants defining a move
INSERT_LINE_LEFT = 0
INSERT_LINE_RIGHT = 1
INSERT_COLUMN_TOP = 2
INSERT_COLUMN_BOTTOM = 3
OPPOSITE = {INSERT_LINE_LEFT: INSERT_LINE_RIGHT, INSERT_LINE_RIGHT: INSERT_LINE_LEFT,
			INSERT_COLUMN_TOP: INSERT_COLUMN_BOTTOM, INSERT_COLUMN_BOTTOM: INSERT_COLUMN_TOP}

MAX_ITEM = 24

TOPLEFT = {(True, True): "▛", (True, False): "▀", (False, True): "▌", (False, False): "▘"}
TOPMID = {True: "▀", False: "│"}
TOPRIGHT = {(True, True): "▜", (True, False): "▀", (False, True): "▐", (False, False): "▝"}
MIDLEFT = {True: "▌", False: "─"}
MIDMID = {(True, False, False, False): "┬", (False, True, False, False): "┤",
			(False, False, True, False): "┴", (False, False, False, True): "├",
			(True, True, False, False): "┐", (False, True, True, False): "┘",
			(False, False, True, True): "└", (True, False, False, True): "┌",
		}
MIDRIGHT = {True: "▐", False: "─"}
BOTTOMLEFT = {(True, True): "▙", (True, False): "▄", (False, True): "▌", (False, False): "▖"}
BOTTOMMID = {True: "▄", False: "│"}
BOTTOMRIGHT = {(True, True): "▟", (True, False): "▄", (False, True): "▐", (False, False): "▗"}

BACKPLAYER = {(True, True): Back.LIGHTMAGENTA_EX, (True, False): Back.LIGHTRED_EX, (False, True): Back.LIGHTBLUE_EX, (False, False): Back.RESET}
ITEMCHAR = {(True, True): Fore.MAGENTA, (True, False): Fore.MAGENTA, (False, True): Fore.BLUE, (False, False): Fore.RESET}

LT_RANDOM = [True, True, True, False, False, False, False, False]