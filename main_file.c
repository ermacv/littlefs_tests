#include "lfs.h"
#include "bd/lfs_filebd.h"

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

static lfs_filebd_t filebd;
// configuration of the filesystem is provided by this struct
static struct lfs_config cfg = {
    // block device operations
    .read  = lfs_filebd_read,
    .prog  = lfs_filebd_prog,
    .erase = lfs_filebd_erase,
    .sync  = lfs_filebd_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 4096,
    .block_count = 128,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,

    .context = &filebd,
};

// entry point
int main(void) {
  int err = lfs_filebd_create(&cfg, "file.lfs");
  if (err) {
    printf("Cannot initialize memory for filedb\r\n");
    return -1;
  }
  // mount the filesystem
  err = lfs_mount(&lfs, &cfg);

  // reformat if we can't mount the filesystem
  // this should only happen on the first boot
  if (err) {
      lfs_format(&lfs, &cfg);
      lfs_mount(&lfs, &cfg);
  }

  // read current count
  uint32_t boot_count = 0;
  lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
  lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

  // update boot count
  boot_count += 1;
  lfs_file_rewind(&lfs, &file);
  lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

  // remember the storage is not updated until the file is closed successfully
  lfs_file_close(&lfs, &file);

  // release any resources we were using
  lfs_unmount(&lfs);

  // print the boot count
  printf("boot_count: %d\n", boot_count);
}