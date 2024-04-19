;; ......

	;; Get XSDP
	mov		RAX, [sysTable]
	mov		RCX, [RAX + EfiSystemTable.entryNum]
	dec		RCX
	mov		RAX, [RAX + EfiSystemTable.conf]
	mov		RBX, [EFI_RSDP_GUID]

@@:
	cmp		RBX, [RAX]
	je		@f
	
	add		RAX, EfiConfTable_ENTRY_SZ
	loop	@b
	
@@:
	mov		RAX, [RAX + EfiConfTable.table]
	mov		[xsdp], RAX
	
;; ......
