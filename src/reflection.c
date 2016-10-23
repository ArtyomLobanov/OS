#include "reflection.h"
// defualt realization

const char* double_class_prefix = "     ""     ";
const char* default_class_prefix = "     ";
const char* default_struct_prefix = " ";

const char* default_prefix(void* p, int n, int style) {
	if (p == 0 || n < 0) return 0;
	return style == CLASS_STYLE? default_class_prefix : default_struct_prefix;
}


//-----------------memory_map_entry----------------------------------------

const char* memory_map_entry_fields[5] = {_D_UINT"size", _D_ULONG"addr", _D_ULONG"lenght", _D_UINT"type", _D_END};

struct_description_t memory_map_entry_descriptor(memory_map_entry_t* ptr) {
	struct_description_t res =  {(void*)ptr, "memory_map_entry", memory_map_entry_fields, default_prefix};
	return res;
}

// ----------------multiboot_info output settings--------------------------

const char* multiboot_info_fields[26] = {_D_UINT"flags", _D_UINT"mem_lower", _D_UINT"mem_upper", 
	_D_UINT"boot_device", _D_UINT"cmdline",	_D_UINT"mods_count", _D_UINT"mods_addr", _D_COMMENT"", _D_UINT"", _D_UINT"", _D_UINT"", _D_UINT"",
	_D_UINT"mmap_length", _D_UINT"mmap_addr", _D_UINT"drives_length", _D_UINT"drives_addr", _D_UINT"config_table", _D_UINT"boot_loader_name",
	_D_UINT"apm_table", _D_UINT"vbe_control_info", _D_UINT"vbe_mode_info", _D_USHORT"vbe_mode", _D_USHORT"vbe_interface_seg", _D_USHORT"vbe_interface_off",
	_D_USHORT"vbe_interface_len", _D_END};

const char* elf_fields[4] = {" num", " size", " addr", " shndx"};
const char* elf_fields_as_class[4] = {"     ""     ""num", "     ""     ""size", "     ""     ""addr", "     ""     ""shndx"};
const char* sym_fields[4] = {" tabsize", " strsize", " addr", " reserved"};
const char* sym_fields_as_class[4] = {"     ""     ""tabsize", "     ""     ""strsize", "     ""     ""addr", "     ""     ""reserved"};
const char* mes_sym = "--multiboot_aout_symbol_table--"; 
const char* mes_elf = "--multiboot_elf_section_header_table--"; 
const char* mes_def = "--no_header_table--"; 
const char* info_prefix(void* p, int n, int style) {
	multiboot_info_t* ptr = (multiboot_info_t*) p;
	if (n == 7) {
		if (ptr->flags & MULTIBOOT_INFO_AOUT_SYMS) return mes_sym;
		if (ptr->flags & MULTIBOOT_INFO_ELF_SHDR) return mes_elf;
		return mes_def;
	}
	if (8 <= n && n <= 11) {
		if (ptr->flags & MULTIBOOT_INFO_AOUT_SYMS) 
			return style == CLASS_STYLE? sym_fields_as_class[n - 9]:sym_fields[n - 9];
		if (ptr->flags & MULTIBOOT_INFO_ELF_SHDR) 
			return style == CLASS_STYLE? elf_fields_as_class[n - 9]:elf_fields[n - 9];
		return 0;
	}
	if ((ptr->flags & MULTIBOOT_INFO_AOUT_SYMS) | (ptr->flags & MULTIBOOT_INFO_ELF_SHDR))
		return (8 <= n && n <= 11)? default_class_prefix : default_class_prefix;
	return (8 <= n && n <= 11)? 0 : default_prefix(p, n, style);
}

struct_description_t info_descriptor(multiboot_info_t* ptr) {
	struct_description_t res =  {(void*)ptr, "multiboot_info", multiboot_info_fields, info_prefix};
	return res;
}



// ----------------multiboot_header output settings--------------------------

const char* multiboot_header_fields[15] = {_D_UINT"magic", _D_UINT"flags", _D_UINT"checksum", 
	_D_COMMENT"--MULTIBOOT_AOUT_KLUDGE flag is set--", _D_UINT"header_addr", _D_UINT"load_addr", _D_UINT"load_end_addr",
       _D_UINT"bss_end_addr", _D_UINT"entry_addr", _D_COMMENT"--MULTIBOOT_VIDEO_MODE flag is set--", _D_UINT"mode_type",
       _D_UINT"width", _D_UINT"height", _D_UINT"depth", _D_END};

const char* header_prefix(void* p, int n, int style) {
	multiboot_header_t* ptr = (multiboot_header_t*) p;
	if (3 <= n && n < 9) {
		if ((ptr->flags & MULTIBOOT_AOUT_KLUDGE) == 0) return 0;
		return style == CLASS_STYLE? double_class_prefix : default_struct_prefix;
	}
	if (9 <= n && n < 14) {
		if ((ptr->flags & MULTIBOOT_VIDEO_MODE) == 0) return 0;
		return style == CLASS_STYLE? double_class_prefix : default_struct_prefix;
	}
	return default_prefix(p, n, style);
}

struct_description_t header_descriptor(multiboot_header_t* ptr) {
	struct_description_t res =  {(void*)ptr, "multiboot_header", multiboot_header_fields, header_prefix};
	return res;
}
