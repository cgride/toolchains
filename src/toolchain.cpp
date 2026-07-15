/**
 *
 *  @file toolchain.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/cgride/toolchains
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Cgride
 *
 */
#include <cgride/toolchains/toolchain.hpp>

#include <utility>

namespace cgride::toolchains
{
  Toolchain::Toolchain(CompilerKind kind)
      : compiler_kind_(kind)
  {
  }

  Toolchain::Toolchain(CompilerKind kind, std::string name)
      : compiler_kind_(kind),
        name_(std::move(name))
  {
  }

  Toolchain &Toolchain::compiler_kind(CompilerKind kind) noexcept
  {
    compiler_kind_ = kind;
    return *this;
  }

  Toolchain &Toolchain::name(std::string name)
  {
    name_ = std::move(name);
    return *this;
  }

  Toolchain &Toolchain::c_compiler(std::filesystem::path path)
  {
    c_compiler_ = std::move(path);
    return *this;
  }

  Toolchain &Toolchain::cxx_compiler(std::filesystem::path path)
  {
    cxx_compiler_ = std::move(path);
    return *this;
  }

  Toolchain &Toolchain::archiver(std::filesystem::path path)
  {
    archiver_ = std::move(path);
    return *this;
  }

  Toolchain &Toolchain::linker(std::filesystem::path path)
  {
    linker_ = std::move(path);
    return *this;
  }

  Toolchain &Toolchain::target_triple(std::string triple)
  {
    target_triple_ = std::move(triple);
    return *this;
  }

  Toolchain &Toolchain::sysroot(std::filesystem::path path)
  {
    sysroot_ = std::move(path);
    return *this;
  }

  Toolchain &Toolchain::default_include_directory(std::filesystem::path path)
  {
    default_include_directories_.push_back(std::move(path));
    return *this;
  }

  Toolchain &Toolchain::default_library_directory(std::filesystem::path path)
  {
    default_library_directories_.push_back(std::move(path));
    return *this;
  }

  Toolchain &Toolchain::builtin_compile_option(std::string option)
  {
    builtin_compile_options_.push_back(std::move(option));
    return *this;
  }

  Toolchain &Toolchain::builtin_link_option(std::string option)
  {
    builtin_link_options_.push_back(std::move(option));
    return *this;
  }

  CompilerKind Toolchain::compiler_kind() const noexcept
  {
    return compiler_kind_;
  }

  const std::string &Toolchain::name() const noexcept
  {
    return name_;
  }

  const std::optional<std::filesystem::path> &Toolchain::c_compiler() const noexcept
  {
    return c_compiler_;
  }

  const std::optional<std::filesystem::path> &Toolchain::cxx_compiler() const noexcept
  {
    return cxx_compiler_;
  }

  const std::optional<std::filesystem::path> &Toolchain::archiver() const noexcept
  {
    return archiver_;
  }

  const std::optional<std::filesystem::path> &Toolchain::linker() const noexcept
  {
    return linker_;
  }

  std::optional<std::filesystem::path> Toolchain::compiler_for(Language language) const
  {
    switch (language)
    {
    case Language::C:
      return c_compiler_;

    case Language::Cxx:
      return cxx_compiler_;

    case Language::Unknown:
      return std::nullopt;
    }

    return std::nullopt;
  }

  const std::string &Toolchain::target_triple() const noexcept
  {
    return target_triple_;
  }

  const std::optional<std::filesystem::path> &Toolchain::sysroot() const noexcept
  {
    return sysroot_;
  }

  const std::vector<std::filesystem::path> &Toolchain::default_include_directories() const noexcept
  {
    return default_include_directories_;
  }

  const std::vector<std::filesystem::path> &Toolchain::default_library_directories() const noexcept
  {
    return default_library_directories_;
  }

  const std::vector<std::string> &Toolchain::builtin_compile_options() const noexcept
  {
    return builtin_compile_options_;
  }

  const std::vector<std::string> &Toolchain::builtin_link_options() const noexcept
  {
    return builtin_link_options_;
  }

  bool Toolchain::has_c_compiler() const noexcept
  {
    return c_compiler_.has_value() && !c_compiler_->empty();
  }

  bool Toolchain::has_cxx_compiler() const noexcept
  {
    return cxx_compiler_.has_value() && !cxx_compiler_->empty();
  }

  bool Toolchain::has_archiver() const noexcept
  {
    return archiver_.has_value() && !archiver_->empty();
  }

  bool Toolchain::has_linker() const noexcept
  {
    return linker_.has_value() && !linker_->empty();
  }

  bool Toolchain::can_compile_cxx() const noexcept
  {
    return has_cxx_compiler();
  }

  bool Toolchain::valid() const noexcept
  {
    return is_known(compiler_kind_) && can_compile_cxx();
  }

} // namespace cgride::toolchains
