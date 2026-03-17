; mov 013,13
mov #0, label0
mov #-1, r0
mov r0, r1
mov r0, label0
mov label0, label1
mov label0, r0

; cmp 013,013
; All the valid commands/instruction:
; Some data at start..
X: .string "First String!"
label0: .data -1
label00: .data -1, 1, -2, 78, 90
label89: .string "H e l l o			. We like chars, so let's put some : 	"
.extern label1
XYZ123XYZ: .data 0	 ,  	0 	,	 0  ,  0,	 	0, 	0 	, 	0
.entry XYZ123XYZ

; mov 013,13
mov #0, label0
mov #-1, r0
mov r0, r1
mov r0, label0
mov label0, label1
mov label0, r0

; cmp 013,013
cmp #0, label0
cmp #-1, r0
cmp #9, #-298
cmp r0, r1
cmp r0, label0
cmp r0, #-928
cmp label0, label1
cmp label0, r0
cmp label0, #1294


; add 013,13
add #3957, label00
add #-1, r0
add r2, r3
add r7, label89
add label0, r6

; sub 013,13
sub #3957, label00
sub #-1, r0
sub r2, r3
sub r7, label89
sub label0, r6

; lea 1,13
lea label89, r4

; clr 13
clr r5
clr XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

; not 13
not r6
not X

; inc 13
inc r7
inc X1234YZASFJKFDSA524bsdasfjdgdaf

; dec 13
dec r0


; Put some data here:

ALPHABETAGAMA123: .string "ALPHABETAGAMA123"
.entry ALPHABETAGAMA123

; bne 12
CCC1: bne X
bne CCC1

; jsr 12
C5: jsr X
jsr C5

; red 13
red r4
red label00

; prn 013
prn r5
prn #-32
prn mychars

rts
rts

stop
X1234YZASFJKFDSA524bsdasfjdgdaf: .data 5
.entry X1234YZASFJKFDSA524bsdasfjdgdaf

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX: .string " "
.entry XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
mychars: .string "mychars!@#$%^&#*() 	\/+-=_"
