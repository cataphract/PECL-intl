--TEST--
numfmt_format_currency()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

$value = 1234567.89;

# Currency formatting.

$fmt = new NumberFormatter( "en_UK", NumberFormatter::CURRENCY );
echo $fmt->formatCurrency( $value, 'UAH' ) . "\n";

$fmt = new NumberFormatter( "ru_RU", NumberFormatter::CURRENCY );
$s = $fmt->formatCurrency( $value, 'UAH' );
echo dump( $s ) . "\n";

?>
--EXPECT--
UAH1,234,567.89
'1 234 567,89грн.'
