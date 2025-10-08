# Chip8

#### **CPU**
    - 16 registers (8 bits)
    - I, PC (16 bits)
    - SP
    - Memory

###### Instructions:

- 00E0 – CLS
- 00EE – RET

- 1NNN – JP addr
- 2NNN – CALL addr
- 3XNN – SE Vx, NN
- 4XNN – SNE Vx, NN
- 5XY0 – SE Vx, Vy
- 9XY0 – SNE Vx, Vy
- 
- 6XNN – LD Vx, NN
- 7XNN – ADD Vx, NN
- 8XY0 – LD Vx, Vy
- 8XY1 – OR Vx, Vy
- 8XY2 – AND Vx, Vy
- 8XY3 – XOR Vx, Vy
- 8XY4 – ADD Vx, Vy
- 8XY5 – SUB Vx, Vy	
- 8XY6 – SHR Vx	
- 8XY7 – SUBN Vx, Vy	Vx = Vy - Vx.	
- 8XYE – SHL Vx
- 
- ANNN – LD I, NNN
- BNNN – JP V0, addr
- CXNN – RND Vx, NN
- FX1E – ADD I, Vx
- FX55 – LD [I], Vx
- FX65 – LD Vx, [I]
- 
- FX07 – LD Vx, DT
- FX15 – LD DT, Vx
- FX18 – LD ST, Vx	
- FX0A – LD Vx, K
- 
- DXYN – DRW Vx, Vy, N