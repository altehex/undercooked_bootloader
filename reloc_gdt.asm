GDT_DESC_SZ		equ	16	; Aligned

;; ......

;; Store GDT descriptor
	sgdt	[RAX]
	
;; Copy segment descriptors
	mov		CX, word [RAX]
	sar		RCX, 3
	mov		RSI, [RAX + 2]
	mov		RDI, RAX
	add		RDI, GDT_DESC_SZ
	repe movsq

;; Load new GDT descriptor
	add		RAX, GDT_DESC_SZ
	mov		[RAX + 2 - GDT_DESC_SZ], RAX
	lgdt	[RAX - GDT_DESC_SZ]

;; Pass the IDT base in RAX to int.asm
	add		AX, word [RAX - GDT_DESC_SZ]
	add		RAX, GDT_DESC_SZ + 7
	and		AL, 0xF8
	
;; ......
