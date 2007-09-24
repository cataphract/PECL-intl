<?php

/**
 * Message Format provides for runtime formatting of messages in a manner
 * somewhat similar to sprintf. The pattern string has its component parts
 * replaced in a locale-sensitive manner using items in the arguments array.
 *
 * @see http://www.icu-project.org/apiref/icu4c/umsg_8h.html
 *
 */
class MessageFormatter {

   /**
    * Constructs a new Message Formatter
    * 
    * @param  string   $locale  the locale to use when formatting arguments
    * @param  string   $pattern the pattern string to stick arguments into
    */
   public function __construct($locale, $pattern) {}

   /**
    * Constructs a new Message Formatter
    * 
    * @param  string   $locale  the locale to use when formatting arguments
    * @param  string   $pattern the pattern string to stick arguments into
    */
   public static function create($locale, $pattern) {}

   /**
    * @param   array   $args   arguments to insert into the pattern string
    * @return  string  the formatted string, or 'null' if an error ocurred
    */
    public function format($args) {}

    /**
     * parses input string and returns any extracted items as an array
     *
     * $error will contain any error code. If an error occurs, $parse_pos contains
     * the position of the error.
     *
     * @param  string  $value      string to parse for items
     * @param  integer $parse_pos  starting position, zero-based, in $value
     * @param  integer $error      error code
     * @return array               array containing items extracted
     *
     */
     public function parse($value, $parse_pos = 0, $error = 0) {}

     /**
      * Inserts the items in $args into $pattern, formatting them
      * according to $locale. This is the static implementation.
      *
      * @param string    $locale   the locale to use when formatting numbers and dates and suchlike
      * @param string    $pattern  the pattern string to insert things into
      * @param array     $args     the array of values to insert into $pattern
      * @param [integer] $error    an integer in which any error codes will be returned
      * @return string             the formatted pattern string or 'null' if an error occured
      */
     public static function formatMessage($locale, $pattern, $args, $error = 0) {}

    /**
     * parses input string and returns any extracted items as an array
     *
     * $error will contain any error code. If an error occurs, $parse_pos contains
     * the position of the error.
     *
     * @param  string  $locale     the locale to use when formatting numbers and dates and suchlike
     * @param  string  $value      string to parse for items
     * @param  integer $parse_pos  starting position, zero-based, in $value
     * @param  integer $error      error code
     * @return array               array containing items extracted
     *
     */
     public static function parseMessage($locale, $value, $parse_pos = 0, $error = 0) {}

     /**
      * @return  string  the pattern string for this message formatter
      */
     public function getPattern() {}

     /**
      * @param  string  $pattern  the pattern string to use in this message formatter
      * @return boolean 'true' if successful, 'false' if an error
      */
     public function setPattern($pattern) {}

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


   /**
    * Constructs a new Message Formatter
    * 
    * @param  string   $locale  the locale to use when formatting arguments
    * @param  string   $pattern the pattern string to stick arguments into
    * @return MessageFormatter formatter object
    */
   function msgfmt_create($locale, $pattern) {}

   /**
    * @param MessageFormatter $fmt The message formatter 
     * @param   array   $args   arguments to insert into the pattern string
    * @return  string  the formatted string, or 'null' if an error ocurred
    */
  function msgfmt_format($fmt, $args) {}

    /**
     * parses input string and returns any extracted items as an array
     *
     * $error will contain any error code. If an error occurs, $parse_pos contains
     * the position of the error.
     *
     * @param MessageFormatter $fmt The message formatter 
     * @param  string  $value      string to parse for items
     * @param  integer $parse_pos  starting position, zero-based, in $value
     * @param  integer $error      error code
     * @return array               array containing items extracted
     *
     */
   function msgfmt_parse($fmt, $value, $parse_pos = 0, $error = 0) {}

     /**
      * Inserts the items in $args into $pattern, formatting them
      * according to $locale. This is the static implementation.
      *
      * @param string    $locale   the locale to use when formatting numbers and dates and suchlike
      * @param string    $pattern  the pattern string to insert things into
      * @param array     $args     the array of values to insert into $pattern
      * @param [integer] $error    an integer in which any error codes will be returned
      * @return string             the formatted pattern string or 'null' if an error occured
      */
    function msgfmt_format_message($locale, $pattern, $args, $error = 0) {}

    /**
     * parses input string and returns any extracted items as an array
     *
     * $error will contain any error code. If an error occurs, $parse_pos contains
     * the position of the error.
     *
     * @param  string  $locale     the locale to use when formatting numbers and dates and suchlike
     * @param  string  $value      string to parse for items
     * @param  integer $parse_pos  starting position, zero-based, in $value
     * @param  integer $error      error code
     * @return array               array containing items extracted
     *
     */
    function msgfmt_parse_message($locale, $value, $parse_pos = 0, $error = 0) {}

     /**
     * @param MessageFormatter $fmt The message formatter 
      * @return  string  the pattern string for this message formatter
      */
    function msgfmt_get_pattern($fmt) {}

     /**
      * @param MessageFormatter $fmt The message formatter 
      * @param  string  $pattern  the pattern string to use in this message formatter
      * @return boolean 'true' if successful, 'false' if an error
      */
    function msgfmt_set_pattern($fmt, $pattern) {}

    /**
     * Get the error code from last operation
     *
     * Returns error code from the last number formatting operation.
     *
     * @param MessageFormatter $fmt The message formatter 
     * @return integer the error code, one of UErrorCode values. Initial value is U_ZERO_ERROR.
     */
    function msgfmt_get_error_code($fmt) {}
    /**
     * Get the error text from the last operation.
     *
     * @param MessageFormatter $fmt The message formatter 
     * @return string Description of the last error.
     */
    function msgfmt_get_error_message($fmt) {}

?>
