# nullfs

## description

nullfs implements a FUSE handler for a flat null filesystem.

flat in that the handler allows the creation, reading, and writing of files but
not of directories.

null in that bytes are never committed to the file system when writing to a
file and bytes are never returned when reading from a file, accomplished by
the handler always returning success for those operations.

## usage

### create a mount point

```bash
mkdir /mnt/null
```

### invoke nullfs on that mount point

```bash
nullfs /mnt/null
```

## development

### code analysis

Use splint (formerly lclint) to statically analyze the code.

### code style

There is only one true bracket style.

Use `astyle --style=otbs` to maintain coding conventions.
