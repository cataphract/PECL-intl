<?php

/**
 * Date Formatter class - locale-dependent formatting/parsing of dates.
 *
 * This class represents the ICU date formatting functionality. It allows users to
 * display dates in a localized format or to parse strings that follow a pre-determined
 * pattern into PHP date values.
 * 
 * Example:
 * <code>
 * $datefmt = new DateFormatter("de-DE", LONG, SHORT, date_default_timezone_get());
 * echo $formatter->format(time());
 * </code>
 *
 * @see 
 * @see 
 *
 */
class DateFormatter {

   /**
    * The following constants are used to specify different formats
    * in the constructor.
    */
    const NONE = -1;
    const FULL = 0;
    const LONG = 1;
    const MEDIUM = 2;
    const SHORT = 3;

    /**
     * The following int constants are used to specify the calendar. 
     * These calendars are all based directly on the Gregorian calendar 
     * Non-Gregorian calendars need to be specified in locale. 
     * Examples might include locale="hi@calendar=BUDDHIST"
     */
    const TRADITIONAL = 0 // non-Gregorian calendar that is locale-defined, required by ICU
    const GREGORIAN = 1 // Gregorian calendar
	
	/**
	 * Create a date formatter 
	 *
	 * @param string  $locale     Locale to use when formatting or parsing
	 * @param integer $datetype   Date type to use (none, short, medium, long, full)
	 * @param integer $timetype   Time type to use (none, short, medium, long, full)
	 * @param [mixed] $timezone   Time zone ID or DateTimeZone object; default is system default
	 * @param [integer] $calendar Calendar to use for formatting or parsing; default is
	 *                            GREGORIAN
	 * @return DateFormatter
	 * @see __construct
	 * @see datefmt_create
	 */
	public static function __construct($locale, $datetype, $timetype, $timezone = null, $calendar= null) {}

	/**
	 * Create a date formatter 
	 *
	 * @param string  $locale     Locale to use when formatting or parsing
	 * @param integer $datetype   Date type to use (none, short, medium, long, full)
	 * @param integer $timetype   Time type to use (none, short, medium, long, full)
	 * @param [mixed] $timezone   Time zone ID or DateTimeZone object; default is system default
	 * @param [integer] $calendar Calendar to use for formatting or parsing; default is
	 *                            GREGORIAN
	 * @return DateFormatter
	 * @see __construct
	 * @see datefmt_create
	 */
	public static function create($locale, $datetype, $timetype, $timezone = null, $calendar= null) {}

	/**
         * formats the time value as a string.
	 * @param [mixed] $value - value to format
         *     integer: a unix timestamp value (seconds since epoch, UTC)
         *     array: a localtime array 
	 *     DateTime: a DateTime value
         * @return string  a formatted string or, if an error occurred, 'null'. 
	 */
        public function format($value) {}

        /**
         * converts string $value to a DateTime object
	 *
	 * @param string $value   string to convert to a time
	 * @return DateTime       value of the parsed string as a DateTime or null if error
	 */
         public function parseDate($value) {}

	 /**
	  * converts the string $value to a Unix timestamp (a time_t integer
	  * value, seconds since epoch).
	  *
	  * @param  string  $value   string to convert to a time
	  * @return integer          timestamp value
	  */
	 public function parseTimestamp($value) {}

	 /**
	  * Field-based equivalent of parse(). 
	  *
	  * @param string $value  String value to parse into a date. 
	  * @return array         An array in the same format as localtime produces
	  * @see http://www.php.net/manual/en/function.localtime.php
	  * @see http://www.w3.org/TR/2005/NOTE-timezone-20051013/
	  */
         public function parseLocaltime($value) {}

