-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: krb5
Binary: krb5-user, krb5-kdc, krb5-kdc-ldap, krb5-admin-server, krb5-kpropd, krb5-multidev, libkrb5-dev, libkrb5-dbg, krb5-pkinit, krb5-otp, krb5-k5tls, krb5-doc, libkrb5-3, libgssapi-krb5-2, libgssrpc4, libkadm5srv-mit11, libkadm5clnt-mit11, libk5crypto3, libkdb5-9, libkrb5support0, libkrad0, krb5-gss-samples, krb5-locales, libkrad-dev
Architecture: any all
Version: 1.17-3+deb10u2
Maintainer: Sam Hartman <hartmans@debian.org>
Uploaders: Russ Allbery <rra@debian.org>, Benjamin Kaduk <kaduk@mit.edu>
Homepage: http://web.mit.edu/kerberos/
Standards-Version: 4.1.1
Vcs-Browser: https://salsa.debian.org/debian/krb5
Vcs-Git: https://salsa.debian.org/debian/krb5
Testsuite: autopkgtest
Testsuite-Triggers: ldap-utils, libsasl2-modules-gssapi-mit, slapd
Build-Depends: debhelper (>= 10), byacc | bison, comerr-dev, docbook-to-man, libkeyutils-dev [linux-any], libldap2-dev <!stage1>, libsasl2-dev <!stage1>, libncurses5-dev, libssl-dev, ss-dev, libverto-dev (>= 0.2.4), pkg-config
Build-Depends-Indep: python, python-cheetah, python-lxml, python-sphinx, doxygen, doxygen-latex, texlive-generic-extra
Package-List:
 krb5-admin-server deb net optional arch=any
 krb5-doc deb doc optional arch=all
 krb5-gss-samples deb net optional arch=any
 krb5-k5tls deb net optional arch=any
 krb5-kdc deb net optional arch=any
 krb5-kdc-ldap deb net optional arch=any profile=!stage1
 krb5-kpropd deb net optional arch=any
 krb5-locales deb localization optional arch=all
 krb5-multidev deb libdevel optional arch=any
 krb5-otp deb net optional arch=any
 krb5-pkinit deb net optional arch=any
 krb5-user deb net optional arch=any
 libgssapi-krb5-2 deb libs optional arch=any
 libgssrpc4 deb libs optional arch=any
 libk5crypto3 deb libs optional arch=any
 libkadm5clnt-mit11 deb libs optional arch=any
 libkadm5srv-mit11 deb libs optional arch=any
 libkdb5-9 deb libs optional arch=any
 libkrad-dev deb libdevel optional arch=any
 libkrad0 deb libs optional arch=any
 libkrb5-3 deb libs optional arch=any
 libkrb5-dbg deb debug optional arch=any
 libkrb5-dev deb libdevel optional arch=any
 libkrb5support0 deb libs optional arch=any
Checksums-Sha1:
 0c404b081db9c996c581f636ce450ee28778f338 8761763 krb5_1.17.orig.tar.gz
 3bbc74460767ba7b275b092721a287f49ecf1d0e 102064 krb5_1.17-3+deb10u2.debian.tar.xz
Checksums-Sha256:
 5a6e2284a53de5702d3dc2be3b9339c963f9b5397d3fbbc53beb249380a781f5 8761763 krb5_1.17.orig.tar.gz
 8a943f0b23f68e74809f8e23b55e1b8af638242b464cf04770aa0324b5ba117c 102064 krb5_1.17-3+deb10u2.debian.tar.xz
Files:
 3b729d89eb441150e146780c4138481b 8761763 krb5_1.17.orig.tar.gz
 e756248ac465e282366c1dfa51ed12ff 102064 krb5_1.17-3+deb10u2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQKmBAEBCgCQFiEERkRAmAjBceBVMd3uBUy48xNDz0QFAmD6rB5fFIAAAAAALgAo
aXNzdWVyLWZwckBub3RhdGlvbnMub3BlbnBncC5maWZ0aGhvcnNlbWFuLm5ldDQ2
NDQ0MDk4MDhDMTcxRTA1NTMxRERFRTA1NENCOEYzMTM0M0NGNDQSHGNhcm5pbEBk
ZWJpYW4ub3JnAAoJEAVMuPMTQ89EsJAQAJL6Mpuf5pgoctzSOikPaYnR/MeYJK/v
Kyc8glI8bmYW6563CxtSZg08VV4e9sSi+s11uGydRLNSbzEVpuzn9YUrLPh7T4Q1
nbddoTks62jU2FY8t4CTW0uCA4xpvlbu0qBQK2bzmLiHXwAJ+cbOO/kRnkL90ZrC
CuT/FKxOoS1ENFI0jo362lT9tgmQNK0oDbsmzo9DoZjI5fm//dfxEZFx0tg4yPkk
I5mWYeARAYSKZVzmdkpvVk7iqLL4HuWgADXJVxau/4l09MquUhPRCV0s6RkfSglG
+CeqJtcVYcNjhOheBzQhvJlqsSKusHof1wzlqhoj2PPiVdN5/aPNlRpieSQcmfzR
2cc4A2R2jiLkKG2+e5Vo8E5gpHoBqUSioCtJqTTzZlbylOrYoS5xxrikTEUciIpC
ydo32UdTR2ssi1SZwUb4wckmdHo1PtY6wPXZa9Fh7KGaklQHbOvK8u6N+LNvVf/b
qLMsgV+mdi565oZ5E4xKzMqWcy/djxfz4RI+t1pRkqS1gs1vXLe20LmW1QfEZNVx
s8Pi5T2H+s2gYWN8VEFLVSS2kpoUfxtKva9XMjBXcbfYyviZYYw8QQKzQzlB60+P
BeGuYwK6gZKMA80kN6KD97o8Qh3BxFSWRZw8rYNBFgw+HKvpbN+cOnVniuc8IDQZ
vo+w6yxJdzX0
=gBG+
-----END PGP SIGNATURE-----
