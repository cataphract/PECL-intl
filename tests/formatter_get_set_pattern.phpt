--TEST--
numfmt_get/set_pattern()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Get/set pattern.
 */


function ut_main()
{
    $res_str = '';
    $test_value = 12345.123456;
    $fmt = ut_nfmt_create( "en_US", NumberFormatter::PATTERN_DECIMAL );

    // Get default patten.
    $res_str .= "Default pattern: '" . ut_nfmt_get_pattern( $fmt ) . "'\n";
    $res_str .= "Formatting result: " . ut_nfmt_format( $fmt, $test_value ) . "\n";

    // Set a new pattern.
    $res = ut_nfmt_set_pattern( $fmt, "0.0" );
    if( $res === false )
        $res_str .= ut_nfmt_get_error_message( $fmt ) . " (" . ut_nfmt_get_error_code( $fmt ) . ")\n";

    // Check if the pattern has been changed.
    $res = ut_nfmt_get_pattern( $fmt );
    if( $res === false )
        $res_str .= ut_nfmt_get_error_message( $fmt ) . " (" . ut_nfmt_get_error_code( $fmt ) . ")\n";
    $res_str .= "New pattern: '" . ut_nfmt_get_pattern( $fmt ) . "'\n";
    $res_str .= "Formatted number: " . ut_nfmt_format( $fmt, $test_value ) . "\n";

    return $res_str;
}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
Default pattern: ''
Formatting result: 12345.123456
New pattern: '#0.0'
Formatted number: 12345.1