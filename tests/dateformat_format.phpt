--TEST--
datefmt_format_code()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Test for the datefmt_format  function
 */


function ut_main()
{
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

        $res_str = '';


	$time_arr = array (
		0,
		-1200000,
		1200000,
		2200000000,
		-2200000000,
		3600,
		-3600
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
	$localtime_arr3 = array (
            'tm_sec' => 0 ,
            'tm_min' => 0,
            'tm_hour' => 5,
            'tm_mday' => 12,
            'tm_mon' => 12,
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
		//$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry,'America/Los_Angeles', DateFormatter::GREGORIAN  ,"dd/mm/yyyy");
		$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry,'America/Los_Angeles', DateFormatter::GREGORIAN  );
		//$fmt = ut_datefmt_create( $locale_entry , $datetype_entry ,$datetype_entry);
		$formatted = ut_datefmt_format( $fmt , $timestamp_entry);
		$res_str .= "\nFormatted timestamp is : $formatted";
	}
	}
	}

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

	return $res_str;

}

include_once( 'ut_common.php' );

// Run the test
ut_run();
?>
--EXPECT--
------------

Input timestamp is : 0
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, December 31, 1969 4:00:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : December 31, 1969 4:00:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Dec 31, 1969 4:00:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 12/31/69 4:00 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19691231 04:00 PM
------------

Input timestamp is : -1200000
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, December 17, 1969 6:40:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : December 17, 1969 6:40:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Dec 17, 1969 6:40:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 12/17/69 6:40 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19691217 06:40 PM
------------

Input timestamp is : 1200000
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, January 14, 1970 1:20:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : January 14, 1970 1:20:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Jan 14, 1970 1:20:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 1/14/70 1:20 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19700114 01:20 PM
------------

Input timestamp is : 2200000000
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, December 31, 1969 4:00:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : December 31, 1969 4:00:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Dec 31, 1969 4:00:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 12/31/69 4:00 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19691231 04:00 PM
------------

Input timestamp is : -2200000000
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, December 31, 1969 4:00:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : December 31, 1969 4:00:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Dec 31, 1969 4:00:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 12/31/69 4:00 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19691231 04:00 PM
------------

Input timestamp is : 3600
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, December 31, 1969 5:00:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : December 31, 1969 5:00:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Dec 31, 1969 5:00:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 12/31/69 5:00 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19691231 05:00 PM
------------

Input timestamp is : -3600
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted timestamp is : Wednesday, December 31, 1969 3:00:00 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted timestamp is : December 31, 1969 3:00:00 PM PST
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted timestamp is : Dec 31, 1969 3:00:00 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted timestamp is : 12/31/69 3:00 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted timestamp is : 19691231 03:00 PM
------------

Input localtime is : tm_sec : '24' , tm_min : '3' , tm_hour : '19' , tm_mday : '3' , tm_mon : '3' , tm_year : '105' , tm_wday : '0' , tm_yday : '92' , tm_isdst : '1' , 
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted localtime_array is : Thursday, April 3, 0105 7:03:24 PM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted localtime_array is : April 3, 0105 7:03:24 PM PDT
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted localtime_array is : Apr 3, 0105 7:03:24 PM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted localtime_array is : 4/3/05 7:03 PM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted localtime_array is : 01050403 07:03 PM
------------

Input localtime is : tm_sec : '24' , tm_min : '3' , tm_hour : '3' , tm_mday : '3' , tm_mon : '3' , tm_year : '205' , tm_wday : '0' , tm_yday : '92' , tm_isdst : '1' , 
------------

DateFormatter locale= en_US ,datetype = 0 ,timetype =0 
Formatted localtime_array is : Wednesday, April 3, 0205 3:03:24 AM PT
DateFormatter locale= en_US ,datetype = 1 ,timetype =1 
Formatted localtime_array is : April 3, 0205 3:03:24 AM PDT
DateFormatter locale= en_US ,datetype = 2 ,timetype =2 
Formatted localtime_array is : Apr 3, 0205 3:03:24 AM
DateFormatter locale= en_US ,datetype = 3 ,timetype =3 
Formatted localtime_array is : 4/3/05 3:03 AM
DateFormatter locale= en_US ,datetype = -1 ,timetype =-1 
Formatted localtime_array is : 02050403 03:03 AM