--TEST--
numfmt_parse()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Number parsing.

// Test parsing float number.
$fmt = new NumberFormatter( "en_US", NumberFormatter::DECIMAL );
echo $fmt->parse( "123E-3" ) . "\n";

// Test parsing float number as integer.
$fmt = new NumberFormatter( "en_US", NumberFormatter::DECIMAL );
echo $fmt->parse( "1.23", NumberFormatter::TYPE_INT32 ) . "\n";

// Test specifying non-zero parsing start position.
$fmt = new NumberFormatter( "en_US", NumberFormatter::DECIMAL );
$pos = 2;
echo $fmt->parse( "123", NumberFormatter::DECIMAL, $pos ) . "\n";

?>
--EXPECT--
0.123
1
3
