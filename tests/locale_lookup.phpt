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
		'de',
		'de-De',
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

    $res_str = '';
	$isCanonical = false;
	
	foreach($loc_ranges as $loc_range){
			$res_str .="--------------\n";
			$result= ut_loc_locale_lookup( $lang_tags , $loc_range,$isCanonical,"en_US");
			$comma_arr =implode(",",$lang_tags); 
			$res_str .= "loc_range:$loc_range and lang_tags $comma_arr\n ";
			$res_str .= "* lookup result:$result\n";
/*
//canonicalized version
			$result= ut_loc_locale_lookup( $lang_tags , $loc_range,!($isCanonical),"en_US");
			$can_loc_range = ut_loc_canonicalize($loc_range);
			//$can_lang_tag = ut_loc_canonicalize($lang_tags);
			//$result= ut_loc_locale_canonical_lookup( $lang_tags , $loc_range);
			$res_str .= "Canonical loc_range:$can_loc_range and lang_tags $comma_arr\n ";
*/
			$res_str .= "* Canonical lookup result:$result\n";
	}

	$res_str .= "\n";
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
--------------
loc_range:de-DE-1996-x-prv1-prv2 and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:de_de_1996
* Canonical lookup result:de_de_1996
--------------
loc_range:de-de and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:de_de
* Canonical lookup result:de_de
--------------
loc_range:sl_IT and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:sl_it
* Canonical lookup result:sl_it
--------------
loc_range:sl_IT_Nedis and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:sl_it
* Canonical lookup result:sl_it
--------------
loc_range:jbo and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:jbo
* Canonical lookup result:jbo
--------------
loc_range:art-lojban and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:art_lojban
* Canonical lookup result:art_lojban
--------------
loc_range:sl_IT and lang_tags de-DEVA,de-DE-1996,de,de-De,zh_Hans,de-CH-1996,sl_IT,sl_IT_nedis-a-kirti-x-xyz,sl_IT_rozaj,sl_IT_NEDIS_ROJAZ_1901,i-enochian,sgn-CH-de,art-lojban,i-lux,art-lojban,jbo,en_sl_IT,zh-Hant-CN-x-prv1-prv2
 * lookup result:sl_it
* Canonical lookup result:sl_it
