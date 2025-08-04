"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *

Authors: T. Hilaire
Licence: GPL

File: Laby.py
	Contains the class Tile and Laby
	-> defines the Labyrinth (its rules, moves, etc.)

Copyright 2021 T. Hilaire
"""

from dataclasses import dataclass
from itertools import product
from random import randint, shuffle, choice
from logging import getLogger

from .Constants import MAX_ITEM, LT_RANDOM
from .Constants import INSERT_COLUMN_BOTTOM, INSERT_COLUMN_TOP, INSERT_LINE_LEFT, INSERT_LINE_RIGHT
from .Constants import TOPLEFT, TOPMID, TOPRIGHT, MIDLEFT, MIDRIGHT, BOTTOMLEFT, BOTTOMMID, BOTTOMRIGHT, MIDMID


logger = getLogger("Labyclassic")  # general logger ('root')

TYPE = {(True, False, True, False): ('I', 0), (False, True, False, True): ('I', 1),
		(False, False, True, True): ('L', 1), (True, False, False, True): ('L', 1),
		(True, True, False, False): ('L', 2), (False, True, True, False): ('L', 3),
		(True, False, False, False): ('T', 0), (False, True, False, False): ('T', 1),
		(False, False, True, False): ('T', 2), (False, False, False, True): ('T', 3)
}

@dataclass
class Tile:
	"""simple class for a tile"""
	north: bool = False
	east: bool = False
	south: bool = False
	west: bool = False
	item: int = 0
	reachable: bool = False

	def rotate(self, rot: int):
		"""rotate a tile of rot 1/4 round in clockwise direction"""
		for _ in range(rot):
			self.north, self.east, self.south, self.west = self.west, self.north, self.east, self.south

	def toType(self):
		"""return a tuple type, rot, item
		where type is in 'T', 'L' or 'I' and rot is an integer between 0 and 3 (1/4 round in clockwise)"""
		return (self.north, self.east, self.south, self.west, self.item)
		#return TYPE[(self.north, self.east, self.south, self.west)] + (self.item,)




class Laby:
	"""simple class for the laby
	- lab: list of lists (lab[x][y] with 0 <= x < L and 0 <= y < H)
	- extra tile
	"""

	def __init__(self, L: int, H: int):
		"""create the labyrinth
		"""
		self.L = L
		self.H = H
		# empty lab L*H and extra tile
		self._lab = [[Tile() for _ in range(H)] for _ in range(L)]
		self._extraTile = Tile()
		# corners
		self._lab[0][0] = Tile(north=True, west=True)
		self._lab[0][-1] = Tile(south=True, west=True)
		self._lab[-1][0] = Tile(north=True, east=True)
		self._lab[-1][-1] = Tile(south=True, east=True)

		# T in every even position (lab[i][j] with i and j even, and not i and j both equal to extremity)
		# MAX_ITEMS/2 items are randomly put on these positions
		items = list(range(1, MAX_ITEM // 2 + 1)) + [0] * ((H + 1) // 2 * (L + 1) // 2 - MAX_ITEM // 2)
		shuffle(items)
		for (i, j), item in zip(product(range(0, L, 2), range(0, H, 2)), items):
			if i == 0 or i == (L - 1) or j == 0 or j == (H - 1):
				self._lab[i][j] = Tile(item=item, north=(j == 0), south=(j == (H - 1)), east=(i == (L - 1)), west=(i == 0))
			else:
				d = randint(0, 3)
				self._lab[i][j] = Tile(item=item, north=(d == 0), east=(d == 1), south=(d == 2), west=(d == 3))

		# now distribution of all the other tiles (3* L//2 * H//2)
		# MAX_ITEM/4 T-shape tiles (with items)
		# MAX_ITEM/4 L-shape tiles (with items)
		# and then random tiles with no item (5 T-shape tiles for 3 L-shape tiles)
		tiles = [Tile(item=i, north=True) for i in range(MAX_ITEM//2+1, MAX_ITEM*3//4+1)]
		tiles.extend(Tile(item=i, north=True, east=True) for i in range(MAX_ITEM*3//4+1, MAX_ITEM+1))
		tiles.extend(Tile(item=0, north=True, east=choice(LT_RANDOM)) for _ in range(3*(L//2)*(H//2)-MAX_ITEM//2 + 1 + L//2 + H//2))
		# shuffle all the tiles
		shuffle(tiles)
		for i in tiles:
			i.rotate(randint(1, 4))
		# distribute them
		for i, j in product(range(0, L-1, 2), range(0, H-1, 2)):
			self._lab[i + 1][j] = tiles.pop()
			self._lab[i][j + 1] = tiles.pop()
			self._lab[i + 1][j + 1] = tiles.pop()
		for i in range(1, L, 2):
			self._lab[i][H-1] = tiles.pop()
		for j in range(1, H, 2):
			self._lab[L-1][j] = tiles.pop()
		assert(len(tiles) == 1)

		self._extraTile = tiles[0]

	@property
	def extraTile(self) -> Tile:
		"""returns the extra tile"""
		return self._extraTile

	def __getitem__(self, key: tuple[int, int]):
		return self._lab[key[0]][key[1]]

	def toStr(self, x: int, y: int, c: str) -> tuple[str, str, str]:
		"""returns three strings representing the tile at position (x,y)
		ie ["▛▀▀",
		    "▍c ",
		    "▍ ▗"]
		where c is the extra character put in the middle (it can be composed with color, so it is in fact a full string)
		If x < 0, then the extra tile is used
		"""
		if x >= 0:
			tile = self._lab[x][y]
		else:
			tile = self._extraTile
		top = TOPLEFT[tile.north, tile.west] + TOPMID[tile.north] + TOPRIGHT[tile.north, tile.east]
		mid = MIDLEFT[tile.west] + c.replace('·', MIDMID[tile.north, tile.east, tile.south, tile.west]) + MIDRIGHT[tile.east]
		bot = BOTTOMLEFT[tile.south, tile.west] + BOTTOMMID[tile.south] + BOTTOMRIGHT[tile.south, tile.east]
		return top, mid, bot

	def insertExtraTile(self, insert: int, number: int, playerPos: list[tuple[int, int]]):
		"""Insert the extra tile and rotate a line or column"""
		# insert the extra tile (and produce a new extra tile)
		if insert == INSERT_COLUMN_TOP:
			# insert the extra tile in the column top
			extra = self._lab[number][-1]
			self._lab[number] = [self._extraTile] + self._lab[number][:-1]
			self._extraTile = extra
		elif insert == INSERT_COLUMN_BOTTOM:
			# insert the extra tile in the column bottom
			extra = self._lab[number][0]
			self._lab[number] = self._lab[number][1:] + [self._extraTile]
			self._extraTile = extra
		elif insert == INSERT_LINE_LEFT:
			# insert the extra tile in the line left
			LL = [self._extraTile] + [line[number] for line in self._lab]
			for line in self._lab:
				line[number] = LL.pop(0)
			self._extraTile = LL.pop(0)
		elif insert == INSERT_LINE_RIGHT:
			# insert the extra tile in the line right
			LL = [line[number] for line in self._lab] + [self._extraTile]
			self._extraTile = LL.pop(0)
			for line in self._lab:
				line[number] = LL.pop(0)


		# move the players
		for i in range(0, 2):
			x, y = playerPos[i]
			# check if the player is on a moved line or column
			# if so, compute the new position (the player go back to the beginning of the line/column if it is expulsed)
			if x == number and insert == INSERT_COLUMN_TOP:
				y = (y+1) % self.H
			if x == number and insert == INSERT_COLUMN_BOTTOM:
				y = (y-1) % self.H
			if y == number and insert == INSERT_LINE_LEFT:
				x = (x+1) % self.L
			if y == number and insert == INSERT_LINE_RIGHT:
				x = (x-1) % self.L
			playerPos[i] = x, y


	def reachable(self, x: int, y: int):
		"""computes all the tiles that can be reached from position x,y
		update the `reacheable` attribute of the tiles"""
		# set reachable to False
		for i, j in product(range(self.L), range(self.H)):
			self._lab[i][j].reachable = False
		# "color" the lab, starting from x,y
		stack = [(x, y)]
		while stack:
			# color the 1st item in the stack
			i, j = stack.pop(0)
			self._lab[i][j].reachable = True
			# stack the neighbors that are not yet reachable
			if (not self._lab[i][j].north) and j > 0 and (not self._lab[i][j-1].reachable) and (not self._lab[i][j-1].south):
				stack.append((i, j-1))
			if (not self._lab[i][j].south) and j < (self.H-1) and (not self._lab[i][j+1].reachable) and (not self._lab[i][j+1].north):
				stack.append((i, j+1))
			if (not self._lab[i][j].west) and i > 0 and (not self._lab[i-1][j].reachable) and (not self._lab[i-1][j].east):
				stack.append((i-1, j))
			if (not self._lab[i][j].east) and i < (self.L-1) and (not self._lab[i+1][j].reachable) and (not self._lab[i+1][j].west):
				stack.append((i+1, j))

	def toJSON(self):
		"""return a dictionary used by the JS client"""
		return {
			'sizeX': self.L,
			'sizeY': self.H,
			'extra': self._extraTile.toType(),
			'lab':  [[self._lab[x][y].toType() for x in range(self.L)] for y in range(self.H)]
		}