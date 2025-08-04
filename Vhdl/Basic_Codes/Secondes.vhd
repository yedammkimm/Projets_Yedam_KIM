----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/11/2023 11:39:23 AM
-- Design Name: 
-- Module Name: Secondes - Behavioral
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

entity Secondes is
  Port (IN_100: in std_logic;
  reset: in std_logic;
  sept_seg: out std_logic_vector(7 downto 0);
  mux_out: out std_logic_vector(3 downto 0));
end Secondes;

architecture Behavioral of Secondes is
signal OUT_1 : std_logic;
signal U,D,M,C,OUT_4: std_logic_vector(3 downto 0);
signal Sel: std_logic_vector(1 downto 0);
begin

C0: entity work.Compt_1(Behavioral) 
port map(IN_100 => IN_100,OUT_1 =>OUT_1);

C1: entity work.compteur(Behavioral)
port map(reset => reset,IN_100 => IN_100, Sel => Sel);

C2: entity work.Compt_Codage_BCD(Behavioral)
port map(IN_1 => OUT_1,U => U, D => D, C => C, M => M);

C3: entity work.Mux_1(Behavioral)
port map(Sel => Sel,U => U, D => D, C => C, M => M,OUT_4 => OUT_4);

C4: entity work.mux(Behavioral)
port map(Sel => Sel, mux_out => mux_out);

C5: entity work.Decodeur_7seg(flot)
port map(valeur => OUT_4, sept_seg => sept_seg);

--C6: entity work.Reg7(comport)
--port map(d => sept_seg, clk => IN_100);

--C7: entity work.registre_raz_rau(comport)
--port map(d => mux_out,clk => IN_100);

end Behavioral;
