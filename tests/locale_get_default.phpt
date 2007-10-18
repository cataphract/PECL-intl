--TEST--
locale_get_default()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try getting the default Locale with different locales
 * with Procedural and Object methods.
 */

function ut_main()
{
    $res_str = '';
	
	$lang = ut_loc_get_default() ;

    $res_str .= "Default locale: $lang";
    $res_str .= "\n";

    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
Default locale: en_US_POSIX
