--TEST--
IntlTimeZone::getGMT(): basic test
--SKIPIF--
<?php
if (!extension_loaded('intl'))
	die('skip intl extension not enabled');
if (!defined('PHP_VERSION_ID') || PHP_VERSION_ID < 50300)
	die('skip for PHP >= 5.3');
--FILE--
<?php
ini_set("intl.error_level", E_WARNING);
$tz = IntlTimeZone::getGMT();
print_r($tz);
$tz = intltz_get_gmt();
print_r($tz);
?>
==DONE==
--EXPECT--
IntlTimeZone Object
(
    [valid] => 1
    [id] => GMT
    [rawOffset] => 0
    [currentOffset] => 0
)
IntlTimeZone Object
(
    [valid] => 1
    [id] => GMT
    [rawOffset] => 0
    [currentOffset] => 0
)
==DONE==
