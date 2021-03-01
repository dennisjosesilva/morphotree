#pragma once

#include "morphotree/core/box.hpp"

#include <iostream>
#include <vector>

namespace morphotree
{
  template<typename T>
  void printImageIntoConsole(const std::vector<T> &f, const Box &domain);

  template<typename T, typename Cast>
  void printImageIntoConsoleWithCast(const std::vector<T> &f, const Box &domain);

  template<typename T>
  void printImageIntoConsole(const std::vector<T> &f, const Box::SubBox &domain);

  template<typename T>
  void printImageIntoConsoleWithCast(const std::vector<T> &f, Box::SubBox &domain);


  // ==================== [IMPLEMENTATION] ================================
  template<typename T>
  void printImageIntoConsole(const std::vector<T> &f, const Box &domain)
  {
    for (int y = domain.top(); y <= domain.bottom(); y++) {
      for (int x = domain.left(); x < domain.right(); x++) {
        std::cout << f[domain.pointToIndex(x, y)] << " ";
      }
      std:: cout << f[domain.pointToIndex(domain.right(), y)] << "\n"; 
    }
  }

  template<typename Cast, typename T>
  void printImageIntoConsoleWithCast(const std::vector<T> &f, const Box &domain)
  {
    for (int y = domain.top(); y <= domain.bottom(); y++) {
      for (int x = domain.left(); x < domain.right(); x++) {
        std::cout << static_cast<Cast>(f[domain.pointToIndex(x, y)]) << " ";
      }
      std:: cout << static_cast<Cast>(f[domain.pointToIndex(domain.right(), y)]) << "\n"; 
    }
  }

  template<typename T>
  void printImageIntoConsole(const std::vector<T> &f, const Box::SubBox &domain)
  {
    for (int y = domain.top(); y <= domain.bottom(); y++) {
      for (int x = domain.left(); x < domain.right(); x++) {
        std::cout << f[domain.pointToIndex(x, y)] << " ";
      }
      std:: cout << f[domain.pointToIndex(domain.right(), y)] << "\n"; 
    }
  }

  template<typename Cast, typename T>
  void printImageIntoConsoleWithCast(const std::vector<T> &f, const Box::SubBox &domain)
  {
    for (int y = domain.top(); y <= domain.bottom(); y++) {
      for (int x = domain.left(); x < domain.right(); x++) {
        std::cout << static_cast<Cast>(f[domain.pointToIndex(x, y)]) << " ";
      }
      std:: cout << static_cast<Cast>(f[domain.pointToIndex(domain.right(), y)]) << "\n"; 
    }
  }

}