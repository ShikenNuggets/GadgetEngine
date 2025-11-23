# --------------------------- #
# --- GadgetEngine Demos ---- #
# --------------------------- #
file (GLOB_RECURSE DEMO_SRC_FILES
	"*.cpp"
	"*.h"
	"*.hpp"
)

add_executable(GadgetEngineDemos ${DEMO_SRC_FILES})
target_link_libraries(GadgetEngineDemos PRIVATE GadgetEngine)
