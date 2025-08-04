library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_Assemblage_unite is
end tb_Assemblage_unite;

architecture Behavioral of tb_Assemblage_unite is
    signal CLK : std_logic := '0';
    signal RESET : std_logic := '0';
    signal WE : std_logic := '0';
    signal COM : std_logic := '0';
    signal COM2 : std_logic := '0';
    signal Reg_WE : std_logic := '0';
    signal WrEn : std_logic := '0';
    signal IMM : std_logic_vector(7 downto 0) := (others => '0');
    signal RA, RB, RW : std_logic_vector(3 downto 0) := (others => '0');
    signal W : std_logic_vector(31 downto 0) := (others => '0');
    signal ALU_OP : std_logic_vector(2 downto 0) := (others => '0');
    constant CLK_period : time := 10 ns;

begin
    Asse: entity work.Assemblage_unite_trait(Behavioral)
port map ( CLK => CLK, RESET => RESET, WE => WE, COM => COM, COM2 => COM2, Reg_WE => Reg_WE, WrEn => WrEn, IMM => IMM, RA => RA, RB => RB, RW => RW, W => W, ALU_OP => ALU_OP );

    CLK_process :process
    begin
        CLK <= '0';
        wait for CLK_period/2;
        CLK <= '1';
        wait for CLK_period/2;
    end process;

    proc: process
    begin		

        RESET <= '1';
        wait for CLK_period*2;
        RESET <= '0';

        Reg_WE <= '1';
        RW <= "0001"; 
        W <= X"00000010"; 
        wait for CLK_period;

        RW <= "0010"; 
        W <= X"00000020"; 
        wait for CLK_period;
        Reg_WE <= '0';

        -- Addition of two registers
        RA <= "0001"; 
        RB <= "0010"; 
        ALU_OP <= "000"; 
        WE <= '1';
        Reg_WE <= '1';
        RW <= "0011"; 
        wait for CLK_period;
        WE <= '0';
        Reg_WE <= '0';

        -- Addition of one register with an immediate value
        RA <= "0011"; 
        IMM <= X"05"; 
        COM <= '1'; 
        WE <= '1';
        Reg_WE <= '1';
        RW <= "0100"; 
        wait for CLK_period;
        WE <= '0';
        Reg_WE <= '0';
        COM <= '0';

        -- Subtraction of two registers
        RA <= "0011"; 
        RB <= "0001"; 
        ALU_OP <= "010"; 
        WE <= '1';
        Reg_WE <= '1';
        RW <= "0101"; 
        wait for CLK_period;
        WE <= '0';
        Reg_WE <= '0';

        --Subtraction of an immediate value from a register
        RA <= "0011"; 
        IMM <= X"03"; 
        COM <= '1'; 
        ALU_OP <= "010";
        WE <= '1';
        Reg_WE <= '1';
        RW <= "0110"; 
        wait for CLK_period;
        WE <= '0';
        Reg_WE <= '0';
        COM <= '0';

        -- Copying the value of one register into another
        RA <= "0011"; 
        WE <= '1';
        Reg_WE <= '1';
        RW <= "0111"; 
        wait for CLK_period;
        WE <= '0';
        Reg_WE <= '0';

        -- Writing a register value to a memory word
        RB <= "0100"; 
        ALU_OP <= "000"; 
        WrEn <= '1';
        wait for CLK_period;
        WrEn <= '0';

        -- Reading a word from memory into a register
        COM2 <= '1'; 
        RA <= "0100"; 
        WE <= '1';
        Reg_WE <= '1';
        RW <= "1000"; 
        wait for CLK_period;
        WE <= '0';
        Reg_WE <= '0';
        COM2 <= '0';

        wait;
    end process;

end Behavioral;

