#!/usr/bin/env python3
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# Nicolas Pécheux <info.cpge@cpge.info>
# http://cpge.info
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

"""Vérification du DM n°09."""

import unittest

def tous_tests():
    """Effectue les tests."""
    total = 0
    success = 0
    print('=' * 40)
    tests = unittest.TestLoader().loadTestsFromTestCase(Test)
    for t in tests:
        with open(os.devnull, 'w', encoding="utf8") as null:
            res = unittest.TextTestRunner(stream=null).run(t)
        print(str(t).split()[0], end=' : ')
        if len(res.failures + res.errors):
            print('FAUX')
        else:
            print('ok')
            success += 1
        total += 1
    print('=' * 40)
    print('Tests réussis : {}/{}'.format(success, total))

if __name__ == '__main__':

    import sys
    import os
    import os.path as osp
    import importlib

    # Vérifier que le code source de l'élève est bien là
    if not osp.exists("dm09.py"):
        raise FileNotFoundError(
            "Le code source dm09.py n'a pas été trouvé. Vérifier que votre "
            "programme est bien dans le même répertoire que ce script, qu'il "
            "s'appelle bien dm09.py et que vous avez utilisé l'exécution en "
            "tant que script sous Pyzo.")

    # Vérifier que la base de données est bien là
    if not osp.exists("communes.db"):
        raise FileNotFoundError(
            "La base de données communes.db n'a pas été trouvée. Vérifier qu'elle "
            "est bien dans le même répertoire que ce script.")

    # Vérifier que sqlite3 est disponible
    try:
        import sqlite3
    except ImportError:
        sys.stderr.write(
            "Sqlite3 n'est pas installé.\n\n")

    # Importer le code source de l'élève
    try:
        if "dm09" not in sys.modules:
            _P = importlib.import_module("dm09")
        else:
            _P = importlib.reload(_P)
    except:
        raise InterruptedError(
            "Erreur : votre programme source ne compile pas !")

    # Importer les tests
    if not osp.exists("tests_dm09.py"):
        raise FileNotFoundError(
            "Le fichier tests_dm09.py n'a pas été trouvé. Vérifier qu'il se "
            "trouve bien dans le même répertoire que ce script.")

    with open("tests_dm09.py", "rt", encoding="utf8") as tests_file:
        exec(tests_file.read())

    tous_tests()
