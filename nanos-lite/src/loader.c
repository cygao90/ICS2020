#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  int fd = fs_open(filename, 0, 0);
  assert(fd != -1);
  Log("open: %s, fd is %d", filename, fd);
  Elf_Ehdr *elf_ehdr = NULL;
  Elf_Phdr *elf_phdr = NULL;
  size_t size = fs_lseek(fd, 0, SEEK_END);
  void *base = malloc(size);
  fs_lseek(fd, 0, SEEK_SET);
  fs_read(fd, base, size);
  elf_ehdr = (Elf_Ehdr*)base;
  elf_phdr = (Elf_Phdr*)&base[elf_ehdr->e_phoff];

  int i;
  for (i = 0; i < elf_ehdr->e_phnum; i++) {
    if (elf_phdr[i].p_type == PT_LOAD) {
      memcpy(elf_phdr[i].p_vaddr, base + elf_phdr[i].p_offset, elf_phdr[i].p_filesz);
      memset(elf_phdr[i].p_vaddr + elf_phdr[i].p_filesz, 0, elf_phdr[i].p_memsz - elf_phdr[i].p_filesz);
    }
  }
  uintptr_t entry = elf_ehdr->e_entry;
  free(base);
  fs_close(fd);
  return entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

