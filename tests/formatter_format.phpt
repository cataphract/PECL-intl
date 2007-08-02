--TEST--
numfmt_format()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

$value = 1234567.89;

# Number formatting.

$fmt = new NumberFormatter( "en_UK", NumberFormatter::DECIMAL );
echo $fmt->format( $value ) . "\n";

$fmt = number_formatter( "de_DE", NumberFormatter::DECIMAL );
echo $fmt->format( $value ) . "\n";

$fmt = number_formatter( "en_UK", NumberFormatter::PATTERN_DECIMAL, "@@##" );
echo $fmt->format( $value ) . "\n";

$fmt = new NumberFormatter( "en_UK", NumberFormatter::PATTERN_DECIMAL, "0.0" );
echo $fmt->format( $value ) . "\n";

?>
--EXPECT--
1,234,567.89
1.234.567,89
1235000
1234567.9
