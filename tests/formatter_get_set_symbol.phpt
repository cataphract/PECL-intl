--TEST--
numfmt_get/set_symbol()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Get/set symbol.

$fmt = new NumberFormatter( "en_US", NumberFormatter::DECIMAL );

// Get default decimal separator.
$ds = $fmt->getSymbol( NumberFormatter::DECIMAL_SEPARATOR_SYMBOL );
echo "Default decimal separator: [$ds]\n";

// Set new decimal separator.
$fmt->setSymbol( NumberFormatter::DECIMAL_SEPARATOR_SYMBOL, '^' );

// Check if the new decimal separator has been set.
$ds = $fmt->getSymbol( NumberFormatter::DECIMAL_SEPARATOR_SYMBOL );
echo "New decimal separator: [$ds]\n";

// Check if the new decimal separator works.
$s = $fmt->format( 1.2 );
echo "A number formatted with the new decimal separator: $s\n";

?>
--EXPECT--
Default decimal separator: [.]
New decimal separator: [^]
A number formatted with the new decimal separator: 1^2
