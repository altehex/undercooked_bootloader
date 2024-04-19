if ~ defined _MACRO_INC_
define _MACRO_INC_ 	1


;; Creates tables of offsets for structures' members 
macro TABLE [name]
{
	forward
	virtual at 0
		name name
	end virtual
}
	

end if ; ~ defined _MACRO_INC_
