--TEST--
numfmt_get/set_text_attribute()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

# Get/set text attribute.

$fmt = new NumberFormatter( "en_US", NumberFormatter::DECIMAL );

// Get default negative prefix and use it.
$np = $fmt->getTextAttribute( NumberFormatter::NEGATIVE_PREFIX );
echo "Default negative prefix: [$np]\n";
echo "Formatting number with default negative prefix: " . $fmt->format( -1 ) . "\n";

// Set new default negative prefix and see if it works out.
$fmt->setTextAttribute( NumberFormatter::NEGATIVE_PREFIX, "<" );
$np = $fmt->getTextAttribute( NumberFormatter::NEGATIVE_PREFIX );
echo "New negative prefix: [$np]\n";
echo "Formatting number with default negative prefix: " . $fmt->format( -1 ) . "\n";

?>
--EXPECT--
Default negative prefix: [-]
Formatting number with default negative prefix: -1
New negative prefix: [<]
Formatting number with default negative prefix: <1
