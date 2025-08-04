library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity top_level is
  Port ( 
    MAX10_CLK1_50 : in std_logic;
	 KEY	:  IN  STD_LOGIC_VECTOR(1 DOWNTO 0);
    HEX0 : out std_logic_vector(6 downto 0); 
    HEX1 : out std_logic_vector(6 downto 0);
    HEX2 : out std_logic_vector(6 downto 0);
    HEX3 : out std_logic_vector(6 downto 0)
  );
end top_level;

architecture Behavioral of top_level is

	signal rst,clk, pol: std_logic;
	signal Data0 : STD_LOGIC_VECTOR(6 downto 0); 
	signal Data1 : STD_LOGIC_VECTOR(6 downto 0); 
	signal Data2 : STD_LOGIC_VECTOR(6 downto 0); 
	signal Data3 : STD_LOGIC_VECTOR(6 downto 0); 
	signal Aff: std_logic_vector(31 downto 0);

begin

rst <= not KEY(0);
clk <= MAX10_CLK1_50; 
pol <= not KEY(1);
HEX0 <= Data0;
HEX1 <= Data1;
HEX2 <= Data2;
HEX3 <= Data3;

  proc: entity work.processor
    port map (CLK => CLK,RESET => rst,Afficheur => Aff);

  hex_decoder_0 : entity work.hex7seg_decoder
    port map (hex_digit => Aff(3 downto 0),segments => Data0);

  hex_decoder_1 : entity work.hex7seg_decoder
    port map (hex_digit => Aff(7 downto 4),segments => Data1);

  hex_decoder_2 : entity work.hex7seg_decoder
    port map (hex_digit => Aff(11 downto 8),segments => Data2);

  hex_decoder_3 : entity work.hex7seg_decoder
    port map (hex_digit => Aff(15 downto 12),segments => Data3);


end Behavioral;
