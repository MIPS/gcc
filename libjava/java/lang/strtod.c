	{
	  /* No exponent after an 'E' : that's an error. */
	  ptr->_errno = EINVAL;
	  e = 0;
	  s = s00;
	  goto ret;
	}
