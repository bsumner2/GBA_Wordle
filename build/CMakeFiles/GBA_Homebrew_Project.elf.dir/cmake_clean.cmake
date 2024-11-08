file(REMOVE_RECURSE
  "GBA_Homebrew_Project.elf"
  "GBA_Homebrew_Project.elf.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/GBA_Homebrew_Project.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
