library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity UAL is
    port (
        A, B : in std_logic_vector(31 downto 0); 
        OP   : in std_logic_vector(2 downto 0);  
        S    : out std_logic_vector(31 downto 0); 
        N, Z, C, V : out std_logic                
    );
end entity UAL;

architecture Behavioral of UAL is
    signal A_int, B_int, S_int : signed(31 downto 0);
    signal Carry_out : std_logic;
begin
    A_int <= signed(A);
    B_int <= signed(B);
    
    process (A_int, B_int, OP)
    begin
        case OP is
            when "000" => -- ADD
                S_int <= A_int + B_int;
                Carry_out <= '0'; 
            when "001" => -- B
                S_int <= B_int;
                Carry_out <= '0';
            when "010" => -- SUB
                S_int <= A_int - B_int;
                Carry_out <= '0';
            when "011" => -- A
                S_int <= A_int;
                Carry_out <= '0';
            when "100" => -- OR
                S_int <= A_int or B_int;
                Carry_out <= '0';
            when "101" => -- AND
                S_int <= A_int and B_int;
                Carry_out <= '0';
            when "110" => -- XOR
                S_int <= A_int xor B_int;
                Carry_out <= '0';
            when "111" => -- NOT
                S_int <= not A_int;
                Carry_out <= '0';
            when others =>
                S_int <= (others => '0');
                Carry_out <= '0';
        end case;
    end process;

	S <= std_logic_vector(S_int);
        N <= '1' when S_int(31) = '1' else '0'; 
        Z <= '1' when S_int = 0 else '0';       
        C <= Carry_out;                         
        V <= '1' when ((A_int(31) = B_int(31)) and (A_int(31) /= S_int(31))) else '0'; 

end architecture Behavioral;

