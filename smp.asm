;; ......
	
	xor		RDX, RDX
	__eficall	EfiBootServices, locate_protocol,	\
				EFI_MP_SERVICES_PROTOCOL_GUID, RDX, \
				EfiMP

	__eficall	EfiMP, get_core_num,				\
				EfiMP, corenum, 	\
				activeCoreNum

	xor		RCX, RCX
	xor		R8, R8
	xor		R9, R9
	__eficall	EfiBootServices, create_event,	\
				RCX, TPL_NOTIFY, R8, R9, 		\
				_event

	xor		RBX, RBX
	cmp		[corenum], PAGE_SZ / 4
	jbe		startup_cores
	mov		[corenum], PAGE_SZ / 4
startup_cores:
	__eficall	EfiMP, get_proc_info,	\
				EfiMP, RBX, procInfo
	
	bt		[procInfo + EfiProcInfo.status], 0
	jc		@f

	__eficall	EfiMP, start_this_ap,	\
				EfiMP, core_init, RBX, 	\
				[_event], 0, _arg, NULL
	
@@:
	inc		BX
	cmp		word [corenum], BX
	jne		startup_cores
	
;; ......
