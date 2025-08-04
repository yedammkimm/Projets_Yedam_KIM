----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/27/2023 10:54:04 AM
-- Design Name: 
-- Module Name: mini_sys - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity mini_sys is
Port ( 
		  clk : in  STD_LOGIC;
		  reset : in std_logic;
		  JB  : inout  STD_LOGIC_VECTOR (7 downto 0); -- Clavier connecté sur le ports de la carte  JB
          an  : out  STD_LOGIC_VECTOR (3 downto 0);   -- Contrôle utilsiation afficheur 
          seg : out  STD_LOGIC_VECTOR (7 downto 0)); --  Pilotage sept_segments
end mini_sys;

architecture Structurelle of mini_sys is

signal Chiffre : STD_LOGIC_VECTOR (3 downto 0);
signal sel0 : std_logic_vector(1 downto 0);
begin

	C0: entity work.Decoder(Behavioral)
	    port map (clk=>clk, Row =>JB(7 downto 4), Col=>JB(3 downto 0), DecodeOut=> Chiffre);
	C1: entity work.Compteur(Behavioral) 
	port map (reset => reset, IN_100 => clk ,sel => sel0); 
	
	C2: entity work.signal_controle(Behavioral)
	port map (clk => clk, compteur => sel0, an => an);    
	
	C3: entity work.Decodeur_7seg(Flot)
	    port map (Valeur=>Chiffre,sept_seg=>seg );



end Structurelle;
