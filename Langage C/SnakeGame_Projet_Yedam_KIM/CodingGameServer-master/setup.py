# coding: utf-8
"""

* --------------------- *
|                       |
|   Coding Game Server  |
|                       |
* --------------------- *


Authors: T. Hilaire, J. Brajard
Licence: GPL

File: setup.py
	install file for Coding Game Server


CGS can be installed with
>> python setup.py install

Copyright 2016-2019 T. Hilaire, J. Brajard
"""


from setuptools import setup


def readme():
	"""returns the README.md file"""
	with open('README.md') as f:
		return f.read()


setup(
	name='CGS',
	version='0.6',
	description='CodingGameServer (CGS) is a framework to create some Coding Games',
	long_description=readme(),
	classifiers=[
		'License :: OSI Approved :: GPL License',
		'Programming Language :: Python :: 3.6',
		'Topic :: Education',
	],
	keywords='coding game server',
	url='https://github.com/thilaire/CodingGameServer',
	author='Thibault Hilaire',
	author_email='thibault.hilaire@lip6.fr',
	license='GPL',
	packages=['CGSserver'],
	install_requires=[
		'colorama', 'colorlog', 'docopt', 'flask', 'jinja2', 'ansi2html',
		'flask-socketio', 'gevent', 'eventlet', 'gevent-websocket', 'pyyaml-include'
	],
	include_package_data=True,
	zip_safe=False,
	entry_points={'console_scripts': ['runCGS=CGSserver.runCGS:runCGS']},
)
