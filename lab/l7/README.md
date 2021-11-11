## Changes in dadfs
```
17d16
< #include <linux/uio.h>
356d354
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
359,361d356
< #else
< ssize_t dadfs_write(struct kiocb * kiocb, struct iov_iter * iov_iter) 
< #endif
371a367
> 
372a369
> 
375d371
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
377,380d372
< #else
<       sb = kiocb->ki_filp->f_inode->i_sb;
< #endif
< 
386,387d377
< 
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
389,392d378
< #else
<       retval = generic_write_checks(kiocb, iov_iter);
< #endif
< 
396d381
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
398,401d382
< #else
<       inode = kiocb->ki_filp->f_inode;
< #endif
< 
404,408c385,386
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
<       bh = sb_bread(filp->f_path.dentry->d_inode->i_sb, sfs_inode->data_block_number);
< #else
<       bh = sb_bread(kiocb->ki_filp->f_inode->i_sb, sfs_inode->data_block_number);
< #endif
---
>       bh = sb_bread(filp->f_path.dentry->d_inode->i_sb,
>                                           sfs_inode->data_block_number);
418d395
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
420,422d396
< #else
<       buffer += kiocb->ki_pos;
< #endif
431d404
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
433,436d405
< #else
<       if(copy_from_iter(buffer, iov_iter->count, iov_iter) == 0){
< #endif
< 
442,443d410
< 
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
445,447d411
< #else
<       kiocb->ki_pos += iov_iter->count; // BUG
< #endif
475,476d438
< 
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
478,481d439
< #else
<       sfs_inode->file_size = (kiocb->ki_pos);
< #endif
< 
484d441
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
487a445
> 
489,496d446
< #else
<               ;
<               mutex_unlock(&dadfs_inodes_mgmt_lock);
<               return retval;
<       }
<       mutex_unlock(&dadfs_inodes_mgmt_lock);
<       return iov_iter->count;
< #endif
501d450
< #if LINUX_VERSION_CODE < KERNEL_VERSION(4, 0, 0)
503,505d451
< #else
<       .write_iter = dadfs_write,
< #endif
776c722
<       printk(KERN_INFO "Journal device is: %s\n", bdevname(dev, b));
---
>       printk(KERN_INFO "Journal device is: %s\n", __bdevname(dev, b));
```