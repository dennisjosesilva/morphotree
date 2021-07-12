
#include <iostream>
#include <vector>
// #include "morphotree/filtering/globalOptimiser/MinCPerimeterWithAbsError.hpp"

#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithAbsErrorToS.hpp"
#include "morphotree/filtering/globalOptimiser/MinCPerimeterWithSquaredErrorToS.hpp"

#include "morphotree/attributes/bitquads/quadCountTreeOfShapesComputer.hpp"
#include "morphotree/attributes/bitquads/quads.hpp"
#include "morphotree/tree/mtree.hpp"

#include "morphotree/core/io.hpp"


namespace mt = morphotree;

int main(int argc, char *argv[]) 
{
  using ValueType = mt::uint8;

  std::vector<ValueType> f = {
    0, 0, 0, 0, 0, 0, 0,
    0, 4, 4, 4, 7, 7, 7,
    0, 7, 7, 4, 7, 4, 7,
    0, 7, 4, 4, 7, 4, 7,
    0, 4, 4, 4, 7, 4, 7,
    0, 7, 7, 4, 7, 7, 7,
    0, 0, 0, 0, 0, 0, 0
  };

  mt::Box domain = mt::Box::fromSize(mt::UI32Point{7, 7});

  float maxError = 0.0f;
  float percentageApprox = 0.05f;

  mt::uint8 rootLevel = f[0];
  for (mt::uint32 p = 0; p < domain.numberOfPoints(); p++)
    maxError += fabsf32(f[p] - rootLevel);

  // mt::MinCPerimeterWithAbsErrorToS<ValueType> filter(domain, f, 5.0f);
  mt::MinCPerimeterWithSquaredErrorToS<ValueType> filter(domain, f, 5.0f);

  // std::vector<ValueType> f_filtered = filter.filterWithNormalisedError(0.93f);
  std::vector<ValueType> f_filtered = filter.filter(170);

  std::cout << "squared error: " << filter.squaredError() << "\n"
            << "sum perimeter: " << filter.fsumPerimeter() << "\n"
            << "normalised squared error: " << filter.normSquaredError() << "\n"
            << "sum filtered nodes: " << filter.numPrunnedNodes();

  std::cout << "\n";
  mt::printImageIntoConsoleWithCast<mt::int32>(f_filtered, domain);
  std::cout << "\n";

  return 0;
}