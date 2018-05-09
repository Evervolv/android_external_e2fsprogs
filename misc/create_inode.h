#ifndef _CREATE_INODE_H
#define _CREATE_INODE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "et/com_err.h"
#include "e2p/e2p.h"
#include "ext2fs/ext2fs.h"

struct hdlink_s
{
	dev_t src_dev;
	ino_t src_ino;
	ext2_ino_t dst_ino;
};

struct hdlinks_s
{
	int count;
	int size;
	struct hdlink_s *hdl;
};

#define HDLINK_CNT	(4)

struct fs_ops_callbacks {
	errcode_t (* create_new_inode)(ext2_filsys fs, const char *target_path,
		const char *name, ext2_ino_t parent_ino, ext2_ino_t root,
		mode_t mode);
	errcode_t (* end_create_new_inode)(ext2_filsys fs,
		const char *target_path, const char *name,
		ext2_ino_t parent_ino, ext2_ino_t root, mode_t mode);
};

/*
 * Represents a range of UID/GID mapping.
 * This maps the id in [|parent_id|, |parent_id| + |length|) into
 * [|child_id|, |child_id| + |length|)
 */
struct ugid_map_entry {
	unsigned int child_id;
	unsigned int parent_id;
	unsigned int length;
};

struct ugid_map {
	/* The number of elements in |entries|. */
	size_t size;

	/* An array of entries. If |size| is 0, this is a null pointer. */
	struct ugid_map_entry* entries;
};

/* For populating the filesystem */
extern errcode_t populate_fs(ext2_filsys fs, ext2_ino_t parent_ino,
			     const char *source_dir, ext2_ino_t root);
extern errcode_t populate_fs2(ext2_filsys fs, ext2_ino_t parent_ino,
			      const char *source_dir, ext2_ino_t root,
			      const struct ugid_map* uid_map,
			      const struct ugid_map* gid_map,
			      struct fs_ops_callbacks *fs_callbacks);
extern errcode_t do_mknod_internal(ext2_filsys fs, ext2_ino_t cwd,
				   const char *name, struct stat *st);
extern errcode_t do_symlink_internal(ext2_filsys fs, ext2_ino_t cwd,
				     const char *name, char *target,
				     ext2_ino_t root);
extern errcode_t do_mkdir_internal(ext2_filsys fs, ext2_ino_t cwd,
				   const char *name, ext2_ino_t root);
extern errcode_t do_write_internal(ext2_filsys fs, ext2_ino_t cwd,
				   const char *src, const char *dest,
				   ext2_ino_t root);

#endif /* _CREATE_INODE_H */
