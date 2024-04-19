PAGE_FLAGS_PML4	equ	00000111b
PAGE_FLAGS_PDP	equ	10000111b

;; Stolen from TempleOS by Terry A. Davis (Mem/PageTables.HC). Rest in power
	
;; R15: page size
;; R14: total RAM
;; R13: additional counter register (see below how it's used)

;; Calculate total memory
	mov		RAX, [memMapBase]
	mov		RBX, RAX
	mov		RAX, [RAX + 4 + 8]
	mov		R14, [RBX + RAX - 8]
	add		R14, [RBX + RAX - 8 - 8]

;; Set page size (1 GB if supported)
	mov		EAX, 0x80000001
	cpuid
	mov		R15, 0x200000	; 2 MB
	bt		EDX, 26	; 1 << 26
	jnc		@f
	sal		R15, 9			; 1 GB
	
;; PML4
@@:
	mov		RAX, [pml4Base]
	mov		RDI, RAX
	and		AX, 0xF000
	or		AL, PAGE_FLAGS_PML4
	
	mov		RCX, R14
	shr		RCX, 30 + 5
	cmp		RCX, 1023	; The number of entries is limited to 1024
	;; It most likely will not exceed 1-2 entries, but what if?
	;; What if some dumbass gonna fire up this baby on a supercomputer.
	jbe		@f
	mov		RCX, 1023
@@:
	mov		R13, RCX	; We're gonna need the number of entries later
@@:
	add		RAX, 0x1000
	mov		[RDI], RAX

	add		RDI, 8
	loope	@b

	mov		RAX, [pml4Base]	; It's gonna iterate through PML4 entries
	sar		R14, 30 ; How many PDP entries will be created (1 per 1 GB)
	dec		R14
	mov		RBX, PAGE_FLAGS_PDP
	bt		R15, 30	
	jc		pdp_1gb

;; PDP and PD (2 MB)
	
	;; WIP
	
	jmp		done
	
;; PDP (1 GB)
pdp_1gb:
	mov		RDI, [RAX + R13 * 8]
	and		DI, 0xF000

	mov		RCX, 1024
	cmp		R14, 1024
	cmovl	RCX, R14
@@:
	mov		[RDI], RBX
	
	add		RBX, R15
	add		RDI, 8
	loop	@b

	sub		R14, 1024
	dec		R13
	cmp		R13, 0
	jge		pdp_1gb	
	
done:	
	
