# build-helper

Common script used in other build scripts.

- `host-functions-source.sh`: to be included with `source` in the host build script
- `container-functions-source.sh`: to be included with `source` in the container build script

Deprecated:

- `builder-helper.sh`: used in the first generation of build scripts.

# Patches

The code used to download and extract archives can also be used
to post-patch the downloaded files. For this a patch file must be
placed in the `patches` folder, and the name must be passed as the
third param to `extract()`.

## Memo

The code to apply the patch (`common-functions-source.sh:extract()`) does 
the following:

```console
$ cd sources/binutils
$ patch -p0 < "binutils-2.31.patch"
```

The patch is applied from the library source folder, so it must be created
with the library relative path.

For example, to create a binutils patch, use:

```console
$ cd sources/binutils
$ cp bfd/ihex.c bfd/ihex-patched.c
$ vi bfd/ihex-patched.c
$ diff -u bfd/ihex.c bfd/ihex-patched.c > patches/binutils-2.31.patch
```


There may also be git patches; they are applied with `git apply file.git-patch`.

To create a patch with the uncommitted changes: 

```console
$ git diff > file.git-patch
```

Links:

- https://git-scm.com/docs/git-diff
- https://git-scm.com/docs/git-apply

