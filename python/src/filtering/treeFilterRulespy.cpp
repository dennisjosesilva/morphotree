#include "filtering/treeFilterRulespy.hpp"

void bindFoundamentalTypesMaxRuleFilter(py::module &m)
{
  bindMaxRuleFilter<mt::uint8>(m, "UI8");
  bindMaxRuleFilter<mt::int8>(m, "I8");
  bindMaxRuleFilter<mt::uint32>(m, "UI32");
  bindMaxRuleFilter<mt::int32>(m, "I32");

  bindIMaxRuleFilter<mt::uint8>(m, "UI8");
  bindIMaxRuleFilter<mt::int8>(m, "I8");
  bindIMaxRuleFilter<mt::uint32>(m, "UI32");
  bindIMaxRuleFilter<mt::int32>(m, "I32");
}