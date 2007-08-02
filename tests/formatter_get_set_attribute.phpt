--TEST--
numfmt_get/set_attribute()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Check setting/getting attribute.

$fmt = number_formatter( "en_US", NumberFormatter::DECIMAL );

// Format a number using default multiplier (1).
echo $fmt->format( 128 ) . "\n";

// Set multiplier to 2.
$fmt->setAttribute( NumberFormatter::MULTIPLIER, 2 );

// Format the same number wit the new multiplier.
echo $fmt->format( 128 ) . "\n";

// Get multiplier value and check if it's 2.
$check = $fmt->getAttribute( NumberFormatter::MULTIPLIER ) == 2;
echo ( $check ? "ok" : "failed to set multiplier" ) . "\n";

?>
--EXPECT--
128
256
ok
