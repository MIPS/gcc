#ifndef IPA_TBAA_MODREF_H
#define IPA_TBAA_MODREF_H

#include <vector>

/*
 * Compound alias-set for a memory access.
 */
struct ipa_tbaa_caset
{
  alias_set_type base;	/* base alias set */
  alias_set_type ref;	/* reference alias set */
};

/*
 * Single function summary.
 */
struct ipa_tbaa_funcref
{
  const char *fname;
  std::vector<ipa_tbaa_caset> loads;  /* compound alias sets for loads */
  std::vector<ipa_tbaa_caset> stores; /* compound alias sets for stores */
  bool finished;
};

ipa_tbaa_funcref *get_ipa_tbaa_summary (cgraph_node *func);

#endif
