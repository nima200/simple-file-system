CFLAGS = -c -g -Wall -std=gnu99 `pkg-config fuse --cflags --libs`

LDFLAGS = `pkg-config fuse --cflags --libs`

# Uncomment on of the following three lines to compile
SOURCES= disk_emu.c sfs_api.c sfs_test.c
#SOURCES= disk_emu.c sfs_api.c sfs_test2.c
#SOURCES= disk_emu.c sfs_api.c fuse_wrappers.c


HEADERS= \
sfs_api.h \
typedefs/super_block.h \
typedefs/inode.h \
typedefs/free_bitmap.h \
typedefs/file_descriptor.h \
typedefs/directory_entry.h \
typedefs/inode_table.h \
typedefs/directory.h \
typedefs/file_descriptor_table.h \
typedefs/indirect_block.h \
constants.h \
utils/disk_utils.h \


OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= 260606511_ADIBPOUR_NIMA

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o */*.o *.disk *~ $(EXECUTABLE)
