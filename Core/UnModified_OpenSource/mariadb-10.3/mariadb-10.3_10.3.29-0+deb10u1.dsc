-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: mariadb-10.3
Binary: libmariadb-dev, libmariadbclient-dev, libmariadb-dev-compat, libmariadb3, libmariadbd19, libmariadbd-dev, mariadb-common, mariadb-client-core-10.3, mariadb-client-10.3, mariadb-server-core-10.3, mariadb-server-10.3, mariadb-server, mariadb-client, mariadb-backup, mariadb-plugin-connect, mariadb-plugin-rocksdb, mariadb-plugin-oqgraph, mariadb-plugin-tokudb, mariadb-plugin-mroonga, mariadb-plugin-spider, mariadb-plugin-gssapi-server, mariadb-plugin-gssapi-client, mariadb-plugin-cracklib-password-check, mariadb-test, mariadb-test-data
Architecture: any all
Version: 1:10.3.29-0+deb10u1
Maintainer: Debian MySQL Maintainers <pkg-mysql-maint@lists.alioth.debian.org>
Uploaders: Otto Kekäläinen <otto@debian.org>
Homepage: https://mariadb.org/
Standards-Version: 4.2.1
Vcs-Browser: https://salsa.debian.org/mariadb-team/mariadb-10.3
Vcs-Git: https://salsa.debian.org/mariadb-team/mariadb-10.3.git
Testsuite: autopkgtest
Build-Depends: bison, chrpath, cmake, cracklib-runtime, debhelper (>= 9.20160709), dh-apparmor, dh-exec, gdb, libaio-dev [linux-any], libarchive-dev, libboost-dev, libcrack2-dev (>= 2.9.0), libgnutls28-dev (>= 3.3.24), libjemalloc-dev [linux-any], libjudy-dev, libkrb5-dev, liblz4-dev, libncurses5-dev (>= 5.0-6~), libpam0g-dev, libpcre3-dev (>= 2:8.35-3.2~), libreadline-gplv2-dev, libreadline-gplv2-dev:native, libsnappy-dev, libsystemd-dev [linux-any], libxml2-dev, libzstd-dev (>= 1.3.3), lsb-release, perl, po-debconf, psmisc, unixodbc-dev, zlib1g-dev (>= 1:1.1.3-5~)
Package-List:
 libmariadb-dev deb libdevel optional arch=any
 libmariadb-dev-compat deb libdevel optional arch=any
 libmariadb3 deb libs optional arch=any
 libmariadbclient-dev deb oldlibs optional arch=any
 libmariadbd-dev deb libdevel optional arch=any
 libmariadbd19 deb libs optional arch=any
 mariadb-backup deb database optional arch=any
 mariadb-client deb database optional arch=all
 mariadb-client-10.3 deb database optional arch=any
 mariadb-client-core-10.3 deb database optional arch=any
 mariadb-common deb database optional arch=all
 mariadb-plugin-connect deb database optional arch=any
 mariadb-plugin-cracklib-password-check deb database optional arch=any
 mariadb-plugin-gssapi-client deb database optional arch=any
 mariadb-plugin-gssapi-server deb database optional arch=any
 mariadb-plugin-mroonga deb database optional arch=any-alpha,any-amd64,any-arm,any-arm64,any-i386,any-ia64,any-mips64el,any-mips64r6el,any-mipsel,any-mipsr6el,any-nios2,any-powerpcel,any-ppc64el,any-sh3,any-sh4,any-tilegx
 mariadb-plugin-oqgraph deb database optional arch=any
 mariadb-plugin-rocksdb deb database optional arch=amd64,arm64,mips64el,ppc64el
 mariadb-plugin-spider deb database optional arch=any
 mariadb-plugin-tokudb deb database optional arch=amd64
 mariadb-server deb database optional arch=all
 mariadb-server-10.3 deb database optional arch=any
 mariadb-server-core-10.3 deb database optional arch=any
 mariadb-test deb database optional arch=any
 mariadb-test-data deb database optional arch=all
Checksums-Sha1:
 86bbee7bceba4290894f54d1c29e1533821a55f3 73261682 mariadb-10.3_10.3.29.orig.tar.gz
 bc59bbcc0f348bea594564bf169a82d13a606298 195 mariadb-10.3_10.3.29.orig.tar.gz.asc
 85fbbe68ed16994c9624d89472dfa3e36d0e1fb4 221320 mariadb-10.3_10.3.29-0+deb10u1.debian.tar.xz
Checksums-Sha256:
 304a9e94c50636b591ac7cbfe459cb341d345c4bd0ef5e45fb9a692268e6db38 73261682 mariadb-10.3_10.3.29.orig.tar.gz
 a8589036d5099e1395d25d21545088f51c4f6ee54f2e930e36114dc0c6c74ec7 195 mariadb-10.3_10.3.29.orig.tar.gz.asc
 2356baf372d5c1c8ca32735ee6e87af4512305b2fa016a42b811859e59bd08e7 221320 mariadb-10.3_10.3.29-0+deb10u1.debian.tar.xz
Files:
 a5adad1c4fb1717d7fe6d608fd4d40de 73261682 mariadb-10.3_10.3.29.orig.tar.gz
 1af480d48b00c9e5afed58f9641b5353 195 mariadb-10.3_10.3.29.orig.tar.gz.asc
 134fdfc807e26a2fab1426fa676349d1 221320 mariadb-10.3_10.3.29-0+deb10u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEmbRSsR88dMO0U+RvvthEn87o2ogFAmCzFPAACgkQvthEn87o
2ogvVxAAmuDxPgx+1GsoKecV1+qYuhezoIzumqsm+coY6NwTSRlIKhNiv4ZlKK9c
2Egi1m6Av3SBizIvxy9YZ6bHJfRVPkap5tH2NjHqjLc3ho4cLmLW/724TskGwuNG
r3eM44deZeSSnDae75BTx+BBj8qT17wjatJj5qQv3v2jsoqjpThS5Sbdvy7YNSsv
hrerUWBPPMLTJUfSRg7wJGJVYRWfrDHd7W2BecrfD0x2chsWJ6TwXPqPhHqNl3lo
6VXEu63XPtYelbpNI+TDrYm5L/HczgpFPyP9SHi2iU7TeZvBdDl+XYm2m2Ch+Q7G
pg5lk9/0a+xI9ZtdlGcPU4cD2OhBh58cNSWuF3eco4+PkcgIW+lXjcz9PLOAhYuu
UK8vyn18D+xmGurfXodUtExsZnMuuazt1VKgFDIXGx0kbZ/W8vQSFpIRyTrdDHs/
x8RwvdwdT8VbHqbBNgiFRdQCzWpwOaTTmAb0gbHVNCKBtyUoGXrhQF2tzgvBecla
13mTt2RdWdsu8xnOeK/mmfmpbEbV/4FMmX0K0EdxxkhInUyQd1PnK5VamxQ8N5KP
mr4xWTIeE3Z1aN0GbC8r54LHMbMq0YSQztMT/1lv/8SpanzUxH3GLr/IHUBBUVus
Y8XYo8aDJKOQ77ST0+Fj27wMYDazAeNY5qiDEJrHIlZUZzpuEcw=
=jKXp
-----END PGP SIGNATURE-----
