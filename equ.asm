L1: EQU 1
L2: EQU 0
SECAO TEXTO
IF L1
LOAD N1 ;faz esta operação se L1 for verdadeiro
IF L2
INPUT N2 ;faz esta operação se L2 for verdadeiro
ROT: INPUT N1
COPY N1, N4 ;comentário qualquer
COPY N2, N3
COPY N3, N3
OUTPUT N3
SECAO DADOS
N2: CONST 0x48
N4: SPACE
N3: SPACE
N1: SPACE
