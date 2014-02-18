#c't Hacks GRBL-Jogger PCB

**Printed Cicuit Board for GRBL 0.8c/j with ATmega644**

Note: "View Raw" to download PDFs. Gerber Files for PCB production. PCB artwork (c) by Carsten Meyer, c't Hacks editorial office, Heise Zeitschriften Verlag 2014.

Double-sided PCB with silk position print available at 
[ct.eurocircuits.de](http://ct.eurocircuits.de/ "c't Hacks Eurocircuits Shop")

GRBL-Jogger
===========

Bill of Materials
Project Path :  E:\GRBL_644\PCB\GRBL_JOG.prj 
Last Saved :  10.02.2014 00:50:49 

Component                     Value/Description          Qty   Name
-------------------------------------------------------------------
2JP                           Pfostenleiste 1-reihig       3   JP2 JP1 JP3         
3JP                           Pfostenleiste 1-reihig       3   JP6 JP5 JP4         
6WP                           Pfostenleiste 1-reihig       1   PL6                 
10WP                          (R) WSL10G, (S) AWHW 10      1   PL7                 
16WP                          (R) WSL16G, (S) AWHW 16      2   PL1 PL5             
17SIL                         Pfostenleiste 1-reihig       2   PL10 PL9            
26WP                          (R) WSL26G, (S) AWHW 26      1   PL8                 
ATmega644 DIL                                              1   U2                  
BC337                                                      2   Q1 Q2               
C                             22n                          4   C18 C17 C15 C16     
C                             22p                          2   C13 C11             
C                             100n                         5   C3 C8 C12 C6 C10    
C                             100n 300V AC                 2   C1 C2               
CP1                           10µ 50V                      2   C7 C9               
CP1                           100µ 16V/25V                 1   C14                 
CP4                           220µ 50V                     1   C4                  
CP4                           1000µ 16V                    1   C5                  
1N4148                                                     4   D1 D2 D4 D5    
UF4004                                                     1   D3
1N4007                                                     2   D6 D7
D25F            (R) D-SUB 25 BU 25EU, (S) DS25F-90°-10mm   1   CONN1               
DC-Koax ROKA                  (S) DCBU 2,1-PR              1   PL11                
L 330µH 0,8A                  (R) L-11PHC 330µ             1   L1                  
LED                           3 mm rot/grün/gelb           6   LED3 LED1 LED2 LED4 LED5 LED6
LM2574 N5 DIL                                              1   U1                  
Phoenix_2                     2pol RM5.08                  3   PL3 PL4 PL2         
R                             0R**                         1   R8                  
R                             1k                           2   R15 R11             
R                             1k8* entfällt                1   R12                 
R                             4k7                          1   R39                 
R                             5k6* entfällt                1   R10                 
R                             22k                          1   R4                  
R                             100R                        10   R13 R29 R30 R31 R32 R33 R34 R35 R6 R7
R                             220R                        17   R1 R3 R2 R16 R18 R27 R26 R24 R28 R25 R22 R21 R20 R19 R17 R14 R36
R                             470R                         3   R40 R38 R37         
R8N                           8x4k7 SIL                    3   RN3 RN2 RN1         
Relais 1pol 10A                                            2   SW12 SW13          
R_POT                         10k B ALPS RK11K113-LOG10K   1   P1               
R_PRE 5x10mm                  2k5* entfällt                1   R9                  
Taster Multimec 3ATL6 mit Kappe 1ZBxx                      4   SW7 SW4 SW2 SW6
Taster Multimec 3FTL6 mit Kappe 1ZCxx                      7   SW5
Taster Multimec 3FTL6 mit Kappe 1Dxx                       7   SW11 SW10 SW1 SW8 SW3 SW9
XTAL                          16 MHz HC49U                 1   XTAL1               
-------------------------------------------------------------------
                                                         112

Wichtige Hinweise
=================

Bei Montage TFT-Maximite 4x Abstandsbolzen M3 x 30 mm
Bauteile mit "*"  nur erforderlich bei LM2574 N-ADJ, entfallen bei LM2574 N5
Bauteile mit "**" (R8 = Drahtbrücke) nur erforderlich bei LM2574 N5, entfallen bei LM2574 N-ADJ
Relais 10A z.B. Reichelt HJR3FF 24VDC SZ oder HJR3FF 12VDC SZ, Segor F3611-12 oder F3611-24, Spannung je nach Versorgung
Anschlussklemmen z.B. Reichelt AKL 249-02 und AKL 220-02, Segor ARK 2/Stift und ARK 2/SteckV
Multimec-Taster: Kappenfarbe (xx) nach Wahl, 03=grau, 06=weiß, 09=schwarz, 30=blau
D-Sub-Buchse Euro-Standard (R) D-SUB 25 BU 25EU, (S) DS25F-90°-10mm
Bei Verwendung eines ATmega644P ist ein hierfür kompiliertes HEX-File nötig!
