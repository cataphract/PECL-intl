--TEST--
get_display_name()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try to get display name for various locales
 * with Procedural and Object methods
 */


function ut_main()
{
    $res_str = '';

    // Locales to test.
    $test_params = array(
        array( 'en_US', 'en_US' ),
        array( 'en_CA', 'en_CA' ),
        array( 'uk_UA', '' ),
        array( '', 'en_US' )
    );

    foreach( $test_params as $test_param )
    {
        list( $obj_loc, $disp_loc ) = $test_param;

        // Get locale display name.
        $res_val = ut_coll_get_display_name( $obj_loc, $disp_loc );

        $res_str .= sprintf( "%s displayed in %s is %s\n",
                             dump( $obj_loc ),
                             dump( $disp_loc ),
                             dump( $res_val ) );
    }

    return $res_str;
}

include_once( 'ut_common.php' );
ut_run();
?>
--EXPECT--
'en_US' displayed in 'en_US' is 'English (United States)'
'en_CA' displayed in 'en_CA' is 'English (Canada)'
'uk_UA' displayed in '' is 'uk (UA)'
'' displayed in 'en_US' is ''
