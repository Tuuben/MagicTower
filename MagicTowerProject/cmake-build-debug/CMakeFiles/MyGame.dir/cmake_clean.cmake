file(REMOVE_RECURSE
  "bin/MyGame.pdb"
  "bin/MyGame"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/MyGame.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
