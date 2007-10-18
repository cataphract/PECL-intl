--TEST--
locale_get_display_variant()
--SKIPIF--
<?php if( !extension_loaded( 'intl' ) ) print 'skip'; ?>
--FILE--
<?php

/*
 * Try getting the display_variant for different locales
 * with Procedural and Object methods.
 */

function ut_main()
{
    $res_str = '';

    $disp_locales=array('en','fr','de');

    $locales = array(
        'uk-ua_CALIFORNIA@currency=;currency=GRN',
        '',
        'root',
        'uk@currency=EURO',
        'Hindi',
//Simple language subtag
        'de',
        'fr',
        'ja',
        'i-enochian', //(example of a grandfathered tag)
//Language subtag plus Script subtag:
        'zh-Hant',
        'zh-Hans',
        'sr-Cyrl',
        'sr-Latn',
//Language-Script-Region
        'zh-Hans-CN',
        'sr-Latn-CS',
//Language-Variant
        'sl-rozaj',
        'sl-nedis',
//Language-Region-Variant
        'de-CH-1901',
        'sl-IT-nedis',
//Language-Script-Region-Variant
        'sl-Latn-IT-nedis',
//Language-Region:
        'de-DE',
        'en-US',
        'es-419',
//Private use subtags:
        'de-CH-x-phonebk',
        'az-Arab-x-AZE-derbend',
//Extended language subtags
        'zh-min',
        'zh-min-nan-Hant-CN',
//Private use registry values
        'x-whatever',
        'qaa-Qaaa-QM-x-southern',
        'sr-Latn-QM',
        'sr-Qaaa-CS',
/*Tags that use extensions (examples ONLY: extensions MUST be defined
   by revision or update to this document or by RFC): */
        'en-US-u-islamCal',
        'zh-CN-a-myExt-x-private',
        'en-a-myExt-b-another',
//Some Invalid Tags:
        'de-419-DE',
        'a-DE',
        'ar-a-aaa-b-bbb-a-ccc'
    );


    $res_str = '';

    foreach( $locales as $locale )
    {
        $res_str .= "locale='$locale'\n";
        foreach( $disp_locales as $disp_locale )
        {
            $scr = ut_loc_get_display_variant( $locale ,$disp_locale );
            $res_str .= "disp_locale=$disp_locale :  display_variant=$scr";
            $res_str .= "\n";
        }
        $res_str .= "-----------------\n";
    }

    return $res_str;

}

include_once( 'ut_common.php' );
ut_run();

?>
--EXPECT--
locale='uk-ua_CALIFORNIA@currency=;currency=GRN'
disp_locale=en :  display_variant=Ukraine
disp_locale=fr :  display_variant=Ukraine
disp_locale=de :  display_variant=Ukraine
-----------------
locale=''
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='root'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='uk@currency=EURO'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='Hindi'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='de'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='fr'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='ja'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='i-enochian'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='zh-Hant'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='zh-Hans'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='sr-Cyrl'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='sr-Latn'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='zh-Hans-CN'
disp_locale=en :  display_variant=China
disp_locale=fr :  display_variant=Chine
disp_locale=de :  display_variant=China
-----------------
locale='sr-Latn-CS'
disp_locale=en :  display_variant=Serbia And Montenegro
disp_locale=fr :  display_variant=Serbie-et-Monténégro
disp_locale=de :  display_variant=Serbien und Montenegro
-----------------
locale='sl-rozaj'
disp_locale=en :  display_variant=ROZAJ
disp_locale=fr :  display_variant=ROZAJ
disp_locale=de :  display_variant=ROZAJ
-----------------
locale='sl-nedis'
disp_locale=en :  display_variant=NEDIS
disp_locale=fr :  display_variant=NEDIS
disp_locale=de :  display_variant=NEDIS
-----------------
locale='de-CH-1901'
disp_locale=en :  display_variant=Switzerland
disp_locale=fr :  display_variant=Suisse
disp_locale=de :  display_variant=Schweiz
-----------------
locale='sl-IT-nedis'
disp_locale=en :  display_variant=Italy
disp_locale=fr :  display_variant=Italie
disp_locale=de :  display_variant=Italien
-----------------
locale='sl-Latn-IT-nedis'
disp_locale=en :  display_variant=Italy
disp_locale=fr :  display_variant=Italie
disp_locale=de :  display_variant=Italien
-----------------
locale='de-DE'
disp_locale=en :  display_variant=Germany
disp_locale=fr :  display_variant=Allemagne
disp_locale=de :  display_variant=Deutschland
-----------------
locale='en-US'
disp_locale=en :  display_variant=United States
disp_locale=fr :  display_variant=États-Unis
disp_locale=de :  display_variant=Vereinigte Staaten
-----------------
locale='es-419'
disp_locale=en :  display_variant=Latin America and the Caribbean
disp_locale=fr :  display_variant=Amérique latine et Caraïbes
disp_locale=de :  display_variant=Lateinamerika und Karibik
-----------------
locale='de-CH-x-phonebk'
disp_locale=en :  display_variant=Switzerland
disp_locale=fr :  display_variant=Suisse
disp_locale=de :  display_variant=Schweiz
-----------------
locale='az-Arab-x-AZE-derbend'
disp_locale=en :  display_variant=X
disp_locale=fr :  display_variant=X
disp_locale=de :  display_variant=X
-----------------
locale='zh-min'
disp_locale=en :  display_variant=MIN
disp_locale=fr :  display_variant=MIN
disp_locale=de :  display_variant=MIN
-----------------
locale='zh-min-nan-Hant-CN'
disp_locale=en :  display_variant=MIN
disp_locale=fr :  display_variant=MIN
disp_locale=de :  display_variant=MIN
-----------------
locale='x-whatever'
disp_locale=en :  display_variant=
disp_locale=fr :  display_variant=
disp_locale=de :  display_variant=
-----------------
locale='qaa-Qaaa-QM-x-southern'
disp_locale=en :  display_variant=QM
disp_locale=fr :  display_variant=QM
disp_locale=de :  display_variant=QM
-----------------
locale='sr-Latn-QM'
disp_locale=en :  display_variant=QM
disp_locale=fr :  display_variant=QM
disp_locale=de :  display_variant=QM
-----------------
locale='sr-Qaaa-CS'
disp_locale=en :  display_variant=Serbia And Montenegro
disp_locale=fr :  display_variant=Serbie-et-Monténégro
disp_locale=de :  display_variant=Serbien und Montenegro
-----------------
locale='en-US-u-islamCal'
disp_locale=en :  display_variant=United States
disp_locale=fr :  display_variant=États-Unis
disp_locale=de :  display_variant=Vereinigte Staaten
-----------------
locale='zh-CN-a-myExt-x-private'
disp_locale=en :  display_variant=China
disp_locale=fr :  display_variant=Chine
disp_locale=de :  display_variant=China
-----------------
locale='en-a-myExt-b-another'
disp_locale=en :  display_variant=A
disp_locale=fr :  display_variant=A
disp_locale=de :  display_variant=A
-----------------
locale='de-419-DE'
disp_locale=en :  display_variant=Latin America and the Caribbean
disp_locale=fr :  display_variant=Amérique latine et Caraïbes
disp_locale=de :  display_variant=Lateinamerika und Karibik
-----------------
locale='a-DE'
disp_locale=en :  display_variant=Germany
disp_locale=fr :  display_variant=Allemagne
disp_locale=de :  display_variant=Deutschland
-----------------
locale='ar-a-aaa-b-bbb-a-ccc'
disp_locale=en :  display_variant=A
disp_locale=fr :  display_variant=A
disp_locale=de :  display_variant=A
-----------------