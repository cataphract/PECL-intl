--TEST--
datefmt_get_locale_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Test for the datefmt_get_locale  function
 */


function ut_main()
{
	$locale_arr = array (
		'i-enochian',
		'de-DE',
		'sl-IT-nedis',
		'en_UK',
		'hi'
	);
	
	$res_str = '';

	foreach( $locale_arr as $locale_entry )
	{
		$res_str .= "\nCreating DateFormatter with locale = $locale_entry";
		$fmt = ut_datefmt_create( $locale_entry , DateFormatter::SHORT,DateFormatter::SHORT,'America/Los_Angeles', DateFormatter::GREGORIAN  );
		$locale = ut_datefmt_get_locale( $fmt , 1);
		$res_str .= "\nAfter call to get_locale :  locale= $locale";
		$res_str .= "\n";
	}

	return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
Creating DateFormatter with locale = i-enochian
After call to get_locale :  locale= en_US_POSIX

Creating DateFormatter with locale = de-DE
After call to get_locale :  locale= de_DE

Creating DateFormatter with locale = sl-IT-nedis
After call to get_locale :  locale= sl

Creating DateFormatter with locale = en_UK
After call to get_locale :  locale= en

Creating DateFormatter with locale = hi
After call to get_locale :  locale= hi
