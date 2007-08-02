--TEST--
numfmt_get_error_message/code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Error handling.

$fmt = new NumberFormatter( "en_US", NumberFormatter::CURRENCY );
$currency = '';
$pos = 0;
$num = $fmt->parseCurrency( '123.45', $currency, $pos );
if( $num === false )
    echo $fmt->getErrorMessage() . " (" . $fmt->getErrorCode() . ")\n";
else
    echo "Ooops, an error should have occured.";
?>
--EXPECT--
Number parsing failed: U_PARSE_ERROR (9)
