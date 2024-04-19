if ~ defined _UEFI_INC_
define _UEFI_INC_ 1
	
;; Macros
;;-------------------------------------

include "./types.inc"
include "./macro.inc"
	
	
EfiVirtAddr	fix I64
EfiPhysAddr	fix I64
	

define _INTERFACES 	conInHdl,			\
					EfiTextIn,			\
					conOutHdl,			\
					EfiTextOut,			\
					stdErrHdl,			\
					stdErr,				\
					EfiRuntimeServices,	\
					EfiBootServices


	
macro __eficall interface*,	\
				func*, 		\
 				a1, a2, a3, a4, [a]
{
	common
	if ~ a4 eq
		if ~ a4 eq R9
			mov		R9, a4
		end if
	end if

	if ~ a3 eq
		if ~ a3 eq R8
			mov		R8, a3
		end if
	end if

	if ~ a2 eq
		if ~ a2 eq RDX
			mov		RDX, a2
		end if
	end if

	if interface in	<_INTERFACES>
	 	mov		RAX, [__sysTable]
		mov		RAX, [RAX + EfiSystemTable.#interface]
	else
		if ~ interface eq RBX
			mov		RAX, [interface]
		end if
	end if

	if ~ a1 eq
		if a1 in <_INTERFACES> | a1 eq interface
			mov		RCX, RAX
		else
			mov		RCX, a1
		end if
	end if

	mov		RAX, [RAX + _#interface#.#func]

	if ~ a eq
		mov		[return], RSP
		and		SPL, 0xF0
	end if

	reverse
	if ~ a eq
		push	a
	end if
	common
	
	call	RAX
	
	if ~ a eq
		mov		RSP, [return]
	end if
}
	
macro EFI_INIT	_imgHandle,	\
				_sysTable
{
	clc
	or		RDX, RDX
	jz		.err
	cmp		dword [RDX], EFI_SYSTEM_TABLE_SIGNATURE
	je		@f
.err:
	xor		RCX, RCX
	xor		RDX, RDX
	stc
@@:
	mov		[_imgHandle], RCX
	
	mov		[_sysTable],  RDX
__sysTable = _sysTable
}

	
;; Equates
;;-------------------------------------*
	
EFI_SUCCESS				=		0 
EFI_LOAD_ERROR			=		1
EFI_INVALID_PARAMETER	=		2
EFI_UNSUPPORTED			=		3

EFI_BUFFER_TOO_SMALL	=		5
	
EFI_DEVICE_ERROR		= 		7
EFI_WRITE_PROTECTED		=		8
EFI_OUT_OF_RESOURCES	=		9
EFI_VOLUME_CORRUPTED	=		10
EFI_VOLUME_FULL			=		11
EFI_NO_MEDIA			=		12
EFI_MEDIA_CHANGED		=		13
EFI_NOT_FOUND			=		14
EFI_ACCESS_DENIED		=		15
	
EFI_SYSTEM_TABLE_SIGNATURE	= 0x20494249

	
;; Structs
;;--------------------------------------*

;; Interfaces
struc EfiTableHeader {
	.sig					I64
	.rev					I32
	.hdrSz					I32
	.crc32					I32
	._res					I32
}
	
struc EfiSystemTable {
	.hdr					EfiTableHeader
	.vendor					PTR
	.rev					I32
	.conInHdl				PTR
	.EfiTextIn				PTR
	.conOutHdl				PTR
	.EfiTextOut				PTR
	.stdErrHdl				PTR
	.stdErr					PTR
	.EfiRuntimeServices		PTR
	.EfiBootServices		PTR
	.entryNum				IN
	.conf					PTR
}

struc _EfiBootServices {
	.hdr					EfiTableHeader
	.raise_tpl				PTR
	.restore_tpl			PTR
	.alloc_pages			PTR
	.free_pages				PTR
	.get_memmap				PTR
	.alloc_pool				PTR
	.free_pool				PTR
	.create_event			PTR
	.set_timer				PTR
	.wait_for_event			PTR
	.sig_event				PTR
	.close_event			PTR
	.check_event			PTR
	.install_interface		PTR
	.reinstall_interface	PTR
	.uninstall_interface	PTR
	.hdl_protocol			PTR
	._res					PTR
	.reg_protocol_notify	PTR
	.locate_hdl				PTR
	.locate_dev_path		PTR
	.install_conf_table		PTR
	.load_img				PTR
	.start_img				PTR
	.exit					PTR
	.unload_img				PTR
	.exit_bs				PTR
	.get_next_mono_count	PTR
	.stall					PTR
	.set_watchdog_timer		PTR
	.connect_controller		PTR
	.disconnect_controller	PTR
	.open_protocol			PTR
	.close_protocol			PTR
	.open_protocol_info		PTR
	.protocols_per_hdl		PTR
	.locate_hdl_buf			PTR
	.locate_protocol		PTR
	.install_mul_protocols	PTR
	.uninstall_mul_protocols	PTR
	.calculate_crc32		PTR
	.copy_mem				PTR
	.set_mem				PTR
	.create_event_EX		PTR
}

struc _EfiTextOut {
	.reset				PTR
	.output_string		PTR
	.test_string		PTR
	.query_mode			PTR
	.set_mode			PTR
	.set_attr			PTR
	.clear_scr			PTR
}

struc _EfiFileSystem {
	.rev			I64
	.open_vol		PTR
}

EFI_FILE_MODE_READ	equ		0x0000000000000001
	
struc _EfiFile {
	.rev			I64
	.open			PTR
	.close			PTR
	.delete			PTR
	.read			PTR
}

struc _EfiLoadedImg {
	.rev			I32
	.parentHdl		PTR
	.sysTable		PTR
	.devHdl			PTR
	.filePath		PTR
	._res			PTR
}

struc _EfiVideoOut {
	.query_mode		PTR
	.set_mode		PTR
	.blt			PTR
	.mode			PTR			; EfiVideoOutMode
}

struc _EfiMP {
	.get_core_num		PTR
	.get_proc_info		PTR
	.start_all_aps		PTR
	.start_this_ap		PTR
	.switch_bsp			PTR
	.enable_disable_ap	PTR
	.who_am_i			PTR
}
	
;; Data structures
struc EfiConfTable {
	.guid_q0		I64
	.guid_q1		I64
	.table			PTR
}
EfiConfTable_ENTRY_SZ	equ	8 * 3
	
struc EfiMemoryDescriptor {
	.type			I32
	.physStart		EfiPhysAddr
	.virtStart		EfiVirtAddr
	.numOfPages		I64
	.attrs			I64
}
	
struc EfiVideoOutMode {
	.maxMode		I32
	.mode			I32
	.info			PTR			; EfiVideoOutModeinfo
	.infoSz			IN
	.fbBase			EfiPhysAddr
	.fbSz			IN
}

struc EfiVideoOutModeInfo {
	.ver			I32
	.hRes			I32
	.vRes			I32
	.pixFmt			I32
	.pixInfo		db	4
	.pixPerLine		I32
}

struc EfiCpuPhysLocation {
	.package		I32
	.core			I32
	.thread			I32
}
	
struc EfiProcInfo {
	.id				I64
	.status			I32
	;; Don't need next fieds
}

;; UUIDs
macro _EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID
{
	dd	0x9042A9DE
	dw	0x23DC, 0x4A38
	db  0x96, 0xFB, 0x7A, 0xDE, 0xD0, 0x80, 0x51, 0x6A
}

macro _EFI_LOADED_IMAGE_PROTOCOL_GUID
{
	dd	0x5B1B31A1
	dw	0x9562, 0x11D2
	db	0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B
}
	
macro _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID
{
	dd	0x964E5B22
	dw	0x6459, 0x11D2
	db  0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B
}

macro _EFI_MP_SERVICES_PROTOCOL_GUID
{
	dd	0x3FDDA605
	dw	0xA76E, 0x4F46
	db	0xAD, 0x29, 0x12, 0xF4, 0x53, 0x1B, 0x3D, 0x08
}

	
macro _EFI_ACPI_TABLE_GUID
{
	dd	0x8868E871
	dw	0xE4F1, 0x11D3
	db	0xBC, 0x22, 0x00, 0x80, 0xC7, 0x3C, 0x88, 0x81
}

	
;; More equates
;;---------------------------------*

NULL	=	0

TRUE	=	1

TPL_NOTIFY	=	16
	
;; EFI_GRAPHICS_PIXEL_FORMAT
PixelRedGreenBlueReserved8BitPerColor	equ		0
PixelBlueGreenRedReserved8BitPerColor	equ		1

;; EFI_ALLOCATE_TYPE
EFI_ALLOCATE_ANY_PAGES		equ		0

;; EFI_MEMORY_TYPE
EFI_RESERVED_MEM	=	 0	
EFI_LOADER_CODE		=	 1
EFI_LOADER_DATA		=	 2
EFI_BS_CODE			=	 3
EFI_BS_DATA			=	 4
EFI_RTS_CODE		=	 5
EFI_RTS_DATA		=	 6
EFI_FREE			=	 7
EFI_UNUSABLE		=	 8
EFI_ACPI_RECLAIM	=	 9
EFI_ACPI_NVS		=	10
EFI_IO				=	11
EFI_IO_PORTS		=	12
EFI_PAL_CODE		=	13
EFI_PERSISTENT		=	14
	

TABLE	\
	EfiSystemTable,			\
	_EfiBootServices,		\
	_EfiTextOut,			\
	_EfiFileSystem,			\
	_EfiFile,				\
	_EfiLoadedImg,			\
	_EfiVideoOut,			\
	_EfiMP,					\
	EfiConfTable,			\
	EfiMemoryDescriptor,	\
	EfiVideoOutMode,		\
	EfiVideoOutModeInfo,	\
	EfiProcInfo
	
	
end if	; ~_UEFI_INC_
