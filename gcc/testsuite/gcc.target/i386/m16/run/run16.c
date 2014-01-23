/*
 * Wrapper to run a 16-bit raw binary as a "BIOS image" under Qemu
 */

#define _GNU_SOURCE
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include16/sys16.h"

/*
 * The program (toybox) is loaded at 0xe0000 and then copied down to
 * 0x10000 to be in "RAM"; the wrapper_data is loaded at 0xf0000 and
 * contains anything needed to set up the early execution environment.
 */
#define TOYBOX_SIZE  65536
#define WRAPPER_SIZE 65536

static unsigned char *toybox, *wrapper_data;
static int toyprot[TOYBOX_SIZE/4096];
extern const unsigned char wrapper[];
extern const unsigned int wrapper_len;

#define ALIGN_UP(x,y) (((x) + (y) - 1) & ~((y) - 1))

static void init_page_tables(void)
{
	int i;
	uint64_t *page_tables = (uint64_t *)(wrapper_data + 0x2000);

	/* Map 256 pages = 1MiB */
	for (i = 0; i < 1024/4; i++) {
		/* Page present, RWX, system */
		page_tables[i] = (i << 12) | 0x063;
	}

	/* Map 64-128K as user pages with appropriate permissions */
	for (i = 64/4; i < 128/4; i++) {
		int tp = toyprot[i-64/4];
		page_tables[i] = (i << 12) | 0x064
			| ((tp & PF_X) ? 0 : (UINT64_C(1) << 63))
			| ((tp & PF_W) ? 2 : 0)
			| ((tp & (PF_X|PF_W|PF_R)) ? 1 : 0);
	}

	/* Set up PDPTR */
	*(uint64_t *)(wrapper_data + 0x0000) = 0xf1000 | 1;

	/* Set up PDE */
	*(uint64_t *)(wrapper_data + 0x1000) = 0xf2000 | 0x027;
}

static void init_reset_vector(void)
{
	/* jmp far 0xf000:0x8000 */
	memcpy(wrapper_data + 0xfff0, "\xea\x00\x80\x00\xf0", 5);
}

static void init_wrapper(void)
{
	/* Fixed wrapper data at 0xf000:0x8000 */
	memcpy(wrapper_data + 0x8000, wrapper, wrapper_len);

	init_page_tables();

	init_reset_vector();
}

static int load_elf(const char *file, uint32_t *start)
{
	int fd;
	struct stat st;
	const char *fp;		/* File data pointer */
	const Elf32_Ehdr *eh;
	const Elf32_Phdr *ph;
	int rv = -1;
	int i;

	fd = open(file, O_RDONLY);
	if (fd < 0 || fstat(fd, &st))
		goto bail;

	fp = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (fp == MAP_FAILED)
		goto bail;

	errno = ENOEXEC;	/* If no other error... */

	/* Must be long enough */
	if (st.st_size < sizeof(Elf32_Ehdr))
		goto bail;

	eh = (const Elf32_Ehdr *)fp;

	/* Must be ELF, 32-bit, littleendian, version 1 */
	if (memcmp(eh->e_ident, "\x7f" "ELF\1\1\1", 6))
		goto bail;

	if (eh->e_machine != EM_386)
		goto bail;

	if (eh->e_version != EV_CURRENT)
		goto bail;

	if (eh->e_ehsize < sizeof(Elf32_Ehdr) || eh->e_ehsize >= st.st_size)
		goto bail;

	if (eh->e_phentsize < sizeof(Elf32_Phdr))
		goto bail;

	if (!eh->e_phnum)
		goto bail;

	if (eh->e_phoff + eh->e_phentsize * eh->e_phnum > st.st_size)
		goto bail;

	if (eh->e_entry > TOYBOX_SIZE)
		goto bail;

	*start = eh->e_entry;

	ph = (const Elf32_Phdr *)(fp + eh->e_phoff);

	for (i = 0; i < eh->e_phnum; i++) {
		uint32_t addr = ph->p_paddr;
		uint32_t msize = ph->p_memsz;
		uint32_t dsize = ph->p_filesz;
		int page, flags;

		ph = (const Elf32_Phdr *)((const char *)ph +
					  i*eh->e_phentsize);

		if (msize && (ph->p_type == PT_LOAD ||
			      ph->p_type == PT_PHDR)) {
			/*
			 * This loads at p_paddr, which is arguably
			 * the correct semantics (LMA).  The SysV spec
			 * says that SysV loads at p_vaddr (and thus
			 * Linux does, too); that is, however, a major
			 * brainfuckage in the spec.
			 */
			if (msize < dsize)
				dsize = msize;

			if (addr >= TOYBOX_SIZE ||
			    addr + msize > TOYBOX_SIZE)
				goto bail;

			if (dsize) {
				if (ph->p_offset >= st.st_size ||
				    ph->p_offset + dsize > st.st_size)
					goto bail;
				memcpy(toybox + addr, fp + ph->p_offset,
				       dsize);
			}

			memset(toybox + addr + dsize, 0, msize - dsize);

			flags = ph->p_flags & (PF_X|PF_W|PF_R);
			for (page = addr & ~0xfff; page < addr + msize;
			     page += 4096)
				toyprot[page >> 12] |= flags;
		}
	}

	rv = 0;			/* Success! */

bail:
	if (fd >= 0)
		close(fd);
	return rv;
}

