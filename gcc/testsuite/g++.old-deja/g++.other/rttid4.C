// Special g++ Options: -w
  // Ill-formed: dynamic_cast to private or ambiguous base
  //   vp = dynamic_cast<B *> (dp);
  //   if (vp == (void *)dp) error(21);
  //   vp = dynamic_cast<B *> (fp);
  //   if (vp == (void *)bbp) error(22);
