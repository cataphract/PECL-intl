--TEST--
numfmt_get/set_pattern()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

include_once( 'ut_common.php' );

# Get/set pattern.

$fmt = new NumberFormatter( "en_US", NumberFormatter::PATTERN_DECIMAL );

// Get default patten.
echo "Default pattern: '" . $fmt->getPattern() . "'\n";

// Set new pattern.
$res = $fmt->setPattern( "0.0" );
if( $res === false )
    echo $fmt->getErrorMessage() . " (" . $fmt->getErrorCode() . ")\n";

// Check if the pattern has changed.
$res = $fmt->getPattern();
if( $res === false )
    echo $fmt->getErrorMessage() . " (" . $fmt->getErrorCode() . ")\n";
echo "New pattern: '" . $fmt->getPattern() . "'\n";

?>
--EXPECT--
Default pattern: ''
New pattern: '#0.0'
