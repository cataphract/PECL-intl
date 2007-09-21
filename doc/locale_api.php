<?php

/**
 * A "Locale" is an identifier used to get language, culture, or regionally-specific 
 * behavior from an API. PHP locales are organized and identified the same
 * way that the CLDR locales used by ICU (and many vendors of Unix-like operating 
 * systems, the Mac, Java, and so forth) use. Locales are identified using
 * RFC 4646 language tags (which use hyphen, not underscore) in addition to the
 * more traditional underscore-using identifiers. Unless otherwise noted
 * the functions in this class are tolerant of both formats. 
 *
 * Examples of identifiers include:
 *
 *  * en-US (English, United States)
 *  * zh-Hant-TW (Chinese, Traditional Script, Taiwan)
 *  * fr-CA, fr-FR (French for Canada and France respectively) 
 *
 * The Locale class (and related procedural functions) are used to interact 
 * with locale identifiers--to verify that an ID is well-formed, valid, 
 * etc. The extensions used by CLDR in UAX #35 (and inherited by ICU) are 
 * valid and used wherever they would be in ICU normally.
 *
 * Locales cannot be instantiated as objects. All of the functions/methods 
 * provided are static.
 *
 *   * The null or empty string obtains the "root" locale. 
 *     The "root" locale is equivalent to "en_US_POSIX" in CLDR.
 *   * Language tags (and thus locale identifiers) are case insensitive. There
 *     exists a canonicalization function to make case match the specification.
 *
 * @see http://www.icu-project.org/apiref/icu4c/uloc_8h.html
 * @see http://www.unicode.org/reports/tr35/
 *
 */
class Locale {

   /**
    * members
    */

   /**
    * The following static members are used with the getLocale methods of
    * the various locale affected classes, such as numfmt.
    */

   /**
    *  identifier for the actual locale, equivalent to ULOC_ACTUAL_LOCALE
    */
   const ACTUAL_LOCALE = 0;
   /**
    * identifier for the valid locale, equivalent to ULOC_VALID_LOCALE
    */
   const VALID_LOCALE = 1;

   /**
    * member functions
    */

   /**
    * @return string the current runtime locale as an RFC 4646 language tag, 
    * normalized according to case mapping conventions in RFC 4646 
    * Section 2.1.
    */
    public static function getDefault() {}

   /*
    * sets the default runtime locale to $locale
    *
    * @param string $locale is a BCP 47 compliant language tag containing the 
    * locale identifier. UAX #35 extensions are accepted.
    * @return boolean 'true' if okay, 'false' if in error
    */
    public static function setDefault($locale) {}


    /*
     * @param string $locale the locale to extract the primary language code from
     * @return string the language code associated with the language (or 
     * a grandfathered language tag). This will not be an empty 
     * value except for the root locale. Deprecated values will be 
     * mapped to modern equivalents. 
     *
     * The language subtag must always be present. Returns 'null' if an
     * error occured extracting the language subtag (as with an invalid
     * locale code).
     */
    public static function getPrimaryLanguage($locale) {}

    /*
     * @param string $locale the locale to extract the script code from
     * @return string the script subtag for the locale or the empty string
     * if the script is not assigned. Note that many locales do not
     * assign a script code.
     *
     * If no script is present, returns the empty string.
     */
     public static function getScript($locale) {}

    /*
     * @param string $locale the locale to extract the region code from
     * @return string the region subtag for the locale or the empty string
     * if the region is not assigned. 
     * Valid values include both ISO 3166 country codes and UN
     * M.49 region codes (which are numeric strings).
     *
     * If no region is present, returns the empty string.
     */
     public static function getRegion($locale) {}


    /*
     * @param string $locale the locale to extract the first variant code from
     * @return string the first variant subtag for the locale or the empty string
     * Note that there can be multiple variants. In practice
     * multiple variants are rare. Obtain the complete list of variants using
     * getAllVariants().
     *
     * If no variant is present, returns the empty string.
     */
     public static function getVariant($locale) {}


     /*
      * @param string $locale the locale to extract the variants from
      * @return array an array containing the list of variants, or null
      * if there are no variants. The array preserves the variant order.
      */
      public static function getAllVariants($locale) {}


      /*
       * @param string $locale locale to extract the keywords from
       * @return an associative array containing keyword-value
       * pairs for this locale. The keys are keys to the array (doh!)
       */
       public static function getKeywords($locale) {}

       /*
        * @param string $locale - the locale to return a displayname for
	* @param [string] $in_locale - optional format locale
	*
	* @return string - display name of the locale in the format
	* appropriate for $in_locale. If $in_locale is 'null' 
	* then the default locale is used. 
	*
	* For example, if the default locale is "de", getDisplayName("de") returns
	* "Deutsch" while getDisplayName("de", "en-US") returns "German".
	*/
        public static function getDisplayName($locale, $in_locale = null) {}

