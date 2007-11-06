--TEST--
locale_compose_locale()
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

	$loc_parts_arr1 = array( 
		'language'=>'sl' ,
		'script'=>'Latn' ,
		'region'=>'IT' ,
		'variant'=>'nedis' 
	);
	$loc_parts_arr2 = array( 
		'language'=>'de' ,
		'region'=>'DE'
	);
	$loc_parts_arr3 = array( 
		'language'=>'hi'
	);
	$loc_parts_arr4 = array( 
		'language'=>'zh' ,
		'script'=>'Hans' ,
		'region'=>'CN'
	);
	$loc_parts_arr5 = array( 
		'language'=>'en' ,
		'language'=>'es' ,
		'script'=>'Hans' ,
		'region'=>'CN'
	);
	$loc_parts_arr6 = array( 
		'language'=>'en' ,
		'script'=>'Hans' ,
		'region'=>'CN',
		'variant18'=>'rozaj' ,
		'variant1'=>'nedis' 
	);
	$loc_parts_arr7 = array( 
		'language'=>'en' ,
		'script'=>'Hans' ,
		'region'=>'CN',
		'variant14'=>'rozaj' ,
		'variant1'=>'nedis' ,
		'extlang0'=>'lng' ,
		'extlang10'=>'ing'
	);
	$loc_parts_arr8 = array( 
		'language'=>'en' ,
		'script'=>'Hans' ,
		'region'=>'CN',
		'variant14'=>'rozaj' ,
		'variant1'=>'nedis' ,
		'extlang0'=>'lng' ,
		'extlang1'=>'ing',
		'private7'=>'prv1' ,
		'private9'=>'prv2'
	);
	$loc_parts_arr9 = array( 
		'region'=>'DE'
	);
	$loc_parts_arr10 = array( 
		'grandfathered'=>'zh-hakka' 
	);
	$loc_parts_arr11 = array( 
		'grandfathered'=>'i-tay' 
	);
	$loc_parts_arr12 = array( 
		'grandfathered'=>'zh-hakka' ,
		'language'=>'en' 
	);

	$loc_parts_arr = array(
		'loc1' => $loc_parts_arr1	,
		'loc2' => $loc_parts_arr2	,
		'loc3' => $loc_parts_arr3	,
		'loc4' => $loc_parts_arr4	,
		'loc5' => $loc_parts_arr5	,	
		'loc6' => $loc_parts_arr6	,
		'loc7' => $loc_parts_arr7	,
		'loc8' => $loc_parts_arr8	,
		'loc9' => $loc_parts_arr9	,
		'loc10' => $loc_parts_arr10	,
		'loc11' => $loc_parts_arr11	,
		'loc12' => $loc_parts_arr12	
	);
/*
	$loc_parts_arr = array(
		'loc6' => $loc_parts_arr6	,
		'loc7' => $loc_parts_arr7	,
		'loc8' => $loc_parts_arr8	
	);
*/

    $res_str = '';
	foreach($loc_parts_arr as $key => $value ){
		$locale = ut_loc_locale_compose( $value);
		$res_str .= "Locale: ";
		if( $locale){
			$res_str .= "$locale\n";
		}else{
			$res_str .= "No values found from Locale compose.\n";
		}
	}

	$res_str .= "\n";
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
Locale: sl_Latn_IT_nedis
Locale: de_DE
Locale: hi
Locale: zh_Hans_CN
Locale: es_Hans_CN
Locale: en_Hans_CN_nedis
Locale: en_lng_Hans_CN_nedis_rozaj
Locale: en_lng_ing_Hans_CN_nedis_rozaj_x_prv1_prv2
Locale: No values found from Locale compose.
Locale: zh-hakka
Locale: i-tay
Locale: zh-hakka