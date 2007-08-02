--TEST--
numfmt_get_locale()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Get locale.

$fmt = new NumberFormatter( "en_US", NumberFormatter::DECIMAL );
echo "Actual locale: '" . $fmt->getLocale( ULOC_ACTUAL_LOCALE ) . "'\n";

?>
--EXPECT--
Actual locale: 'en_US'
