#!/bin/bash
# Usage: ./autogen.sh [PHP_PREFIX [ICU_PREFIX]]

PHP_PREFIX=/opt/php5
ICU_PREFIX=/usr

function check_dir()
{
	test -d $1 && return
	echo "Directory $1 does not exist."
	exit 1
}

[ -n "$1" ] && PHP_PREFIX="$1"
[ -n "$2" ] && ICU_PREFIX="$2"

# Check PHP and ICU directories for existance.
check_dir $PHP_PREFIX
check_dir $ICU_PREFIX

# Build the extension.
$PHP_PREFIX/bin/phpize

./configure \
	--enable-intl \
	--with-php-config=$PHP_PREFIX/bin/php-config \
	--with-icu-dir=$ICU_PREFIX \
&&

make clean &&
make &&

# Check it.
make test
