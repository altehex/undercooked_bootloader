if ~ defined _MEM_MAP_INC_
define _MEM_MAP_INC_	1
	

include "./types.inc"
include "./macro.inc"

	
struc __memRecord {
	.type	I32
	.base	PTR
	.sz		I64
}

;; Memory types
__RAM			=	0			; Everything else, except runtime regions and unusable memory (we don't map them)
__ACPI_TABLES	= 	1	  		; EfiACPIReclaimMemory
__ACPI_NVS		= 	2			; EfiACPIMemoryNVS
__NON_VOLATILE  =   3			; EfiPersistentMemory
__KERNEL        =   4			; For kernel image
__MEMORY_MAP	=   0x50414D4D	; For memory map
	
	
TABLE	__memRecord

	
end if 		; ~ defined _MEM_MAP_INC_
