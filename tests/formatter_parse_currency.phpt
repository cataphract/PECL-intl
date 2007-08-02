--TEST--
numfmt_parse_currency()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Currency parsing.

$fmt = new NumberFormatter( "en_US", NumberFormatter::CURRENCY );
$pos = 0;
$currency = '';
$num = $fmt->parseCurrency( '$9,988,776.65', $currency, $pos );
echo "$num $currency\n";

$fmt = new NumberFormatter( "en_US", NumberFormatter::CURRENCY );
$pos = 1;
$currency = '';
$num = $fmt->parseCurrency( ' $123.45', $currency, $pos );
echo "$num $currency\n";

?>
--EXPECT--
9988776.65 USD
123.45 USD