	/*
	 * Returns an appropriately localized display name for the language
	 * subtag $lang. For example, the language subtag for locale
	 * "en-US" is "en". Thus getDisplayLanguage(getLanguage("en-US"), "en-US")
	 * would return the string "English".
	 *
	 * Language must always be present. If an error occurs returns null.
	 *
	 * @param  string  $lang      language code to get display value for
	 * @param  string  $in_locale locale to use to display the language name
	 * @return string  display name for $lang
	 */
	public static function getDisplayLanguage($lang, $in_locale = null) {}

	/*
	 * Returns an appropriately localized display name for the script 
	 * subtag $script. For example, the script subtag for locale
	 * "zh-Hant-TW" is "Hant". Thus getDisplayScript(getScript("zh-Hant-TW"), "en-US")
	 * would return the string "Traditional Chinese".
	 *
	 * If no script is present, returns null.
	 *
	 * @param  string  $script    script code to get the display name for
	 * @param  string  $in_locale locale to use to display the script name
	 * @return string  display name for $script
	 */
	public static function getDisplayScript($script, $in_locale = null) {}

	/*
	 * Returns an appropriately localized display name for the region 
	 * subtag $region. For example, the region subtag for locale
	 * "en-US" is "US". Thus getDisplayRegion(getRegion("en-US"), "en-US")
	 * would return the string "United States".
	 *
	 * If no $region is empty or an error occurs, returns null.
	 *
	 * @param  string  $region    region to get the display name for
	 * @param  string  $in_locale locale to use to display the region name
	 * @return string  display name for $region
	 */
	public static function getDisplayRegion($region, $in_locale = null) {}

	/*
	 * Returns an appropriately localized display name for the variant 
	 * subtag $variant. For example, the variant subtag 
	 * for locale "sl-IT-rozaj" is "rozaj". 
	 * getDisplayVariants(getVariant("sl-IT-rozaj"), "en-US") would return 
	 * the string "Resian".
	 *
	 * Note that a locale can contain multiple variants. You will need to iterate
	 * over the list of variants in order to obtain each display name.
	 *
	 * If no $variant is empty or an error occurs, returns null.
	 *
	 * @param  string  $variant   variant to get a display name for
	 * @param  string  $in_locale locale to use to display the variant's name
	 * @return string  display name for $variant
	 */
	public static function getDisplayVariant($variant, $in_locale = null) {}

       /*
        * @param string $langtag the language tag to check
	* @param string $locale the language range to check against
        * @return boolean 'true' if $locale matches $langtag according to RFC 4647's 
	* basic filtering algorithm. That is, if $locale is "de-DE", then 
	* langtag "de-Deva" would not match while "de-De-xyzzy" does. 
	*
	* Note that $langtag is the 'tag' and $locale is the 'range' in 
	* this function call. This functionality is not implemented by ICU
	* and is provided as a utility for doing language neogitation.
	*/
        public static function filterMatches($langtag, $locale) {}

       /*
        * Searchs the items in $langtag for the best match to the language
	* range specified in $locale according to RFC 4647's lookup
	* algorithm. $default is returned if none of the tags in $langtag
	* match. If $default is not provided, returns the empty string.
	*
	* @param array $langtag - an array containing a list of language tags to compare
	*            to $locale
	* @param string $locale  - the locale to use as the language range when matching
	* @param string $default - the locale to use if no match is found
	* @return string closest matching language tag, $default, or empty string
	*/
        public static function lookup(array $langtag, $locale, $default = null) {}

	/* 
	 * @param string $langtag	the language tag to check
	 * @return boolean 'true' if $langtag is "well-formed" according to the ABNF
	 * in RFC 4646, 'false' otherwise.
	 *
	 * Note that the tag may still not be valid.
	 */
         public static function isWellFormed($langtag) {}

        /*
	 * @param	string $langtag	the language tag to check
	 * @return boolean 'true' if $langtag is well-formed according to
	 * RFC 4646 and if all of the subtags are also valid, according
	 * to the copy of the IANA Language Subtag Registry in this library.
	 *
	 * Note that CLDR (ICU) extensions to language tags are implemented
	 * as private use codes and do not interfere with validity checking.
	 */ 
         public static function isValid($langtag) {}

        /*
	 * @param string $langtag	The language tag string to canonicalize
	 * @return string the canonicalized language tag form of $langtag.
         * Deprecated subtags are mapped to modern equivalents and 
	 * case is normalized according to the rules in RFC 4646. 
	 * ICU-style locale identifiers are converted into the 
	 * language tag form by replacing underscores with hyphens
	 *
	 * Note that invalid subtags are preserved. A canonicalized
	 * language tag may not be valid.
	 */
         public static function canonicalize($langtag) {}

        /*
	 * @return string the IANA Language Subtag Registry date used by this
	 * version of the library as a string in the RFC 3339 full-date
	 * format. This is useful in debugging (when tags are unexpectedly 
	 * invalid, for example).
	 */
         public static function getRegistryDate() {}

}

/** Now the same as procedural API */


?>
