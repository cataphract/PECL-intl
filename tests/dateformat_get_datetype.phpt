--TEST--
datefmt_get_datetype_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
<?php print 'skip'; ?>
--FILE--
<?php

/*
 * Test for the datefmt_get_datetype  function
 */


function ut_main()
{
	$datetype_arr = array (
		DateFormatter::FULL,
		DateFormatter::LONG,
		DateFormatter::MEDIUM,
		DateFormatter::SHORT,
		DateFormatter::NONE
	);
	
	$res_str = '';

	foreach( $datetype_arr as $datetype_entry )
	{
		$res_str .= "\nCreating DateFormatter with date_type = $datetype_entry";
		$fmt = ut_datefmt_create( "de-DE",  $datetype_entry , DateFormatter::SHORT,'America/Los_Angeles', DateFormatter::GREGORIAN  );
		$date_type = ut_datefmt_get_datetype( $fmt);
		$res_str .= "\nAfter call to get_datetype :  datetype= $date_type";
		$res_str .= "\n";
	}

	return $res_str;

}

include_once( 'ut_common.inc' );

// Run the test
ut_run();
?>
--EXPECT--
Creating DateFormatter with date_type = 0
After call to get_datetype :  datetype= 0

Creating DateFormatter with date_type = 1
After call to get_datetype :  datetype= 1

Creating DateFormatter with date_type = 2
After call to get_datetype :  datetype= 2

Creating DateFormatter with date_type = 3
After call to get_datetype :  datetype= 3

Creating DateFormatter with date_type = -1
After call to get_datetype :  datetype= -1
