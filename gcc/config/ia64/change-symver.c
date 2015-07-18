#define _GNU_SOURCE 1
#define _FILE_OFFSET_BITS 64
#include <endian.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <gelf.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
compute_veridx (const char *name, Elf *elf, Elf_Data *verd, GElf_Shdr *verd_shdr)
{
  if (strcmp (name, "_GLOBAL_") == 0)
    return 1;

  int cnt;
  size_t offset = 0;
  for (cnt = verd_shdr->sh_info; --cnt >= 0; )
    {
      GElf_Verdef defmem;
      GElf_Verdef *def;
      GElf_Verdaux auxmem;
      GElf_Verdaux *aux;
      unsigned int auxoffset;

      /* Get the data at the next offset.  */
      def = gelf_getverdef (verd, offset, &defmem);
      if (def == NULL)
	break;

      auxoffset = offset + def->vd_aux;
      aux = gelf_getverdaux (verd, auxoffset, &auxmem);
      if (aux == NULL)
	break;

      if (strcmp (name, elf_strptr (elf, verd_shdr->sh_link,
		  aux->vda_name)) == 0)
	return def->vd_ndx;

      /* Find the next offset.  */
      offset += def->vd_next;
    }

  return -1;
}

int
main (int argc, char **argv)
{
  if (argc < 4)
    error (1, 0, "Usage: change_symver library from_symver to_symver symbol...\nExample: change_symver libfoo.so FOO_1.0 *global* bar baz");

  const char *fname = argv[1];

  /* Open the file.  */
  int fd;
  fd = open (fname, O_RDWR);
  if (fd == -1)
    error (1, errno, fname);

  elf_version (EV_CURRENT);

  /* Now get the ELF descriptor.  */
  Elf *elf = elf_begin (fd, ELF_C_READ_MMAP, NULL);
  if (elf == NULL || elf_kind (elf) != ELF_K_ELF)
    error (1, 0, "Couldn't open %s: %s", fname, elf_errmsg (-1));

  size_t shstrndx;
  /* Get the section header string table index.  */
  if (elf_getshstrndx (elf, &shstrndx) < 0)
    error (1, 0, "cannot get shstrndx from %s", fname);

  GElf_Ehdr ehdr_mem;
  GElf_Ehdr *ehdr;

  /* We need the ELF header in a few places.  */
  ehdr = gelf_getehdr (elf, &ehdr_mem);
  if (ehdr == NULL)
    error (1, 0, "couldn't get ELF headers %s: %s", fname, elf_errmsg (-1));

  Elf_Scn *scn = NULL;
  GElf_Shdr shdr_mem, verd_shdr, ver_shdr, dynsym_shdr;
  Elf_Data *ver = NULL, *verd = NULL, *dynsym = NULL;

  while ((scn = elf_nextscn (elf, scn)) != NULL)
    {
      GElf_Shdr *shdr = gelf_getshdr (scn, &shdr_mem);

      if (shdr == NULL)
	error (1, 0, "couldn't get shdr from %s", fname);

      if ((shdr->sh_flags & SHF_ALLOC) != 0)
	{
	  const char *name = elf_strptr (elf, shstrndx, shdr->sh_name);
	  Elf_Data **p;

	  if (strcmp (name, ".gnu.version") == 0)
	    {
	      p = &ver;
	      ver_shdr = *shdr;
	    }
	  else if (strcmp (name, ".gnu.version_d") == 0)
	    {
	      p = &verd;
	      verd_shdr = *shdr;
	    }
	  else if (strcmp (name, ".dynsym") == 0)
	    {
	      p = &dynsym;
	      dynsym_shdr = *shdr;
	    }
	  else
	    continue;

	  if (*p != NULL)
	    error (1, 0, "Two %s sections in %s", name, fname);
	  *p = elf_getdata (scn, NULL);
	  if (*p == NULL || elf_getdata (scn, *p) != NULL)
	    error (1, 0, "No data or non-contiguous data in %s section in %s",
		   name, fname);
	}
    }

  if (ver == NULL || verd == NULL || dynsym == NULL)
    error (1, 0, "Couldn't find one of the needed sections in %s", fname);

  int from_idx = compute_veridx (argv[2], elf, verd, &verd_shdr);
  if (from_idx == -1)
    error (1, 0, "Could not find symbol version %s in %s", argv[2], fname);

  int to_idx = compute_veridx (argv[3], elf, verd, &verd_shdr);
  if (to_idx == -1)
    error (1, 0, "Could not find symbol version %s in %s", argv[3], fname);

  if (dynsym_shdr.sh_entsize != gelf_fsize (elf, ELF_T_SYM, 1, ehdr->e_version)
      || dynsym_shdr.sh_size % dynsym_shdr.sh_entsize
      || ver_shdr.sh_entsize != 2
      || (ver_shdr.sh_size & 1)
      || dynsym_shdr.sh_size / dynsym_shdr.sh_entsize != ver_shdr.sh_size / 2)
    error (1, 0, "Unexpected sh_size or sh_entsize in %s", fname);

  size_t nentries = ver_shdr.sh_size / 2;
  size_t cnt;
  GElf_Versym array[nentries];
  for (cnt = 0; cnt < nentries; ++cnt)
    {
      GElf_Versym vsymmem;
      GElf_Versym *vsym;

      vsym = gelf_getversym (ver, cnt, &vsymmem);
      if (vsym == NULL)
	error (1, 0, "gelt_getversym failed in %s: %s", fname, elf_errmsg (-1));

      array[cnt] = *vsym;
      if (*vsym != from_idx)
	continue;

      GElf_Sym sym_mem;
      GElf_Sym *sym;
      sym = gelf_getsym (dynsym, cnt, &sym_mem);
      if (sym == NULL)
	error (1, 0, "gelt_getsym failed in %s: %s", fname, elf_errmsg (-1));

      const char *name = elf_strptr (elf, dynsym_shdr.sh_link, sym->st_name);

      int argn;
      for (argn = 4; argn < argc; ++argn)
	if (fnmatch (argv[argn], name, 0) == 0)
	  {
	    array[cnt] = to_idx;
	    break;
	  }
    }

  if (sizeof (array[0]) != 2)
    abort ();

#if __BYTE_ORDER == __LITTLE_ENDIAN
  if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
    ;
  else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
#elif __BYTE_ORDER == __BIG_ENDIAN
  if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
    ;
  else if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
#else
# error Unsupported endianity
#endif
    {
      for (cnt = 0; cnt < nentries; ++cnt)
	array[cnt] = ((array[cnt] & 0xff) << 8) | ((array[cnt] & 0xff00) >> 8);
    }
  else
    error (1, 0, "Unknown EI_DATA %d in %s", ehdr->e_ident[EI_DATA], fname);

  if (elf_end (elf) != 0)
    error (1, 0, "couldn't close %s: %s", fname, elf_errmsg (-1));

  if (lseek (fd, ver_shdr.sh_offset, SEEK_SET) != (off_t) ver_shdr.sh_offset)
    error (1, 0, "failed to seek to %zd in %s", (size_t) ver_shdr.sh_offset,
	   fname);

  if (write (fd, array, 2 * nentries) != (ssize_t) (2 * nentries))
    error (1, 0, "failed to write .gnu.version section into %s", fname);

  close (fd);
  return 0;
}