        /**
         * converts string $value to an incremental time value, starting at
	 * $parse_pos and consuming as much of the input value as possible
	 * If no error occurs before $value is consumed, $parse_pos will contain -1
	 * otherwise it will contain the position at which parsing ended (and the error
	 * occurred). $error will contain the error code. Note that this function
	 * does not change the value in get_error().
	 * @param string  $value      string to convert to a time
	 * @param integer $parse_pos  position at which to start the parsing in $value (zero-based)
	 *                            This variable will contain the end position if the parse fails
	 *                            If $parse_pos > strlen($value), the parse fails immediately.
	 * @param integer $error      error value of any error that occurs during parsing
	 * @return integer timestamp  parsed value
	 */
	 public function parse($value, $parse_pos, $error) {}
	 
        /**
         * converts string $value to a field-based time value, starting at
	 * $parse_pos and consuming as much of the input value as possible
	 * If no error occurs before $value is consumed, $parse_pos will contain -1
	 * otherwise it will contain the position at which parsing ended (and the error
	 * occurred). $error will contain the error code. Note that this function
	 * does not change the value in get_error().
	 * @param string  $value      string to convert to a time
	 * @param array   $localtime  the array to store the parsed value; values in this array will
	 *                            be overwritten!
	 * @param integer $parse_pos  position at which to start the parsing in $value (zero-based)
	 *                            This variable will contain the end position if the parse fails
	 *                            If $parse_pos > strlen($value), the parse fails immediately.
	 * @param integer $error      error value of any error that occurs during parsing
	 * @return array localtime compatible array of integers
	 */
	 public function localtime($value, $localtime, $parse_pos, $error) {}

	 /**
	  * @return integer the current 'type' value of the formatter
	  */
         public function getType() {}

	 /**
	  * @return boolean   'true' if parser is lenient, 'false' if parser is strict
	  *                   default value for parser is 'false'.
	  */
         public function isLenient() {}

	 /**
	  * @param boolean $lenient  sets whether the parser is lenient or not, default is 'false'
          *                          'true' sets the parser to accept otherwise flawed date or 
	  *                          time patterns, parsing as much as possible to obtain a value.
          *                          'false' sets the parser to strictly parse strings into dates. 
	  *                          Extra space, unrecognized tokens, or invalid values 
	  *                          ("Feburary 30th") are not accepted.
	  *
          * @return boolean          'true' if successful; 'false' if an error occurred. 
	  */
	 public function setLenient($lenient) {}


	 /**
	  * @param  [integer]  which locale should be returned? 
	  *                    values may include ULOC_ACTUAL_LOCALE,
	  *                    ULOC_VALID_LOCALE. By default the actual
	  *                    locale is returned.
	  * @return string     the locale of this formatter  or 'false' if error
	 */

	 public function getLocale($type = ULOC_ACTUAL_LOCALE) {}

         /**
	  * @return string ID string for the time zone used by this formatter
	  */
	 public function getTimeZoneId() {}


	/**
	 * @return DateTimeZone DateTimeZone used by this formatter
	 */
        public function getTimeZone() {}


	/**
	 * sets the time zone to use
	 * @param mixed $zone  DateTimeZone or zone ID string of the time zone to use.
	 *                     if null or the empty string, the default time zone for
	 *                     the runtime is used.
	 * @return boolean 'true' on successful setting of the time zone, 'false'
	 *                 if an error occurred (such as the time zone wasn't recognized).
	 */
        public function setTimeZoneId($zone) {}

        /**
         * sets the calendar used to the appropriate calendar, which must be 
	 * one of the constants defined above. Some examples include:
         *   - Gregorian calendar
         *   - Traditional
         * Default value is GREGORIAN
	 * @param integer $which the calendar (an enumerated constant) to use.
         * @return boolean 'true' if successful, 'false' if an error occurred or if the calendar was not recognized 
	 */
         public function setCalendar($which) {}

        /**
	 * @return integer the calendar being used by the formatter
	 */
         public function getCalendar() {}

	/**
	 * Get the error code from last operation
	 *
	 * Returns error code from the last number formatting operation.
	 *
	 * @return integer the error code, one of UErrorCode values. Initial value is U_ZERO_ERROR.
	 */
	public function getErrorCode() {}
	/**
	 * Get the error text from the last operation.
	 *
	 * @return string Description of the last error.
	 */
	public function getErrorMessage() {}

}

/** Now the same as procedural API */


?>
