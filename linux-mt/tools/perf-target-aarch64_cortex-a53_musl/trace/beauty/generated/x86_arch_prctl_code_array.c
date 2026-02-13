#define x86_arch_prctl_codes_1_offset 0x1001
static const char *x86_arch_prctl_codes_1[] = {
	[0x1001 - 0x1001]= "SET_GS",
	[0x1002 - 0x1001]= "SET_FS",
	[0x1003 - 0x1001]= "GET_FS",
	[0x1004 - 0x1001]= "GET_GS",
	[0x1011 - 0x1001]= "GET_CPUID",
	[0x1012 - 0x1001]= "SET_CPUID",
	[0x1021 - 0x1001]= "GET_XCOMP_SUPP",
	[0x1022 - 0x1001]= "GET_XCOMP_PERM",
	[0x1023 - 0x1001]= "REQ_XCOMP_PERM",
	[0x1024 - 0x1001]= "GET_XCOMP_GUEST_PERM",
	[0x1025 - 0x1001]= "REQ_XCOMP_GUEST_PERM",
};

#define x86_arch_prctl_codes_2_offset 0x2001
static const char *x86_arch_prctl_codes_2[] = {
	[0x2001 - 0x2001]= "MAP_VDSO_X32",
	[0x2002 - 0x2001]= "MAP_VDSO_32",
	[0x2003 - 0x2001]= "MAP_VDSO_64",
};

#define x86_arch_prctl_codes_3_offset 0x4001
static const char *x86_arch_prctl_codes_3[] = {
	[0x4001 - 0x4001]= "GET_UNTAG_MASK",
	[0x4002 - 0x4001]= "ENABLE_TAGGED_ADDR",
	[0x4003 - 0x4001]= "GET_MAX_TAG_BITS",
	[0x4004 - 0x4001]= "FORCE_TAGGED_SVA",
};

