/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_ELF_CAF_H
#define _LINUX_ELF_CAF_H

#include <linux/elf.h>

/* Generic helpers for ELF use */
/* Return first section header */
static inline struct elf_shdr *elf_sheader(struct elfhdr *hdr)
{
	return (struct elf_shdr *)((size_t)hdr + (size_t)hdr->e_shoff);
}

/* Return idx section header */
static inline struct elf_shdr *elf_section(struct elfhdr *hdr, int idx)
{
	return &elf_sheader(hdr)[idx];
}

/* Return first program header */
static inline struct elf_phdr *elf_pheader(struct elfhdr *hdr)
{
	return (struct elf_phdr *)((size_t)hdr + (size_t)hdr->e_phoff);
}

/* Return idx program header */
static inline struct elf_phdr *elf_program(struct elfhdr *hdr, int idx)
{
	return &elf_pheader(hdr)[idx];
}

/* Retunr section's string table header */
static inline char *elf_str_table(struct elfhdr *hdr)
{
	if (hdr->e_shstrndx == SHN_UNDEF)
		return NULL;
	return (char *)hdr + elf_section(hdr, hdr->e_shstrndx)->sh_offset;
}

#endif /* _LINUX_ELF_CAF_H */
