<?php

#############################################################################
# Grapheme API
#############################################################################

	/**
	 * Get string length in grapheme units
	 * @param  string	$input		The string being measured for length.
	 * @return int		The length of the string on success, and 0 if the string is empty.
	*/
	function grapheme_strlen($input) {}

	/**
	 * Find position (in grapheme units) of first occurrence of a string
	 * @param string	$haystack	The string to look in
	 * @param string	$needle		The string to look for
	 * @param [int]		$offset		The optional offset parameter allows you to specify 
						which character in haystack  to start searching. The position 
						returned is still relative to the beginning of haystack.
	 * @return int		Returns the position as an integer. If needle is not found, strpos() will return boolean FALSE.
	*/
	function grapheme_strpos($haystack, $needle, $offset = 0) {}

	
	/**
         * Find position (in grapheme units) of first occurrence of a case-insensitive string
         * @param string        $haystack       The string to look in
         * @param string        $needle         The string to look for
         * @param [int]         $offset         The optional offset parameter allows you to specify
                                                which character in haystack  to start searching. The position
                                                returned is still relative to the beginning of haystack.
         * @return int          Returns the position as an integer. If needle is not found, grapheme_stripos() will return boolean FALSE.
        */
        function grapheme_stripos($haystack, $needle, $offset = 0) {}
 

	/**
         * Find position (in grapheme units) of last occurrence of a string
         * @param string        $haystack       The string to look in
         * @param string        $needle         The string to look for
         * @param [int]         $offset         The optional offset parameter allows you to specify
                                                which character in haystack  to start searching. The position
                                                returned is still relative to the beginning of haystack.
         * @return int          Returns the position as an integer. If needle is not found, grapheme_strrpos() will return boolean FALSE.
        */
        function grapheme_strrpos($haystack, $needle, $offset = 0) {}


	/**
         * Find position (in grapheme units) of last occurrence of a case-insensitive string
         * @param string        $haystack       The string to look in
         * @param string        $needle         The string to look for
         * @param [int]         $offset         The optional offset parameter allows you to specify
                                                which character in haystack  to start searching. The position
                                                returned is still relative to the beginning of haystack.
         * @return int          Returns the position as an integer. If needle is not found, grapheme_strripos() will return boolean FALSE.
        */
        function grapheme_strripos($haystack, $needle, $offset = 0) {}


	/**
	 * Return part of a string
	 * @param string	$string		The input string.
	 * @param int		$start		If start  is non-negative, the returned string will start at the 
						start'th position in string, counting from zero. If start is negative,
						the returned string will start at the start'th character from the 
						end of string.
	 * @param [int]		$length		If length  is given and is positive, the string returned will contain
						at most length characters beginning from start (depending on the 
						length of string). If string is less than or equal to start characters
						long, FALSE  will be returned. If length is given and is negative, then
						that many characters will be omitted from the end of string (after the
						start position has been calculated when a start is negative). If start
						denotes a position beyond this truncation, an empty string will be returned.
	 * @return int		Returns the extracted part of string.
	*/
	function grapheme_substr($string, $start, $length = -1) {}


	/**
	 * Returns part of haystack string from the first occurrence of needle to the end of haystack.
	 * @param string	$haystack	The input string.
	 * @param string	$needle		The string to look for.
	 * @param [boolean]	$before_needle	If TRUE (the default is FALSE), grapheme_strstr() returns the part of the
						haystack before the first occurence of the needle.
	 * @return string	Returns the portion of string, or FALSE if needle is not found.
	*/
	function grapheme_strstr($haystack, $needle, $before_needle = FALSE) {}


	/**
	 * Returns part of haystack string from the first occurrence of case-insensitive needle to the end of haystack.
         * @param string        $haystack       The input string.
         * @param string        $needle         The string to look for.
         * @param [boolean]     $before_needle  If TRUE (the default is FALSE), grapheme_strstr() returns the part of the
                                                haystack before the first occurence of the needle.
         * @return string       Returns the portion of string, or FALSE if needle is not found.
        */
        function grapheme_stristr($haystack, $needle, $before_needle = FALSE) {}


	/**
	 * Function to extract a sequence of default grapheme clusters from a text buffer, which must be encoded in UTF-8.
	 * @param string 	$haystack	string to search
	 * @param int		$size		maximum number of grapheme clusters to return
	 * @param [int]		$start		starting position in $haystack in bytes
	 * @return string	A string starting at offset $start containing no more than $size grapheme clusters 
				and ending on a default grapheme cluster boundary.
	*/
	public function grapheme_extract($haystack, $size, $start = 0) {}


	/**
	 * Function to extract a sequence of default grapheme clusters from a text buffer, which must be encoded in UTF-8.
	 * @param string	$haystack	string to search
	 * @param int		$bsize		maximum number of bytes to return
	 * @param [int]		$start		starting position in $haystack in bytes
	 * @return string	A string starting at offset $start containing no more than $bsize bytes 
				and ending on a default grapheme cluster boundary.
  	*/
	public function grapheme_extractB($haystack, $bsize, $start = 0) {}


?>


