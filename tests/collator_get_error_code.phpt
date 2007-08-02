--TEST--
get_error_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Check error codes with Procedural and Object methods
 */
function ut_main()
{
    $res = '';
    $coll = ut_coll_create( 'ru_RU' );

    # Try specifying a correct attribute.
    ut_coll_get_attribute( $coll, Collator::NORMALIZATION_MODE );
    $status = ut_coll_get_error_code( $coll );
    $res .= $status . "\n";

    # Try specifying an incorrect attribute.
    ut_coll_get_attribute( $coll, 12345 );
    $status = ut_coll_get_error_code( $coll );
    $res .=  $status . "\n";

    return $res;
}

# Suppress warning messages.
error_reporting( E_ERROR );

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
0
1
