----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/15/2023 01:57:26 PM
-- Design Name: 
-- Module Name: Codeur - Behavioral
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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Codeur is
    Port (
clk : in  STD_LOGIC;
          Row : in  STD_LOGIC_VECTOR (3 downto 0);
          Col : out  STD_LOGIC_VECTOR (3 downto 0);
          Chiffre : out  STD_LOGIC_VECTOR (3 downto 0));
end Codeur;

 architecture Behavioral of Codeur is

signal sclk :STD_LOGIC_VECTOR(19 downto 0) := "00000000000000000000";

begin
	process(clk)
		begin 
		if clk'event and clk = '1' then
			-- 1ms
			if sclk = "00011000011010100000" then 
				--C1
				Col<= "1110";
				sclk <= sclk+1;
			-- check row pins
			elsif sclk = "00011000011010101000" then	
				--R1
				if Row = "1110" then
					Chiffre <= "0001";	--1
				--R2
				elsif Row = "1101" then
					Chiffre <= "0100"; --4
				--R3
				elsif Row = "1011" then
					Chiffre <= "0111"; --7
				--R4
				elsif Row = "0111" then
					Chiffre <= "0000"; --0
				end if;
				sclk <= sclk+1;
			-- 2ms
			elsif sclk = "110000110101000000" then	
				--C2
				Col<= "1101";
				sclk <= sclk+1;
			-- check row pins
			elsif sclk = "110000110101001000" then	
				--R1
				if Row = "1110" then		
					Chiffre <= "0010"; --2
				--R2
				elsif Row = "1101" then
					Chiffre <= "0101"; --5
				--R3
				elsif Row = "1011" then
					Chiffre <= "1000"; --8
				--R4
				elsif Row = "0111" then
					Chiffre <= "1111"; --F
				end if;
				sclk <= sclk+1;	
			--3ms
			elsif sclk = "1001001001111100000" then 
				--C3
				Col<= "1011";
				sclk <= sclk+1;
			-- check row pins
			elsif sclk = "1001001001111101000" then 
				--R1
				if Row = "1110" then
					Chiffre <= "0011"; --3	
				--R2
				elsif Row = "1101" then
					Chiffre <= "0110"; --6
				--R3
				elsif Row = "1011" then
					Chiffre <= "1001"; --9
				--R4
				elsif Row = "0111" then
					Chiffre <= "1110"; --E
				end if;
				sclk <= sclk+1;
			--4ms
			elsif sclk = "01100001101010000000" then 			
				--C4
				Col<= "0111";
				sclk <= sclk+1;
			-- check row pins
			elsif sclk = "01100001101010001000" then 
				--R1
				if Row = "1110" then
					Chiffre <= "1010"; --A
				--R2
				elsif Row = "1101" then
					Chiffre <= "1011"; --B
				--R3
				elsif Row = "1011" then
					Chiffre <= "1100"; --C
				--R4
				elsif Row = "0111" then
					Chiffre <= "1101"; --D
				end if;
				sclk <= "00000000000000000000";	
			else
				sclk <= sclk+1;	
			end if;
		end if;
	end process;
		
		
						 
end Behavioral;