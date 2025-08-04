
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# Nicolas Pécheux <info.cpge@cpge.info>
# http://cpge.info
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

def SQLquery(cmd):
    # Ouverture de la base et exécution de la commande
    conn = sqlite3.connect('communes.db')
    c = conn.cursor()
    c.execute(cmd)
    rows = c.fetchall()
    conn.commit()
    conn.close()
    return rows

class Test(unittest.TestCase):

    def test_question_01(self):
        prof = set(SQLquery("""SELECT * FROM communes WHERE nom = 'Dijon';"""))
        eleve = set(SQLquery(_P.question_01))
        self.assertEqual(prof, eleve)

    def test_question_02(self):
        eleve = SQLquery(_P.question_02)
        self.assertEqual(eleve[0][0], 34970)

    def test_question_03(self):
        eleve = SQLquery(_P.question_03)
        self.assertEqual(eleve[0][0], 9)

    def test_question_04(self):
        eleve = SQLquery(_P.question_04)
        self.assertTrue(50 < eleve[0][0] < 5000)

    def test_question_05(self):
        eleve = SQLquery(_P.question_05)
        self.assertEqual(len(eleve), 32807)

    def test_question_06(self):
        eleve = SQLquery(_P.question_06)
        self.assertEqual(len(eleve), 4)

    def test_question_07(self):
        eleve = SQLquery(_P.question_07)
        self.assertEqual(len(eleve), 8)

    def test_question_08(self):
        eleve = SQLquery(_P.question_08)
        self.assertTrue(1 < eleve[0][0] < 11)

    def test_question_09(self):
        eleve = SQLquery(_P.question_09)
        self.assertTrue(0.1 < eleve[0][0] < 0.4)

    def test_question_10(self):
        eleve = SQLquery(_P.question_10)
        self.assertEqual(len(eleve[0]), 3)
        self.assertEqual(len(eleve), 34869)

    def test_question_11(self):
        eleve = SQLquery(_P.question_11)
        self.assertEqual(len(eleve), 29773)

    def test_question_12(self):
        eleve = SQLquery(_P.question_12)
        self.assertEqual(len(eleve[0]), 5)

    def test_question_13(self):
        eleve = SQLquery(_P.question_13)
        self.assertTrue(300 < len(eleve) < 900)

    def test_question_14(self):
        eleve = SQLquery(_P.question_14)
        self.assertEqual(len(eleve), 101)
        self.assertEqual(sorted(eleve)[0], ("Agen", "Lot-et-Garonne", None))

    def test_question_15(self):
        eleve = SQLquery(_P.question_15)
        self.assertEqual(len(eleve), 1456)
        self.assertEqual(eleve[0], ("Sainte-Colombe", 12))

    def test_question_16(self):
        eleve = SQLquery(_P.question_16)
        self.assertEqual(eleve[0][0], 66361638)

    def test_question_17(self):
        eleve = SQLquery(_P.question_17)
        self.assertEqual(eleve[0][0], 30)

    def test_question_18(self):
        eleve = set(SQLquery(_P.question_18))
        self.assertTrue(('1001', 766) in eleve)

    def test_question_19(self):
        eleve = SQLquery(_P.question_19)
        self.assertEqual(eleve[0], ("10006",))

    def test_question_20(self):
        eleve = SQLquery(_P.question_20)
        self.assertEqual(len(eleve[0]), 3)
        self.assertEqual(len(eleve), 6)

    def test_question_21(self):
        eleve = SQLquery(_P.question_21)
        self.assertEqual(len(eleve[0]), 3)
        self.assertEqual(len(eleve), 19)

    def test_question_22(self):
        eleve = SQLquery(_P.question_22)
        self.assertEqual(len(eleve[0]), 4)
        self.assertEqual(len(eleve), 5)

    def test_question_23(self):
        eleve = SQLquery(_P.question_23)
        self.assertEqual(len(eleve), 9)

    def test_question_24(self):
        eleve = SQLquery(_P.question_24)
        self.assertEqual(len(eleve[0]), 6)

    def test_question_25(self):
        eleve = SQLquery(_P.question_25)
        self.assertEqual(len(eleve[0]), 2)
        self.assertEqual(len(eleve), 1)

    def test_question_26(self):
        eleve = SQLquery(_P.question_26)
        self.assertEqual(len(eleve[0]), 5)

    def test_question_27(self):
        eleve = SQLquery(_P.question_27)
        self.assertEqual(len(eleve[0]), 4)

    def test_question_28(self):
        eleve = SQLquery(_P.question_28)
        self.assertTrue(10000 < eleve[0][0] < 100000)

    def test_question_29(self):
        eleve = SQLquery(_P.question_29)
        self.assertTrue(0 < eleve[0][0] < 10)

    def test_question_30(self):
        eleve = SQLquery(_P.question_30)
        self.assertTrue(10 < eleve[0][0] < 1000)

    def test_question_31(self):
        eleve = SQLquery(_P.question_31)
        self.assertTrue(eleve[0] == ('Paris', 240))

    def test_question_32(self):
        eleve = SQLquery(_P.question_32)
        self.assertTrue(10 < eleve[0][1] < 20)

    def test_question_33(self):
        eleve = SQLquery(_P.question_33)
        self.assertEqual(len(eleve[0]), 4)
        self.assertEqual(len(eleve), 4)

    def test_question_34(self):
        eleve = SQLquery(_P.question_34)
        self.assertEqual(len(eleve[0]), 1)
        self.assertEqual(len(eleve), 1)

    def test_question_35(self):
        eleve = SQLquery(_P.question_35)
        self.assertEqual(len(eleve[0]), 3)
        self.assertEqual(len(eleve), 4)

    def test_question_36(self):
        eleve = SQLquery(_P.question_36)
        eleve = set(list(zip(*eleve))[-1])
        prof = {32, 33, 100, 36, 41, 20, 21, 54, 23, 28}
        self.assertEqual(eleve, prof)
