set(CMAKE_C_FLAGS "-Og -g")
set(CMAKE_CXX_FLAGS "-Og -g")
# set(CMAKE_BUILD_TYPE Debug CACHE INTERNAL "")

set(SCORE_STATIC_PLUGINS 0 CACHE INTERNAL "")
# set(CMAKE_CXX_COMPILER /usr/bin/clazy)

# set(SCORE_COTIRE True CACHE INTERNAL "")
set(DEPLOYMENT_BUILD False)
set(SCORE_COVERAGE False)

include(default-plugins)
