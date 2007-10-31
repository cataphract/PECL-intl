--TEST--
locale_lookup.phpt()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try parsing different Locales  
 * with Procedural and Object methods.
 */

function ut_main()
{
/*
	$loc_ranges = array(
		'de-de',
		'sl_IT',
		'sl_IT_Nedis',
		'jbo',
		'art-lojban',
		'sl_IT'
	);
	
	$lang_tags = array(
		'de-DEVA',
		'de-DE-1996',
		'de-DE',
		'zh_Hans',
		'de-CH-1996',
		'sl_IT',
		'sl_IT_nedis-a-kirti-x-xyz',
		'sl_IT_rozaj',
		'sl_IT_NEDIS_ROJAZ_1901',
		'i-enochian',
		'sgn-CH-de',
		'art-lojban',
		'i-lux',
		'art-lojban',
		'jbo',
		'en_sl_IT',
		'zh-Hant-CN-x-prv1-prv2'
	);
*/

	$loc_ranges = array(
		'de-DE-1996-x-prv1-prv2',
	);
	$lang_tags = array(
		'de-DEVA',
		'de-DE-1996',
		'de',
		'de-De'
	);

    $res_str = '';
	foreach($loc_ranges as $loc_range){
			$res_str .="--------------\n";
			$result= ut_loc_locale_lookup( $lang_tags , $loc_range,"en_US");
			$comma_arr =implode(",",$lang_tags); 
			$res_str .= "loc_range:$loc_range and lang_tags $comma_arr\n ";
			$res_str .= "lookup result:$result\n";
/*
//canonicalized version
			$result= ut_loc_locale_canonical_lookup( $lang_tags , $loc_range);
			$can_loc_range = ut_loc_canonicalize($loc_range);
			//$can_lang_tag = ut_loc_canonicalize($lang_tags);
			$result= ut_loc_locale_canonical_lookup( $lang_tags , $loc_range);
			$res_str .= "Canonical loc_range:$can_loc_range and lang_tags $comma_arr\n ";
*/
			$res_str .= "lookup result:$result\n";
	}

	$res_str .= "\n";
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
--------------
loc_range:de-DE-1996-x-prv1-prv2 and lang_tags de-DEVA,de-DE-1996,de,de-De
 lookup result:de-de-1996
lookup result:de-de-1996