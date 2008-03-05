--TEST--
datefmt_format_ex_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Test for the datefmt_format  function
 */
    $dateFormatter=new DateFormatter("en_US",DateFormatter::LONG,DateFormatter::SHORT , 'CN');
	//$time_arr = array(0,1200000,1200000000,2200000000,3200000000,3600,-3600);
	$time_arr = array(22000000002200000000,220000000022000000000,222200000000,22222200000000);
	$newline = "\n";
	foreach ($time_arr as $time_entry ) {
		echo $newline.'-------------------------';
		echo $newline.'Result of Formatting '.$time_entry.' =  ';
		echo $dateFormatter->format($time_entry) ;
		echo $newline . 'Error_mes :'. intl_get_error_message() ;
		echo '    Error_code :'. intl_get_error_code() ;
		if($dateFormatter){
			echo $newline.'fmt is non-null';
		}else{
			echo $newline.'fmt is null';
		}
		echo $dateFormatter->getLocale();
	}
/*
	   echo $newline.'Formatting 3200000000   ';
   echo $dateFormatter->format(3200000000) ;
   echo $newline.'Formatting 1200000   ';
   echo $dateFormatter->format(1200000) ;
   echo $newline.'Formatting 0   ';
   echo $dateFormatter->format(0) . $newline;

    //$dateFormatter=new DateFormatter("en_US",DateFormatter::LONG,DateFormatter::SHORT , 'America/Los_Angeles');
    $dateFormatter=new DateFormatter("en_US",DateFormatter::LONG,DateFormatter::SHORT , 'CN');
   //expects :January 15, 1970 4:20 AM
   $newline = "\n";
   echo $newline.'Formatting 1200000   ';
   echo $dateFormatter->format(1200000) ;
   echo $newline.'Formatting 0   ';
   echo $dateFormatter->format(0) . $newline;
   echo $newline.'Formatting 1200000000   ';
   echo $dateFormatter->format(1200000000) ;
   echo $newline.'Formatting 2200000000   ';
   echo $dateFormatter->format(2200000000) ;
	echo $newline . 'Error_mes :'. intl_get_error_message() ;
	echo '    Error_code :'. intl_get_error_code() ;
	if($dateFormatter){
		echo $newline.'fmt is non-null';
	}else{
		echo $newline.'fmt is null';
	}
   echo $newline.'Formatting 3200000000   ';
   echo $dateFormatter->format(3200000000) ;
	echo $newline . 'Error_mes :'. intl_get_error_message() ;
	echo '    Error_code :'. intl_get_error_code() ;
	if($dateFormatter){
		echo $newline.'fmt is non-null';
	}else{
		echo $newline.'fmt is null';
	}
   echo $newline.'Formatting 3600   ';
   echo $dateFormatter->format(3600) ;
	echo $newline . 'Error_mes :'. intl_get_error_message() ;
	echo '    Error_code :'. intl_get_error_code() ;
	if($dateFormatter){
		echo $newline.'fmt is non-null';
	}else{
		echo $newline.'fmt is null';
	}
   echo $newline.'Formatting -3600   ';
   echo $dateFormatter->format(-3600) ;
*/


function ut_main()
{
        $res_str = '';
/*
	$locale_arr = array (
		'en_US'
	);
	
	$datetype_arr = array (
                DateFormatter::FULL,
                DateFormatter::LONG,
                DateFormatter::MEDIUM,
                DateFormatter::SHORT,
                DateFormatter::NONE
        );



	$time_arr = array (
		1200000
	);

	$localtime_arr1 = array (
	    'tm_sec' => 24 ,
	    'tm_min' => 3,
	    'tm_hour' => 19,
	    'tm_mday' => 3,
	    'tm_mon' => 3,
	    'tm_year' => 105,
	    'tm_wday' => 0,
	    'tm_yday' => 92,
	    'tm_isdst' => 1
	);
	$localtime_arr2 = array (
	    'tm_sec' => 24 ,
	    'tm_min' => 3,
	    'tm_hour' => 3,
	    'tm_mday' => 3,
	    'tm_mon' => 3,
	    'tm_year' => 205,
	    'tm_wday' => 0,
	    'tm_yday' => 92,
	    'tm_isdst' => 1
	);
	$localtime_arr = array (
		$localtime_arr1,
		$localtime_arr2
	);

	//Test format with input as a timestamp : integer
	foreach( $time_arr as $timestamp_entry){
		$res_str .= "\n------------\n";
		$res_str .= "\nInput timestamp is : $timestamp_entry";
		$res_str .= "\n------------\n";
		foreach( $locale_arr as $locale_entry ){
			foreach( $datetype_arr as $datetype_entry )
	{
		$res_str .= "\nDateFormatter locale= $locale_entry ,datetype = $datetype_entry ,timetype =$datetype_entry ";
		$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry);
		//$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry,'America/Los_Angeles', DateFormatter::GREGORIAN  );
		//$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry);
		$formatted = ut_datefmt_format( $fmt , $timestamp_entry);
		$res_str .= "\nFormatted timestamp is : $formatted";
	}
	}
	}
*/
/*

	//Test format with input as a localtime :array
	foreach( $localtime_arr as $localtime_entry){
		$res_str .= "\n------------\n";
		$res_str .= "\nInput localtime is : ";
		foreach( $localtime_entry as $key => $value){
                    $res_str .= "$key : '$value' , ";
		}

		$res_str .= "\n------------\n";
		foreach( $locale_arr as $locale_entry ){
			foreach( $datetype_arr as $datetype_entry )
	{
		$res_str .= "\nDateFormatter locale= $locale_entry ,datetype = $datetype_entry ,timetype =$datetype_entry ";
		$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry);
		$formatted1 = ut_datefmt_format( $fmt , $localtime_entry);
		if( intl_get_error_code() == U_ZERO_ERROR){
			$res_str .= "\nFormatted localtime_array is : $formatted1";
		}else{
			$res_str .= "\nError while formatting as: '".intl_get_error_message()."'";
		}
	}
	}
	}
*/

	return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
