# Edit these as you wish
BOOT_DIR   = filesys/efi/boot
IMG_PATH   = \"\\efi\\$(IMG_NAME)\"
STACK_SIZE = 1024
IMG_BASE   = 0x100000
IMG_SIZE   = 4096

FASM_DEFINES = -dIMG_BASE=$(IMG_BASE) \
			   -dIMG_PATH=$(IMG_PATH) \
			   -dIMG_SIZE=$(IMG_SIZE) \
			   -dSTACK_SIZE=$(STACK_SIZE)

.PHONY := all
all: bootx64.efi

%.efi: %.asm
	fasm $(FASM_DEFINES) $<
	mkdir -p $(BOOT_DIR)
	cp $@ $(BOOT_DIR)

CLEAN = *.efi *.fas

.PHONY += clean
clean:
	$(RM) $(CLEAN)
