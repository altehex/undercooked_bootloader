if ~ defined _TYPES_INC_
define _TYPES_INC_ 1

struc I8 data
{
	if ~ data eq
	   	. db data
	else
		. db ?
	end if
}

struc I16 data
{
	align 2
	
	if ~ data eq
	   	. dw data
	else
		. dw ?
	end if
}

struc I32 data
{
	align 4
	
	if ~ data eq
	    . dd data
	else
		. dd ?
	end if
}

struc I64 data
{
	align 8
	
	if ~ data eq
	    . dq data
	else
		. dq ?
	end if
}

IN		fix I64
PTR		fix	I64

end if	; ~_TYPES_INC_
