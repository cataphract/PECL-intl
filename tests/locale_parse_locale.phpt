--TEST--
locale_parse_locale()
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
    $res_str = '';

    $locales = array(
        'sl_IT_nedis',
	'uk-ua_CALIFORNIA@currency=;currency=GRN' ,
        'sl_nedis-x-prv1-prv2',
        'en-a-ext1-x-prv',
        'sl_nedis-x-prv1-prv2_IT',
    );


    $res_str = '';

    foreach( $locales as $locale )
    {
        $arr = ut_loc_locale_parse( $locale);
        $res_str .= "---------------------\n";
        $res_str .= "$locale: \n";
        if( $arr){
            foreach( $arr as $key => $value){
                    $res_str .= "$key : '$value' , ";
            }
        }
        else{
            $res_str .= "No values found from Locale parsing.";
        }
        $res_str .= "\n";
    }

    $res_str .= "\n";
    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
---------------------
sl_IT_nedis: 
language : 'sl' , region : 'IT' , variant0 : 'NEDIS' , 
---------------------
uk-ua_CALIFORNIA@currency=;currency=GRN: 
language : 'uk' , region : 'UA' , variant0 : 'CALIFORNIA' , 
---------------------
sl_nedis-x-prv1 prv2: 
language : 'sl' , region : 'NEDIS' , private0 : 'prv1' , private1 : 'prv2' , 
---------------------
en-a-ext1-x-prv: 
language : 'en' , private0 : 'prv' , 
---------------------
sl_nedis-x-prv1 prv2 IT: 
language : 'sl' , region : 'NEDIS' , private0 : 'prv1' , private1 : 'prv2' , private2 : 'IT' ,