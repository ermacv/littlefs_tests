#include "lfs.h"
#include "bd/lfs_rambd.h"

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

static lfs_rambd_t rambd;
// configuration of the filesystem is provided by this struct
static struct lfs_config cfg = {
    // block device operations
    .read  = lfs_rambd_read,
    .prog  = lfs_rambd_prog,
    .erase = lfs_rambd_erase,
    .sync  = lfs_rambd_sync,

    // block device configuration
    .read_size = 16,
    .prog_size = 16,
    .block_size = 4096,
    .block_count = 128,
    .cache_size = 16,
    .lookahead_size = 16,
    .block_cycles = 500,

    .context = &rambd,
};

// entry point
int main(void) {
  int err = lfs_rambd_create(&cfg);
  if (err) {
    printf("Cannot initialize memory for ramdb\r\n");
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
  lfs_rambd_destroy(&cfg);
}