static void build_env(const char *file, char **argv)
{
	uint32_t start;
	struct system_struct *SYS;
	char **argp;
	uint32_t argptr, argstr;

	/* Read the input file */
	if (load_elf(file, &start))
		goto barf;

	/* Set up the system structure */
	SYS = (struct system_struct *)(toybox + SYS_STRUCT_ADDR);
	SYS->entrypoint = start;
	for (argp = argv; *argp; argp++)
		SYS->argc++;

	argptr = ALIGN_UP(SYS_STRUCT_ADDR + sizeof(struct system_struct), 4);
	argstr = argptr + (SYS->argc + 1)*sizeof(uint32_t);

	SYS->argv = argptr;

	for (argp = argv; *argp; argp++) {
		int len = strlen(*argp)+1;

		if (argstr + len >= TOYBOX_SIZE) {
			fprintf(stderr, "run16: %s: command line too long\n",
				file);
			exit(127);
		}

		*(uint32_t *)(toybox + argptr) = argstr;
		argptr += sizeof(uint32_t);
		memcpy(toybox + argstr, *argp, len);
		argstr += len;
	}

	/* Set up page protection */
	toyprot[0xE] |= PF_R | PF_W;	/* Stack page */
	toyprot[0xF] |= PF_R;		/* System page */

	init_wrapper();
	return;

barf:
	fprintf(stderr, "run16: %s: %s\n", file, strerror(errno));
	exit(127);
}

static int run(const char *qemu, const char *file, char **argv)
{
	const char *tmpdir, *et;
	char *filename;
	int fd, status, err;
	pid_t p;

	tmpdir = getenv("TMPDIR");
	if (!tmpdir)
		tmpdir = _PATH_TMP;

	et = strchr(tmpdir, '\0');
	if (asprintf(&filename, "%s%srun16.tmp.XXXXXX", tmpdir,
		     (et == tmpdir || et[-1] == '/') ? "" : "/") < 0)
		goto barf_no_rm;
	fd = mkstemp(filename);
	if (fd < 0)
		goto barf_no_rm;

	if (ftruncate(fd, TOYBOX_SIZE + WRAPPER_SIZE))
		goto barf;

	toybox = mmap(NULL, TOYBOX_SIZE + WRAPPER_SIZE, PROT_READ|PROT_WRITE,
		      MAP_SHARED, fd, 0);
	if (toybox == MAP_FAILED)
		goto barf;

	wrapper_data = toybox + TOYBOX_SIZE;

	build_env(file, argv);

	if (munmap(toybox, TOYBOX_SIZE + WRAPPER_SIZE))
		goto barf;
	if (close(fd))
		goto barf;

	fflush(NULL);

	p = fork();
	if (p < 0)
		goto barf;

	if (p == 0) {
		execlp(qemu, qemu, "-cpu", "qemu32,+xd",
		       "-nodefaults", "-nodefconfig",
		       "-m", "1", "-display", "none", "-vga", "none",
		       "-debugcon", "stdio", "-device", "isa-debug-exit",
		       "-bios", filename, (char *)NULL);
		_exit(127);
	}

	while (waitpid(p, &status, 0) != p)
		;

	unlink(filename);

	return (WIFEXITED(status) && WEXITSTATUS(status) == (('S' << 1) | 1)
		? 0 : 1);

barf:
	err = errno;
	unlink(filename);
	errno = err;
barf_no_rm:
	fprintf(stderr, "run16: %s: %s\n", file, strerror(errno));
	exit(127);
}

int main(int argc, char *argv[])
{
	const char *qemu = "qemu-system-i386";

	if (argc < 2) {
		fprintf(stderr, "Usage: %s filename [args...]\n", argv[0]);
		return 127;
	}

	return run(qemu, argv[1], argv+1);
}
