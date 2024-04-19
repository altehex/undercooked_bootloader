if ~ defined _ACPI_INC_
define _ACPI_INC_ 1

include "./types.inc"
include "./macro.inc"


struc rsdp {
	.sig			I64
	.checksum		I8
	.oemId_1		rb	6
	.rev			I8
	.rsdt			I32	
	.sz				I32
	.xsdt			PTR
	.checksum_ext	I8
}

struc xsdt {
	.sig			I32
	.sz				I32
	.rev			I8
	.checksum		I8
	.oemId			rb	6
	.oemTableId		I64
	.oemRev			I32
	.creatorId		I32
	.creatorRev		I32
	.entry			PTR
}


TABLE	\
	rsdp,	\
	xsdt

	
end if 	; ~ defined _ACPI_INC_
