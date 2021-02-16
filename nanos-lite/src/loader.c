#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  extern uint8_t ramdisk_start;
  extern uint8_t ramdisk_end;
  Elf_Ehdr *elf_ehdr = NULL;
  Elf_Phdr *elf_phdr = NULL;
  void *base = &ramdisk_start;
  elf_ehdr = (Elf_Ehdr*)base;
  elf_phdr = (Elf_Phdr*)&base[elf_ehdr->e_phoff];

  int i;
  for (i = 0; i < elf_ehdr->e_phnum; i++) {
    if (elf_phdr[i].p_type == PT_LOAD) {
      // need to be loaded
      ramdisk_read(elf_phdr[i].p_vaddr, elf_phdr[i].p_offset, elf_phdr[i].p_filesz);
      memset(elf_phdr[i].p_vaddr + elf_phdr[i].p_filesz, 0, elf_phdr[i].p_memsz - elf_phdr[i].p_filesz);
    }
  }
  return elf_ehdr->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

