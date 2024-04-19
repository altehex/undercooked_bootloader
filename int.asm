CLI_HLT		equ 0xf4fa	; "cli; hlt"
GEN_INT_SZ  equ 2

GATE_SZ		equ	16
IDT_DESC_SZ	equ 16	; Aligned
IDT_SZ		equ 256 * GATE_SZ

GATE_ATTR   equ 0x8E0000000000

;; ......
	
;; Load first 32 vectors
	;; mov		RAX, [idtBase] <-- it is already loaded (see mem_map.asm)

	;; Load general interrupt handler
	mov		RBX, RAX
	add		RBX, IDT_SZ + IDT_DESC_SZ
	mov		word [RBX], CLI_HLT
	
	;; Gate attributes
	mov		RDX, GATE_ATTR
	;; Offset low
	mov		RCX, RBX
	and		RCX, 0x0FFFF
	or		RDX, RCX
	;; Offset mid
	sar		RBX, 8
	and		RBX, 0x0FFFF000
	sal		RBX, 40
	or		RDX, RBX
	;; Code segment selector
	mov		RBX, CS
	sal		RBX, 16
	or		RDX, RBX
	;; No need to set high 64 bits

	mov		RBX, RAX
	add		RBX, IDT_DESC_SZ	; Reserved for IDT descriptor
	mov		RCX, 32
@@:
	mov		RDI, RCX
	shl		RDI, 4
	add		RDI, RBX
	mov		[RDI - GATE_SZ], RDX 
	loop	@b

;; Create IDT descriptor and load it to IDTR
	mov		RBX, RAX
	sal		RBX, 16
	or		BX, IDT_SZ - 1
	mov		qword [RAX], RBX
	lidt	[RAX]
	
	add		RAX, IDT_SZ + IDT_DESC_SZ + GEN_INT_SZ + 0xFFF
	and 	AX, 0xF000
	mov		[pml4Base], RAX
	
;; ......
